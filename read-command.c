// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"
#include "alloc.h"

#include <error.h>
#include <stdio.h>

/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */
#define SIZE 1024

/* FIXME: Define the type 'struct command_stream' here.  This should
   complete the incomplete type declaration in command.h.  */
typedef struct command_stream
{
  char ** commands;
  char * stream;
  int num_commands;
  int stream_size;
} *command_stream_t;

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  /* FIXME: Replace this with your implementation.  You may need to
     add auxiliary functions and otherwise modify the source code.
     You can also use external functions defined in the GNU C Library.  */
  char c;
  int index = 0;
  int i;
  int count = 0;
  size_t size = SIZE;

  // allocate struct
  command_stream_t command_stream = checked_malloc(sizeof(command_stream));

  if (command_stream == NULL)
    error(1, 0, "struct not allocated");

  // allocate array for file stream
  command_stream->stream = (char *)checked_malloc(size * sizeof(char));

  if (command_stream->stream == NULL)
    error(1, 0, "No memory allocated");

  // populate array with the file data
  while(1)
  {
    // reallocate by size if the file data fills up array
    if ( (size_t)index == size )
    {
      command_stream->stream = (char *)checked_grow_alloc(command_stream->stream, &size);

      if (command_stream->stream == NULL)
        error(1, 0, "No memory reallocated");
    }

    c = get_next_byte(get_next_byte_argument);
    if (c == EOF)
      break;

    command_stream->stream[index] = c;
    index++;
  }

  // if it is an empty file, set an error
  if (index == 0)
    error(1,0,"Empty File");

  // add a newline to the end of the file if none
  if (command_stream->stream[index - 1] != '\n')
  {
    command_stream->stream[index] = '\n';
    index++;
  }

  // get number of commands from stream
  for (i = 0; i < index; i++)
  {
    if (command_stream->stream[i] == '\n')
      count++;
  }

  // create commands array
  command_stream->commands = (char **)checked_malloc((size_t)count * sizeof(char *));
  if (command_stream->commands == NULL)
    error(1,0,"Allocation error");

  // populate array with pointers to each command in stream array
  char * cptr = command_stream->stream;
  for (i = 0; i < count; i++)
  {
    command_stream->commands[i] = cptr;
    while (*cptr != '\n')
      cptr++;
    cptr++;
  }

  // print array for debugging
  for (i = 0; i < count; i++)
  {
    char * ch = command_stream->commands[i];
    while (*ch != '\n')
    {
      printf("%c", *ch);
      ch++;
    }
    printf("%c", *ch);
  }

  // save data into struct
  command_stream->num_commands = count;
  command_stream->stream_size = index;
  return command_stream;
}

command_t
read_command_stream (command_stream_t s)
{
  /* FIXME: Replace this with your implementation too.  */
  
    // TODO:
  // get commands index
  // if the first character is a newline character, go to the next command line
  // while the character is not a newline
  //  save characters into an array
  //  if the character is a >, < symbol
  //    save the characters to the right of the symbol to command->input or command->output respectively
  //  if the character is a symbol (&&, ||, |, etc.)
  //    save character array into command->u.command[0]
  //    set command->type to appropriate type based on symbol
  //    recursively call on the rest of the command
  //    save the second 'half' of the command into command->u.command[1]
  //  if the character is a ( symbol
  //    set command->type to SUBSHELL_COMMAND
  //    recursively call on the rest of the command
  //  if the character is a ) symbol
  //    return the command
  // exit loop
  // set command->type to SIMPLE_COMMAND
  // return the command

  error (1, 0, "command reading not yet implemented");
  return 0;
}
