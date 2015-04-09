// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"
#include "alloc.h"
#include "stack.h"

#include <error.h>
#include <stdio.h>
#include <stdlib.h>

/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */
#define SIZE 1024

/* FIXME: Define the type 'struct command_stream' here.  This should
   complete the incomplete type declaration in command.h.  */

struct commandNode
{
  struct command * command;   // store root of command tree
  struct commandNode * next;
};

typedef struct command_stream
{
  struct commandNode * head;
  struct commandNode * tail;    // use this to insert new nodes
  struct commandNode * cursor;  // use this to advance to next node
} *command_stream_t;

char * create_char_buffer (int (*get_next_byte) (void *),
         void *get_next_byte_argument, int * index);

command_stream_t 
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  /* FIXME: Replace this with your implementation.  You may need to
     add auxiliary functions and otherwise modify the source code.
     You can also use external functions defined in the GNU C Library.  */

  int index = 0;
  command_stream_t command_stream = (command_stream_t) checked_malloc(sizeof(command_stream_t));
  char * char_buffer = create_char_buffer(get_next_byte,get_next_byte_argument, &index);

  struct operator * op_struct;

  commandStack command_stack;
  opStack op_stack;

  CommandStackInit(&command_stack, SIZE);
  OpStackInit(&op_stack, SIZE);

  struct commandNode * command_node = (struct commandNode *)checked_malloc(sizeof(struct commandNode));
  command_stream->head = command_node;
  command_stream->tail = command_node;
  command_stream->cursor = command_node;
  
  int command_word_index = 0;
  int word_index = 0;
  int i = 0;
  
  // creating first command
  command_node->command = (struct command *) checked_malloc(sizeof(struct command));
  command_node->command->u.word = (char **) checked_malloc(SIZE*sizeof(char*));
  char * word = (char *) checked_malloc(SIZE*sizeof(char));


  //until buffer reaches EOF
  while(char_buffer[i] != EOF)
  { 
    // if buffer reaches space or new line
    if (char_buffer[i] == ' ' || char_buffer[i] == '\n')   
    {
      // set end of line to null byte
      word[word_index] = '\0';
      
      //set command_node's word equal to word 
      command_node->command->u.word[command_word_index++] = word;

      // check for more whitespace
      while(1)
      {
        if (char_buffer[i + 1] == ' ') { i++; }
        else { break; }
      }

      // if there are two new lines at end of buffer, make new command tree
      if (char_buffer[i + 1] == '\n')
      {
        // terminate word with null byte
        command_node->command->u.word[command_word_index] = '\0';
        command_node->command->type = SIMPLE_COMMAND;

        // push this command onto command stack
        while (!OpStackIsEmpty(&op_stack))
        {
          struct command * root_command = (struct command *)checked_malloc(sizeof(struct command));
          root_command->type = *(OpStackPop(&op_stack)->op_type);
          root_command->u.command[1] = command_node->command;
          root_command->u.command[0] = CommandStackPop(&command_stack);
          command_node->command = root_command;
        }

        // check for more new lines or EOF
        int more = 2;
        int eof = 0;
        while (1)
        {
          if (char_buffer[i + more] == '\n' || char_buffer[i + more] == ' ') { more++; }          
          else if (char_buffer[i + more] == EOF) { eof = 1; break; }
          else { break; }
        }
        if (eof) { break; }

        struct commandNode * new = (struct commandNode *)checked_malloc(sizeof(struct commandNode));

        // with each new command tree, make new commandNode and set to next
        command_node->next = new;
        command_node = new;
        command_stream->tail = command_node;
        command_word_index = 0;
        command_node->command = (struct command *) checked_malloc(sizeof(struct command));
        command_node->command->u.word = (char **) checked_malloc(SIZE*sizeof(char*));
        word = (char *) checked_malloc(SIZE*sizeof(char));
        word_index = 0;
        i+=more;
        continue;
      }
      //if not new command tree
      else
      {
        word = (char *) checked_malloc(SIZE*sizeof(char));
        word_index = 0;
      }
    }
    // if next char is an operator
    else if ( (char_buffer[i] == '&' && char_buffer[i+1] == '&') ||
               char_buffer[i] == '|' || char_buffer[i] == ';' )
    {
      // set end of line to null byte

      if (char_buffer[i-1] != ' ')
      {
        word[word_index] = '\0';
        //set command_node's word equal to word 
        command_node->command->u.word[command_word_index++] = word;
      }
      
      
      // terminate word with null byte
      command_node->command->u.word[command_word_index] = '\0';

      command_node->command->type = SIMPLE_COMMAND;
     
      // TODO: IMPLEMENT STACK FUNCTIONALITY HERE. 
      
      CommandStackPush(&command_stack, command_node->command);
      // create new command
      command_node->command = (struct command *)checked_malloc(sizeof(struct command));
      command_node->command->u.word = (char **) checked_malloc(SIZE*sizeof(char*));
      word = (char *) checked_malloc(SIZE*sizeof(char));
      word_index = 0;
      command_word_index = 0;
      op_struct = (struct operator *) checked_malloc(sizeof(struct operator));
      op_struct->op_type = (enum command_type *) checked_malloc(sizeof(enum command_type));

      if ( (char_buffer[i] == '&' && char_buffer[i + 1] == '&') ||
           (char_buffer[i] == '|' && char_buffer[i + 1] == '|') )
      {
        if (char_buffer[i] == '&')
        {
          *(op_struct->op_type) = AND_COMMAND;
          op_struct->value = 1;
        }
        else
        {
          *(op_struct->op_type) = OR_COMMAND;
          op_struct->value = 1;
        }
        i+=2;
        
        // check for more whitespace
        while(1)
        {
          if (char_buffer[i] == ' ' || char_buffer[i] == '\n') { i++; }
          else { break; }
        }
      }
      else if ( (char_buffer[i] == '|' && char_buffer[i + 1] != '|') ||
                 char_buffer[i] == ';' )
      {
        if (char_buffer[i] == '|')
        {
          *(op_struct->op_type) = PIPE_COMMAND;
          op_struct->value = 2;
        }
        else
        {
          *(op_struct->op_type) = SEQUENCE_COMMAND;
          op_struct->value = 0;
        }
        i++;
        
        // check for more whitespace
        while(1)
        {
          if (char_buffer[i] == ' ' || char_buffer[i] == '\n') { i++; }
          else { break; }
        }
      }

      if ( OpStackIsEmpty(&op_stack) )
      {
        OpStackPush(&op_stack, op_struct);
        continue;
      }
      else
      {
        while( !OpStackIsEmpty(&op_stack) && op_struct->value <= OpStackTop(&op_stack)->value ) 
        {
          
          command_node->command->type = *(OpStackPop(&op_stack)->op_type); 
          /*if (count == 0) 
          {
            command_node->command->u.command[1] = CommandStackPop(&command_stack);
            command_node->command->u.command[0] = CommandStackPop(&command_stack);
            count++;
          }*/
          
          command_node->command->u.command[1] = CommandStackPop(&command_stack);
          command_node->command->u.command[0] = CommandStackPop(&command_stack);
          
          CommandStackPush(&command_stack, command_node->command); 
          command_node->command = (struct command *)checked_malloc(sizeof(struct command));  
        }

        // create new command
        command_node->command->u.word = (char **) checked_malloc(SIZE*sizeof(char*));
        OpStackPush(&op_stack, op_struct);
        continue;
      }
    }

    // if the line is a comment starting with #, ignore the whole line
    else if (char_buffer[i] == '#')
    {
      i++;
      while(1)
      {
        if (char_buffer[i] == '\n') { break; }
        else { i++; }
      }

      // Check for more newlines, whitespaces, or EOF
      int eof = 0;
      while (1)
      {
        if (char_buffer[i] == '\n' || char_buffer[i] == ' ') { i++; }          
        else if (char_buffer[i] == EOF) { eof = 1; break; }
        else { break; }
      }
      if (eof) { break; }
      else { continue; }
    }

    // if not a space or newline, add character to word char array
    else
    { 
      word[word_index++] = char_buffer[i]; 
    }

    i++;
  }

  return command_stream;
}

command_t
read_command_stream (command_stream_t s)
{
  /* FIXME: Replace this with your implementation too.  */

  if (s->cursor == NULL) {return NULL;}
  struct commandNode * temp = s->cursor;
  s->cursor = s->cursor->next;
  return temp->command;
}

char * create_char_buffer (int (*get_next_byte) (void *),
         void *get_next_byte_argument, int * index)
{
  size_t size = SIZE;
  char * buffer = (char *) checked_malloc(sizeof(char) * size);
  char ch;

  while (1)
  {
    ch = get_next_byte(get_next_byte_argument);
    if (ch == EOF)
    {
      buffer[(*index)++] = '\n';
      buffer[(*index)++] = '\n';
      buffer[(*index)] = EOF;
      break;
    }

    if ((size_t)(*index) == size)
    {
      size *= sizeof(char);
      buffer = (char *)checked_grow_alloc(buffer, &size);
    }

    buffer[(*index)++] = ch;
  }
  return buffer;
}

