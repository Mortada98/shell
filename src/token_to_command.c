/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_to_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfatih <helfatih@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:02:21 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/05 16:11:28 by helfatih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_pipe_token(t_parse *var, t_data **data)
{
	var->i = 0;
	(*data)->count_red_in = 0;
	if (!handle_pipe(&var->current, &var->current_cmd, var->first_cmd, data))
		return (0);
	return (1);
}

int	handle_redir_in_token(t_parse *var, t_data **data)
{
	if (!handle_redir_in(&var->current, var->current_cmd, data))
		return (0);
	return (1);
}

int	handle_redir_out_token(t_parse *var)
{
	if (!handle_redir_out(&var->current, var->current_cmd))
		return (0);
	return (1);
}

int	handle_redir_append_token(t_parse *var)
{
	if (!handle_redir_append(&var->current, var->current_cmd))
		return (0);
	return (1);
}

int	handle_heredoc_token(t_parse *var)
{
	if (!handle_heredoc(&var->current, var->current_cmd, &var->i))
		return (0);
	return (1);
}
