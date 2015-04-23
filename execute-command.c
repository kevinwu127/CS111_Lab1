// UCLA CS 111 Lab 1 command execution

#include "command.h"
#include "command-internals.h"

#include <unistd.h>
#include <error.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>

void execute_simple(command_t c);
void execute_non_redir(command_t c, enum command_type type);
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
			execute_non_redir(c, c->type);
			break;
 		}
 		case PIPE_COMMAND:
 		{
 			execute_pipe(c);
 			break;
 		}
 		case SUBSHELL_COMMAND:
 		{
 			execute_command(c->u.subshell_command, 0);
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
	int p = fork();
	if (p == 0)
	{
		char ** cmd = c->u.word;
		execvp(cmd[0], cmd);
		exit(127);								// if command FAILS, manually exit 127
	}
	else
	{
		int status;
		waitpid(p, &status, 0);
		int exit_status = WEXITSTATUS(status);
		c->status = exit_status;
		exit(exit_status);
	}
}

void
execute_non_redir(command_t c, enum command_type type)
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
		if ( (exit_status == 1 && type != AND_COMMAND) ||
			 (exit_status == 0 && type != OR_COMMAND) )
		{
			int secp = fork();
			if (secp == 0)
			{
				execute_command(c->u.command[1], 0);
			}
			else
			{
				int sec_status;
				waitpid(secp, &sec_status, 0);
				int sec_exit_status = WEXITSTATUS(sec_status);
				c->status = sec_exit_status;
				exit(sec_exit_status);
			}
		}
		else if ( (exit_status == 1 && type == AND_COMMAND) ||
				  (exit_status == 0 && type == OR_COMMAND) )
		{
			c->status = exit_status;
			exit(exit_status);
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