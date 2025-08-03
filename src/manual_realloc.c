/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manual_realloc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfatih <helfatih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:22:08 by helfatih          #+#    #+#             */
/*   Updated: 2025/07/28 16:31:16 by helfatih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	red_in_realloc(t_command *cmd, t_data **data, t_token **current)
{
	char	**new_file_input;
	int		j;

	new_file_input = gc_malloc(sizeof(char *) * ((*data)->count_red_in + 2));
	if (!new_file_input)
	{
		return (0);
	}
	if (cmd->file_input)
	{
		for (j = 0; j < (*data)->count_red_in; j++)
			new_file_input[j] = cmd->file_input[j];
	}
	cmd->file_input = new_file_input;
	cmd->file_input[(*data)->count_red_in] = gc_strdup((*current)->next->av);
	if (!cmd->file_input[(*data)->count_red_in])
	{
		return (0);
	}
	cmd->file_input[(*data)->count_red_in + 1] = NULL;
	(*data)->count_red_in++;
	return (1);
}

int	heredoc_realloc(int *i, t_command *cmd, t_token **current)
{
    char **new_herdoc;
    int j;
    
	new_herdoc = gc_malloc(sizeof(char *) * (*i + 2));
	if (!new_herdoc)
		return (0);
	if (cmd->herdoc)
	{
		for (j = 0; j < *i; j++)
			new_herdoc[j] = cmd->herdoc[j];
	}
	cmd->herdoc = new_herdoc;
	cmd->herdoc[(*i)] = gc_strdup((*current)->next->av);
	if (!cmd->herdoc[*i])
		return (0);
	cmd->herdoc[*i + 1] = NULL;
	(*i)++;
    return(1);
}
