// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"
#include "alloc.h"

#include <error.h>

/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */
#define SIZE 1024

/* FIXME: Define the type 'struct command_stream' here.  This should
   complete the incomplete type declaration in command.h.  */
typedef struct command_stream
{
  char ** commands;
};

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  /* FIXME: Replace this with your implementation.  You may need to
     add auxiliary functions and otherwise modify the source code.
     You can also use external functions defined in the GNU C Library.  */
  char * stream = (char *)checked_malloc(SIZE * sizeof(char));

  if (command_stream_t->stream == NULL)
    error(1, 0, "No memory allocated");

  int c;
  int index = 0;
  int size = SIZE;
  while( (c = get_next_byte(get_next_byte_argument)) != EOF )
  {
    if (index == size)
    {
      command_stream_t->stream = (char *)checked_grow_alloc(command_stream_t->stream, &size);

      if (command_stream_t->stream == NULL)
        error(1, 0, "No memory reallocated");

      size *= 2;
    }

    stream[index] = (char) c;
    index++;
  }

  if (index == 0)
}

command_t
read_command_stream (command_stream_t s)
{
  /* FIXME: Replace this with your implementation too.  */
  error (1, 0, "command reading not yet implemented");
  return 0;
}
