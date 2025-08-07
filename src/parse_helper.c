/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfatih <helfatih@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 11:55:44 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/05 15:33:37 by helfatih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND);
}

int	parse_redirections(t_command **current_cmd, t_token **current,
		t_data **data)
{
	if ((*current)->type == TOKEN_REDIR_IN)
	{
		if (!handle_redir_in(current, *current_cmd, data))
			return (0);
	}
	else if ((*current)->type == TOKEN_REDIR_OUT)
	{
		if (!handle_redir_out(current, *current_cmd))
			return (0);
	}
	else if ((*current)->type == TOKEN_REDIR_APPEND)
	{
		if (!handle_redir_append(current, *current_cmd))
			return (0);
	}
	return (1);
}

int	word_or_herdoc(t_token **current, t_command *current_cmd, t_data **data,
		int i)
{
	if ((*current)->type == TOKEN_HERDOC)
	{
		if (!handle_heredoc(current, current_cmd, &i))
			return (0);
	}
	else if ((*current)->type == TOKEN_WORD)
	{
		append_arg(current_cmd, (*current)->av, data);
		*current = (*current)->next;
	}
	return (1);
}

int	reset_value(t_command **current_cmd, t_token **current, t_data **data,
		t_token *token)
{
	reset_redir_error(0);
	*current = token;
	*current_cmd = create_command();
	if (!*current_cmd)
		return (0);
	(*data)->count_red_in = 0;
	return (1);
}

int	another_function(t_token **current, t_command **current_cmd, t_data **data,
		int *i)
{
	if (is_redirection((*current)->type))
	{
		if (!parse_redirections(current_cmd, current, data))
			return (0);
	}
	else if ((*current)->type == TOKEN_HERDOC || (*current)->type == TOKEN_WORD)
	{
		if (!word_or_herdoc(current, *current_cmd, data, *i))
			return (0);
	}
	return (1);
}
