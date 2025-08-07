/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfatih <helfatih@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:48:08 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/06 09:50:42 by helfatih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	execute_command(t_command *cmd, char ***env, t_data **data)
{
	int			prev_fd;
	int			fd[2];
	pid_t		pid;
	int			fd_out;
	int			fd_in;
	t_command	*curr;
	int			saved_stdin;
	pid_t		pids[1024];
	int			pid_count;
	int			save;
	int			hd_fd;
	char		*command;
	bool		has_command;

	fd_out = -1;
	prev_fd = -1;
	fd_out = -1, fd_in = -1;
	curr = cmd;
	saved_stdin = dup(0);
	pid_count = 0;
	if (!cmd)
	{
		close(saved_stdin);
		return ;
	}
	if (cmd && cmd->next == NULL && cmd->args && cmd->args[0]
		&& built_in(cmd->args[0]))
	{
    if (!access_file(cmd))
      return;
    // Check if this command has redirection errors - if so, don't execute
		if (cmd->redir_error)
		{
			dup2(saved_stdin, 0);
			close(saved_stdin);
			set_status(1); // Set error status
			return ;
		}
		excute_redirection_of_parent(&cmd, &fd_out, *data, &saved_stdin, env);
		dup2(saved_stdin, 0);
		close(saved_stdin);
		return ;
	}
	
	// Check if we have any valid commands before entering the pipeline
	has_command = empty_command(cmd);
	
	// Even if there's no command, we need to process redirections (like bash does)
	if (!has_command)
	{
		// Check if there are any redirections to process
		t_command *temp_cmd = cmd;
		bool has_redirections = false;
		
		while (temp_cmd)
		{
			if (temp_cmd->file_input || temp_cmd->file_output || temp_cmd->herdoc)
			{
				has_redirections = true;
				break;
			}
			temp_cmd = temp_cmd->next;
		}
		
		if (has_redirections)
		{
			// Fork a child to process redirections
			pid_t redir_pid = fork();
			if (redir_pid == 0)
			{
				// Child process: try to open redirections
				excute_redirection_of_child(&cmd, data, &fd_out, &fd_in);
				if(*env[0])
					free_2d_array(*env);
				gc_cleanup();
				close(saved_stdin);
				exit(0); // Exit after processing redirections
			}
			else if (redir_pid > 0)
			{
				// Parent: wait for child and get status
				int status;
				waitpid(redir_pid, &status, 0);
				if (WIFEXITED(status))
					set_status(WEXITSTATUS(status));
			}
		}
		
		dup2(saved_stdin, 0);
		close(saved_stdin);
		return;
	}

	while (curr && !(cmd)->file)
	{
		save = dup(0);
		if (curr->next)
		{
			if (pipe(fd) == -1)
			{
				perror("pipe");
				set_status(1);
				close(save);
				dup2(saved_stdin, 0);
				close(saved_stdin);
				return ;
			}
		}
		pid = fork();
		if (pid == 0)
		{
			if ((cmd)->file)
			{
				(cmd)->file = false;
				set_status(130);
				exit(130);
			}
			signal(SIGQUIT, SIG_DFL);
			signal(SIGINT, SIG_DFL);
			if (prev_fd != -1)
			{
				dup2(prev_fd, 0);
				close(prev_fd);
			}
			if (!(curr->file) && curr->herdoc_file)
			{
				hd_fd = open(curr->herdoc_file, O_RDONLY);
				if (hd_fd < 0)
				{
					write(2, "minishell: No such file or directory\n", 36);
					exit(1);
				}
				dup2(hd_fd, 0);
				close(hd_fd);
			}
			if (curr->next)
			{
				close(fd[0]);
				dup2(fd[1], 1);
				close(fd[1]);
			}
			excute_redirection_of_child(&curr, data, &fd_out, &fd_in);
			// Check if this command has redirection errors - if so, don't execute
			if (curr->redir_error)
			{
				close(save);
				close(saved_stdin);
				exit(1); // Exit child process with error status
			}
			if (curr->args && curr->args[0] && curr->args[0][0] != '\0')
			{
				if (built_in(curr->args[0]))
				{
					excute_redirection_of_child_builtin(&curr, &fd_out, *data, &saved_stdin, &save, env);
					close(save);
					close(saved_stdin);
					gc_cleanup();
					if ((*env)[0])
					{
						int i = 0;
						while((*env)[i])
						{
							free((*env)[i]);
							i++;
						}
						free(*env);
					}
					rl_clear_history();
					exit(get_status());
				}
				command = get_command(curr->args[0], *env);
				if (!command)
				{
					close(saved_stdin);
					close(save);
					gc_cleanup();
					if ((*env)[0])
					{
						int i = 0;
						while((*env)[i])
						{
							free((*env)[i]);
							i++;
						}
						free(*env);
					}
					exit(get_status());
				}
				if (execve(command, curr->args, *env) != 0)
				{
					close(saved_stdin);
					close(save);
					gc_cleanup();
					perror("execve");
					if ((*env)[0])
					{
						int i = 0;
						while((*env)[i])
						{
							free((*env)[i]);
							i++;
						}
						free(*env);
					}
					exit(1);
				}
				gc_cleanup();
				exit(1);
			}
			else
			{
				exit(0);
			}
		}
		else if (pid > 0)
		{
			pids[pid_count++] = pid;
			if (prev_fd != -1)
				close(prev_fd);
			if (curr->next)
			{
				close(fd[1]);
				prev_fd = fd[0];
			}
		}
		else
		{
			perror("fork");
			set_status(1);
			close(save);
			dup2(saved_stdin, 0);
			close(saved_stdin);
			return ;
		}
		dup2(save, 0);
		close(save);
		curr = curr->next;
	}
	handle_core_dumped(pids, pid_count, data);
	signal(SIGINT, my_handler);
	dup2(saved_stdin, 0);
	close(saved_stdin);
}
