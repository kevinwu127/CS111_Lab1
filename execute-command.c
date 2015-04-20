// UCLA CS 111 Lab 1 command execution

#include "command.h"
#include "command-internals.h"

#include <unistd.h>
#include <error.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */

void execute_simple(command_t c);
void execute_non_redir(command_t c, bool or_command);
void handle_io(command_t c);
void execute_pipe(command_t c);

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
 	
    
    handle_io(c);

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
 		case PIPE_COMMAND:
 		{
 			execute_pipe(c);
 			break;
 		}
 		default:
 		{
 			abort();
 		}

 	}
}

void
handle_io(command_t c)
{
	if (c->input)
	{
		int fd = open(c->input, O_RDONLY, 0644);
		if (fd < 0) return;
		dup2(fd, 0);
	}

	if (c->output)
	{
		int fd = open(c->output, O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if (fd < 0) return;
		dup2(fd, 1);
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

void
execute_pipe(command_t c)
{
	int fd[2];
	pipe(fd);
	int first_pid = fork();
	if (first_pid == 0)
	{
		close(fd[1]);
		dup2(fd[0], 0);
		execute_command(c->u.command[1], 0);
	}
	else
	{
		int second_pid = fork();
		if (second_pid == 0)
		{
			close(fd[0]);
			dup2(fd[1], 1);
			execute_command(c->u.command[0], 0);
		}
		else
		{
			close(fd[0]);
			close(fd[1]);
			int status;
			int returned_pid = waitpid(-1, &status, 0);
			if (returned_pid == second_pid)
			{
				waitpid(first_pid, &status, 0);
				int exit_status = WEXITSTATUS(status);
				c->u.command[0]->status = exit_status;
			}
			if (returned_pid == first_pid)
			{
				waitpid(second_pid, &status, 0);
				int exit_status = WEXITSTATUS(status);
				c->u.command[1]->status = exit_status;
			}
		}
	}
}