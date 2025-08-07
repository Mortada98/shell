/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meta_char.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfatih <helfatih@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 20:10:38 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/05 16:32:31 by helfatih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_pipe(t_token **current, t_command **current_cmd,
		t_command *first_cmd, t_data **data)
{
	t_command	*new_cmd;

	(void)first_cmd;
	if ((*current)->prev == NULL)
	{
		write(2, "minishell: syntax error near unexpected token `|'\n", 54);
		set_status(2);
		return (0);
	}
	if ((*current)->next == NULL)
	{
		write(2, "minishell: syntax error near unexpected token `|'\n", 54);
		(*data)->exit = 2;
		return (0);
	}
	new_cmd = create_command();
	if (!new_cmd)
		return (0);
	(*current_cmd)->next = new_cmd;
	*current_cmd = new_cmd;
	*current = (*current)->next;
	(*data)->exit = 0;
	reset_redir_error(0);
	return (1);
}

int	handle_redir_in(t_token **current, t_command *cmd, t_data **data)
{
	if (!(*current)->next)
	{
		write(2, "minishell: syntax error near unexpected token `newline'\n",
			59);
		set_status(2);
		return (0);
	}
	if ((*current)->next->type != TOKEN_WORD)
	{
		write(2, "minishell: syntax error near unexpected token `", 46);
		write(2, (*current)->next->av, ft_strlen((*current)->next->av));
		write(2, "'\n", 2);
		set_status(2);
		return (0);
	}
	if (red_in_realloc(cmd, data, current) == 0)
		return (0);
	(*current) = (*current)->next->next;
	return (1);
}

int	handle_redir_out(t_token **current, t_command *cmd)
{
	char	*filename;

	if (!check_redir_syntax(current))
		return (0);
	if (get_redir_error())
	{
		(*current) = (*current)->next->next;
		return (1);
	}
	filename = gc_strdup((*current)->next->av);
	if (!filename)
		return (0);
	open_output_file_0(cmd, filename);
	cmd->append = 0;
	(*current) = (*current)->next->next;
	return (1);
}

int	handle_redir_append(t_token **current, t_command *cmd)
{
	char	*filename;

	if (!check_redir_syntax(current))
		return (0);
	if (get_redir_error())
	{
		(*current) = (*current)->next->next;
		return (1);
	}
	filename = gc_strdup((*current)->next->av);
	if (!filename)
		return (0);
	if (!open_output_file_1(cmd, filename))
		return (0);
	cmd->append = 1;
	(*current) = (*current)->next->next;
	return (1);
}

int	handle_heredoc(t_token **current, t_command *cmd, int *i)
{
	if (!(*current)->next)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		set_status(2);
		return (0);
	}
	if ((*current)->next->type != TOKEN_WORD)
	{
		write(2, "minishell: syntax error near unexpected token `", 46);
		write(2, (*current)->next->av, ft_strlen((*current)->next->av));
		write(2, "'\n", 2);
		set_status(2);
		return (0);
	}
	if (heredoc_realloc(i, cmd, current) == 0)
		return (0);
	(*current) = (*current)->next->next;
	return (1);
}
