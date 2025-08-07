/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouizak <mbouizak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:01:25 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/07 14:38:04 by mbouizak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	validation(t_command *cmd)
{
	if (cmd->args[1] && !is_number(cmd->args[1]))
	{
		write(2, "minishell: exit: ", 17);
		write(2, cmd->args[1], ft_strlen(cmd->args[1]));
		write(2, ": numeric argument required\n", 28);
		return (2);
	}
	return (0);
}

int	make_exit(t_command *cmd)
{
	int	status;
	int	i;

	status = 0;
	i = 0;
	if (cmd->args[1] != NULL)
	{
		i = validation(cmd);
		if (i != 0)
		return (i);
		status = ft_atoi(cmd->args[1]) % 256;
		if (status < 0)
		status += 256;
		return (status);
	}
	return (0);
}

void	my_exit(t_command **cmd, t_data *data, int *error)
{
	int	i;
	
	(void)data;
	if (ft_strcmp((*cmd)->args[0], "exit") == 0)
	{
		if ((*cmd)->args[2] && (*cmd)->args[1] && is_number((*cmd)->args[1]))
		{
			write(2, "minishell: exit: too many arguments\n", 36);
			set_status(1);
			*error = 0;
			return ;
		}
		write(1, "exit\n", 5);
		i = make_exit(*cmd);
		if (i == 1)
		{
			set_status(1);
			*error = 0;
			return ;
		}
		if (i == 0)
		{
			set_status(get_status());
			*error = 1;
			return ;
		}
		set_status(i);
		*error = 1;
		return ;
	}
}

void	my_exit_child(t_command **cmd, t_data *data, int *error)
{
	int	i;

	(void)data;
	if (ft_strcmp((*cmd)->args[0], "exit") == 0)
	{
		if ((*cmd)->args[2])
		{
			write(2, "minishell: exit: too many arguments\n", 36);
			set_status(1);
			*error = 0;
			return ;
		}
		i = make_exit(*cmd);
		if (i == 1)
		{
			set_status(1);
			*error = 0;
			return ;
		}
		if (i == 0)
		{
			set_status(get_status());
			*error = 1;
			return ;
		}
		set_status(i);
		*error = 1;
		return ;
	}
}
