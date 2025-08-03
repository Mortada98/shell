/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouizak <mbouizak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 17:26:13 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/03 10:03:44 by mbouizak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int	is_number(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '\0')
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

// void	core_dumped(int *pids, int pid_count, t_data **data)
// {
// 	int	status;
// 	int	sig;

// 	sig = 0;
// 	if (pid_count == 0)
// 		return ;
// 	for (int i = 0; i < pid_count; i++)
// 	{
// 		waitpid(pids[i], &status, 0);
// 		if (WIFSIGNALED(status))
// 		{
// 			sig = WTERMSIG(status);
// 			if (sig == SIGINT)
// 				write(1, "\n", 1);
// 			else if (sig == SIGQUIT)
// 				write(2, "Quit (core dumped)\n", 19);
// 			set_status(128 + sig);
// 			(*data)->exit = 128 + WTERMSIG(status);
// 		}
// 		else if (i == pid_count - 1)
// 		{
// 			set_status(WEXITSTATUS(status));
// 		}
// 	}
// }

void	handle_core_dumped(int *pids, int pid_count, t_data **data)
{
	int sig;
	int status;
	if (pid_count == 0)
		return;

	sig = 0;
	signal(SIGINT, SIG_IGN);
	for (int i = 0; i < pid_count; i++)
	{
		waitpid(pids[i], &status, 0);
		if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			if (sig == SIGINT)
				write(1, "\n", 1);
			else if (sig == SIGQUIT)
				write(2, "Quit (core dumped)\n", 19);
			set_status(128 + sig);
			(*data)->exit = 128 + WTERMSIG(status);
		}
		else if (i == pid_count - 1)
		{
			set_status(WEXITSTATUS(status));
		}
	}
}

bool	empty_command(t_command *cmd)
{
	t_command *temp_cmd;
	bool has_command;

	has_command = false;
	temp_cmd = cmd;
	if (!temp_cmd)
		return (true);
	while (temp_cmd)
	{
		if (temp_cmd->args && temp_cmd->args[0][0] == '\0')
		{
			write(2, "command not found\n", 19);
		}
		if (temp_cmd->args && temp_cmd->args[0] && temp_cmd->args[0][0] != '\0')
		{
			has_command = true;
		}
		temp_cmd = temp_cmd->next;
	}
	return (has_command);
}

void	execute_command(t_command *cmd, char ***env, t_data **data)
{
	int			prev_fd;
	int			fd[2];
	pid_t		pid;
	int			fd_out;
	int			fd_in;
	t_command	*curr;
	// int			status;
	int			saved_stdin;
	pid_t		pids[1024];
	int			pid_count;
	// t_command	*temp_cmd;
	int			save;
	int			hd_fd;
	char		*command;
	// int			sig;
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
					printf("minishell: No such file or directory\n");
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
					int i = 0;
					while((*env)[i])
					{
						free((*env)[i]);
						i++;
					}
					free(*env);
					rl_clear_history();
					exit(get_status());
				}
				command = get_command(curr->args[0], *env);
				if (!command)
				{
					close(saved_stdin);
					close(save);
					write(2, "minishell: ", 11);
                    write(2, curr->args[0], ft_strlen(curr->args[0]));
                    write(2, ": command not found\n", 20);
					gc_cleanup();
					set_status(127);
					// int i = 0;
					// while((*env)[i])
					// {
					// 	free((*env)[i]);
					// 	i++;
					// }
					// free(*env);
					exit(127);
				}
				if (execve(command, curr->args, *env) != 0)
				{
					close(saved_stdin);
					close(save);
					gc_cleanup();
					perror("execve");
					int i = 0;
					while((*env)[i])
					{
						free((*env)[i]);
						i++;
					}
					free(*env);
					exit(1);
				}
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

// bool	logic_of_meta(t_token *cmd, t_data **data)
// {
// 	t_token	*cur;

// 	(void)data;
// 	cur = cmd;
// 	while (cur && cur->next)
// 	{
// 		if (cur->type != TOKEN_PIPE && cur->type != TOKEN_WORD
// 			&& cur->next->type != TOKEN_PIPE && cur->next->type != TOKEN_WORD)
// 		{
// 			printf("minishell: syntax error  near unexpected token %s\n",
// 				cur->next->av);
// 			set_status(2);
// 			return (false);
// 		}
// 		else
// 			cur = cur->next;
// 	}
// 	return (true);
// }



void	my_handler(int sig)
{
	// rl_catch_signals = 0;
	if (sig == SIGINT)
	{
		write(1, "^C\n", 3);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		set_status(130);
	}
}

void	make_prompt(char ***env)
{
	char		*line;
	t_token		*token;
	t_command	*cmd;
	t_data		*data;
	t_command	*temp_cmd;
	bool		heredoc_interrupted;
	int			herdoc_count;
	t_command	*check_cmd;
	bool		has_command;

	signal(SIGINT, my_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		data = gc_malloc(sizeof(t_data));
		if (!data)
		{
			printf("minishell: memory allocation failed\n");
			return ;
		}
		ft_memset(data, 0, sizeof(t_data));
		line = readline(prompt(*env));
		if (!line)
		{
			printf("exit\n");
			gc_cleanup();
			//gc_cleanup_partial();
			// free(data);
			break;
		}
		gc_register_external(line);
		if (line[0] == '\0')
		{
			continue ;
		}
		if (line[0] != '\0')
		{
			add_history(line);
			token = tokenize(line, &data, *env);
			if (!token)
			{
				continue ;
			}
			if (data->flags)
			{
				continue;
			}
			join_nodes(&token);
			cmd = parsing_command(token, &data);
			if (!cmd)
				continue ;
			temp_cmd = cmd;
			heredoc_interrupted = false;
			while (temp_cmd && !heredoc_interrupted)
			{
				if (temp_cmd->herdoc && temp_cmd->herdoc[0])
				{
					herdoc_count = 0;
					while (temp_cmd->herdoc[herdoc_count])
						herdoc_count++;
					data->count_herdoc = herdoc_count;
					excute_herdoc_for_child(&temp_cmd, &data, *env);
					if (g_value == SIGINT)
					{
						heredoc_interrupted = true;
						break ;
					}
				}
				temp_cmd = temp_cmd->next;
			}
			if (heredoc_interrupted)
			{
				g_value = 0;
				gc_cleanup_partial();
				continue ;
			}
			g_value = 0;
			if (cmd)
			{
				check_cmd = cmd;
				has_command = false;
				while (check_cmd)
				{
					if (check_cmd->args && check_cmd->args[0])
					{
						has_command = true;
						break;
					}
					check_cmd = check_cmd->next;
				}
				if (cmd)
				{
					execute_command(cmd, env, &data);
				}
				else if (cmd->herdoc_file)
				{
					unlink(cmd->herdoc_file);
				}
			}
			gc_cleanup_partial();
		}
	}
	gc_cleanup();
	int i = 0;
	while((*env)[i])
	{
		free((*env)[i]);
		i++;
	}
	free(*env);
	// free(data);
	rl_clear_history();
}

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	rl_catch_signals = 0;
	char **my_env;
	my_env = copy_env(env);
	make_prompt(&my_env);
}
