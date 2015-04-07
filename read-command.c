// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"
#include "alloc.h"
#include "stack.h"

#include <error.h>
#include <stdio.h>

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
} *command_stream;

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  /* FIXME: Replace this with your implementation.  You may need to
     add auxiliary functions and otherwise modify the source code.
     You can also use external functions defined in the GNU C Library.  */
  char ch;
  char * command_buffer = (char *) checked_malloc(SIZE * sizeof(char));
  char * operand_buffer = (char *) checked_malloc(2 * sizeof(char));
  commandStack command_stack;
  opStack op_stack;
  CommandStackInit(&command_stack, SIZE);
  OpStackInit(&op_stack, SIZE);

  int index = 0;

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

    

  }

 
}

command_t
read_command_stream (command_stream_t s)
{
  /* FIXME: Replace this with your implementation too.  */

  /*if (s->cursor == NULL) {return NULL;}
  struct commandNode * temp = s->cursor;
  s->cursor = s->cursor->next;
  return temp;*/

  error (1, 0, "command reading not yet implemented");
  return 0;
}

