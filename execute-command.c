// UCLA CS 111 Lab 1 command execution

#include "command.h"
#include "command-internals.h"

#include <unistd.h>
#include <error.h>
#include <stdlib.h>
#include <sys/wait.h>

/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */

void execute_simple(command_t c);
void execute_non_redir(command_t c, bool or_command);

int
command_status (command_t c)
{
  return c->status;
}

void
execute_command (command_t c, bool time_travel)
{
  /* FIXME: Replace this with your implementation.  You may need to
     add auxiliary functions and otherwise modify the source code.
     You can also use external functions defined in the GNU C Library.  */
 	
 	switch (c->type)
 	{
 		case SIMPLE_COMMAND:
 		{
 			execute_simple(c);
 			break;
 		}
 		case AND_COMMAND:
 		case OR_COMMAND:
 		case SEQUENCE_COMMAND:
 		{
			execute_non_redir(c, c->type == OR_COMMAND);
			break;
 		}
 		default:
 		{
 			abort();
 		}

 	}
}

void
execute_simple(command_t c)
{
	char ** cmd = c->u.word;
	execvp(cmd[0], cmd);
	exit(127);								// if command FAILS, manually exit 127
}

void
execute_non_redir(command_t c, bool or_command)
{
	int p = fork();
	if (p == 0)
	{
		execute_command(c->u.command[0], 0);
	}
	else
	{
		int status;								// stores not only the exit status, but a lot of other information 											  too.
												// need to extract bits for exit status, LEAST SIGNIFICANT 8 BITS
		waitpid(p, &status, 0);					// blocking approach
		int exit_status = WEXITSTATUS(status);	// gets exit status from status
		if ( (exit_status == 1 && or_command) ||
			 (exit_status == 0 && !or_command) )
		{
			execute_command(c->u.command[1], 0);
		}
	}
}