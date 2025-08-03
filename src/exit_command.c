/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouizak <mbouizak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:01:25 by helfatih          #+#    #+#             */
/*   Updated: 2025/07/30 17:53:12 by mbouizak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	validation(t_command *cmd)
{
	if (cmd->args[1] && !is_number(cmd->args[1]))
	{
		printf("minishell: exit: %s: numeric argument required\n",
			cmd->args[1]);
		return (2);
	}
	if (cmd->args[2])
	{
		if (!is_number(cmd->args[2]))
		{
			printf("minishell: exit: too many arguments\n");
			return (2);
		}
		else
		{
			printf("minishell: exit: too many arguments\n");
			return (1);
		}
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
		printf("exit\n");
		i = make_exit(*cmd);
		if (i == 0)
		{
			set_status(get_status());
			*error = 1;
			return ;
			// exit(get_status());
		}
		set_status(i);
		*error = 1;
		return ;
		// exit(i);
	}
}

void	my_exit_child(t_command **cmd, t_data *data, int	*error)
{
	int i;

	(void)data;
	if (ft_strcmp((*cmd)->args[0], "exit") == 0)
	{
		i = make_exit(*cmd);
		if (i == 0)
		{
			set_status(get_status());
			*error = 1;
			return;
		}
		set_status(i);
		*error = 1;
		return;
	}
}