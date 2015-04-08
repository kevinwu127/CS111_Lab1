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
  char * operand_buffer = (char *) checked_malloc(2 * sizeof(char));
  commandStack command_stack;
  opStack op_stack;
  CommandStackInit(&command_stack, SIZE);
  OpStackInit(&op_stack, SIZE);
  struct commandNode * command_node = (struct commandNode *)checked_malloc(sizeof(struct commandNode));

  command_stream->head = command_node;
  command_stream->tail = command_node;
  command_stream->cursor = command_node;
  
  int command_word_index = 0;

  int i = 0;
  
  // creating first command
  command_node->command = (struct command *) checked_malloc(sizeof(struct command));
  command_node->command->u.word = (char **) checked_malloc(SIZE*sizeof(char*));
  char * word = (char *) checked_malloc(SIZE*sizeof(char));
  int word_index = 0;

  //until buffer reaches EOF
  while(char_buffer[i] != EOF)
  { 
    // if buffer reaches space or new line
    if(char_buffer[i] == ' ' || char_buffer[i] == '\n')   
    {
      // set end of line to null byte
      word[word_index] = '\0';
      
      //set command_node's word equal to word 
      command_node->command->u.word[command_word_index++] = word;

      // if there are two new lines at end of buffer, make new command tree
      if (char_buffer[i + 1] == '\n')
      {
        // terminate word with null byte
        command_node->command->u.word[command_word_index] = '\0';
        command_node->command->type = SIMPLE_COMMAND;
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
        i+=2;
        continue;
      }
      //if not new command tree
      else
      {
        word = (char *) checked_malloc(SIZE*sizeof(char));
        word_index = 0;
      }
    }

    else if ( (char_buffer[i] == '&' && char_buffer[i+1] == '&') ||
               char_buffer[i] == '|' || char_buffer[i] == ';' )
    {
      // set end of line to null byte
      word[word_index] = '\0';
      
      //set command_node's word equal to word 
      command_node->command->u.word[command_word_index++] = word;
      // terminate word with null byte
      command_node->command->u.word[command_word_index] = '\0';
      command_node->command->type = SIMPLE_COMMAND;
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
      i+=2;
      continue;

    }
    // if not a space or newline, add character to word char array
    else
    {
      word[word_index++] = char_buffer[i];
    }
    i++;
    
  }

  command_stream->tail->command->type = SIMPLE_COMMAND;


return command_stream;

printf("%d", command_word_index);

struct commandNode * it;
int j;

for(it = command_stream->head; it != NULL; it = it->next)
{
  for(i = 0; it->command->u.word[i] != NULL; i++)
  {
    for(j = 0; it->command->u.word[i][j] != '\0'; j++) 
    {

      printf("%c", it->command->u.word[i][j]);
    }
  }
  printf("\n");
}


  

/*
  // populate array with the file data
  while(1)
  {
    
    // get the next character
    ch = get_next_byte(get_next_byte_argument);
    if (ch == EOF)
      break;

    if (ch == '&' || ch == '|')
    {
      char next = get_next_byte(get_next_byte_argument);

      if (next == '&' && ch =='&')        // &&
      {
          operand_buffer[0] = '&';
          operand_buffer[1] = '&';
          printf("%c", operand_buffer[0]);
      }
      else if (next != '&' && ch == '&')  // &
        error(1,0,"wrong operand");

      if (next == '|' && ch == '|')       // || 
      {
        operand_buffer[0] = '|';
        operand_buffer[1] = '|';
        printf("%c", operand_buffer[0]);
      }
      else if (next != '|' && ch == '|')  // |
      {
        operand_buffer[0] = '|';
        operand_buffer[1] = '\0';
        command_buffer[index] = next;
        index++;
      }
    }

    

  }*/

 
}

command_t
read_command_stream (command_stream_t s)
{
  /* FIXME: Replace this with your implementation too.  */

  if (s->cursor == NULL) {return NULL;}
  struct commandNode * temp = s->cursor;
  s->cursor = s->cursor->next;
  return temp->command;

  //error (1, 0, "command reading not yet implemented");
  //return 0;
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

