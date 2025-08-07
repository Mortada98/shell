/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfatih <helfatih@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 09:35:30 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/04 09:57:08 by helfatih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	my_server(int ig)
{
	if (ig == SIGINT)
	{
		close(0);
		g_value = SIGINT;
		write(1, "^C", 2);
	}
}

void	read_and_convert(char *buffer, int *fd, unsigned char *c, int *i)
{
	if (read(*fd, c, 1) != 1)
	{
		close(*fd);
		return ;
	}
	buffer[(*i)++] = 'a' + (*c % 26);
}

int	herdoc_condition_2(t_command **cmd, t_data **data)
{
	(void)data;
	if (g_value == SIGINT)
	{
		(*cmd)->file = true;
		set_status(130);
		return (0);
	}
	return (1);
}

void	herdoc_condition_1(t_command **cmd, t_data **data, char *join)
{
	if ((*data)->start == (*data)->count_herdoc - 1)
		(*cmd)->herdoc_file = gc_strdup(join);
	else
		unlink(join);
}
