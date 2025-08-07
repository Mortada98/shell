/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouizak <mbouizak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:54:31 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/07 14:12:43 by mbouizak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_number(char *str)
{
	int	i;

	if (!str)
		return (0);
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

void	handle_core_dumped(int *pids, int pid_count, t_data **data)
{
	int (i), status, sig;
	sig = 0;
	signal(SIGINT, SIG_IGN);
	i = -1;
	while (++i < pid_count)
	{
		waitpid(pids[i], &status, 0);
		if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			if (sig == SIGINT)
			{
				set_status(128 + sig);
				write(1, "\n", 1);
				break ;
			}
			else if (sig == SIGQUIT)
				write(2, "Quit (core dumped)\n", 19);
			set_status(128 + sig);
			(*data)->exit = 128 + WTERMSIG(status);
		}
		else if (i == pid_count - 1)
			set_status(WEXITSTATUS(status));
	}
}

bool	empty_command(t_command *cmd)
{
	t_command	*temp_cmd;
	bool		has_command;

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

int	access_file(t_command *cmd)
{
	int	j;
	int	fd;

	j = 0;
	if (cmd->file_input)
	{
		while (cmd->file_input[j])
		{
			fd = open(cmd->file_input[j], O_RDONLY);
			if (fd < 0)
			{
				write(2, "minishell: ", 11);
				write(2, cmd->file_input[j], ft_strlen(cmd->file_input[j]));
				write(2, ": No such file or directory\n", 28);
				return (0);
			}
			close(fd);
			j++;
		}
	}
	return (1);
}

void	my_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "^C\n", 3);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		set_status(130);
	}
}
