/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfatih <helfatih@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 17:26:54 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/04 09:25:57 by helfatih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_token(t_token **token)
{
	t_token	*current;
	t_token	*next;

	current = *token;
	while (current)
	{
		next = current->next;
		current = next;
	}
	*token = NULL;
}

int	is_space(char c)
{
	if (c == ' ')
		return (0);
	return (1);
}

int	count_args(char **av)
{
	int	i;

	i = 0;
	while (av[i])
	{
		i++;
	}
	return (i);
}

void	append_arg(t_command *cmd, char *str, t_data **data)
{
	char	**new_array;
	int		i;
	int		j;

	if (!cmd || !str)
		return ;
	(*data)->exit = 0;
	i = 0;
	if (cmd->args)
		i = count_args(cmd->args);
	new_array = gc_calloc(i + 2, sizeof(char *));
	if (!new_array)
		return ;
	j = -1;
	while (++j < i)
		new_array[j] = cmd->args[j];
	new_array[i] = gc_strdup(str);
	if (!new_array[i])
		return ;
	new_array[i + 1] = NULL;
	cmd->args = new_array;
}
