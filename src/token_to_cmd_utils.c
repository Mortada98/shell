/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_to_cmd_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouizak <mbouizak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:06:17 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/07 11:38:16 by mbouizak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_word(t_parse *var, t_data **data)
{
	if (!var->current->found)
	{
		append_arg(var->current_cmd, var->current->av, data);
	}
	var->current = var->current->next;
	return (1);
}

int	process_current_token(t_parse *var, t_data **data)
{
	if (var->current->type == TOKEN_PIPE)
		return (handle_pipe_token(var, data));
	else if (var->current->type == TOKEN_REDIR_IN)
		return (handle_redir_in_token(var, data));
	else if (var->current->type == TOKEN_REDIR_OUT)
		return (handle_redir_out_token(var));
	else if (var->current->type == TOKEN_REDIR_APPEND)
		return (handle_redir_append_token(var));
	else if (var->current->type == TOKEN_HERDOC)
		return (handle_heredoc_token(var));
	else if (var->current->type == TOKEN_WORD)
	{
		return (handle_word(var, data));
	}
	return (1);
}
