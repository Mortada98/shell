/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouizak <mbouizak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:24:34 by helfatih          #+#    #+#             */
/*   Updated: 2025/07/30 20:57:44 by mbouizak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token_type	get_token_type(char *str)
{
	if (ft_strncmp(str, ">>", 2) == 0)
		return (TOKEN_REDIR_APPEND);
	else if (ft_strncmp(str, "<<", 2) == 0)
		return (TOKEN_HERDOC);
	else if (ft_strncmp(str, ">", 1) == 0)
		return (TOKEN_REDIR_OUT);
	else if (ft_strncmp(str, "<", 1) == 0)
		return (TOKEN_REDIR_IN);
	else if (ft_strncmp(str, "|", 1) == 0)
		return (TOKEN_PIPE);
	else
		return (TOKEN_WORD);
}

t_token	*tokenize(char *line, t_data **data, char **env)
{
	t_token	*token;

	token = NULL;
	(*data)->start = 0;
	(*data)->end = 0;
	if (!check_somthing(line))
		return (NULL);
	while (line[(*data)->end])
	{
		if (line[(*data)->end] == '$' && (ft_isalnum(line[(*data)->end + 1])
				|| line[(*data)->end + 1] == '_'))
		{
			handle_dollar(&token, line, data, env);
			continue ;
		}
		if (line[(*data)->end] == '$' && !ft_isalnum(line[(*data)->end + 1]))
		{
			handle_some_cases(&token, line, data, env);
		}
		if (line[(*data)->end] == '|' || line[(*data)->end] == '<'
			|| line[(*data)->end] == '>')
		{
			handle_word_token(&token, line, data, env);
			(*data)->end = handle_speciale_token(&token, line, (*data)->end,
					data);
			(*data)->start = (*data)->end;
		}
		else if (line[(*data)->end] == '\"' || line[(*data)->end] == '\'')
			handle_special_quot(&token, line, data, env);
		else if (line[(*data)->end] == ' ' || line[(*data)->end] == '\t')
			handle_white_spaces(&token, line, data, env);
		else
			(*data)->end++;
	}
	handle_word_token(&token, line, data, env);
	return (token);
}

void	join_nodes(t_token **token)
{
	char	*joined;

	t_token(*curr), *next;
	curr = *token;
	while (curr && curr->next)
	{
		if (curr->info)
		{
			next = curr->next;
			joined = gc_strjoin(curr->av, next->av);
			if (!joined)
			{
				printf("minishell: memory allocation failed\n");
				return ;
			}
			curr->av = joined;
			curr->next = next->next;
			if (!next->info)
				curr->info = false;
			continue ;
		}
		curr = curr->next;
	}
}
