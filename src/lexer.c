/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfatih <helfatih@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 20:15:47 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/03 20:15:50 by helfatih         ###   ########.fr       */
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

int	handle_dollar_cases(t_token **token, t_data **data, char *line, char **env)
{
	if (line[(*data)->end] == '$' && (ft_isalnum(line[(*data)->end + 1])
			|| line[(*data)->end + 1] == '_'))
	{
		handle_dollar(token, line, data, env);
		return (0);
	}
	if (line[(*data)->end] == '$' && !ft_isalnum(line[(*data)->end + 1]))
	{
		handle_some_cases(token, line, data, env);
	}
	(*data)->end++;
	return (1);
}

void	handle_other_cases(t_token **token, char *line, t_data **data,
		char **env)
{
	if (line[(*data)->end] == '|' || line[(*data)->end] == '<'
		|| line[(*data)->end] == '>')
	{
		handle_word_token(token, line, data, env);
		(*data)->end = handle_speciale_token(token, line, (*data)->end, data);
		(*data)->start = (*data)->end;
	}
	else if (line[(*data)->end] == '\"' || line[(*data)->end] == '\'')
		handle_special_quot(token, line, data, env);
	else if (line[(*data)->end] == ' ' || line[(*data)->end] == '\t')
		handle_white_spaces(token, line, data, env);
	else
		(*data)->end++;
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
		if (line[(*data)->end] == '$')
		{
			if (!handle_dollar_cases(&token, data, line, env))
				continue ;
		}
		else
			handle_other_cases(&token, line, data, env);
	}
	handle_word_token(&token, line, data, env);
	return (token);
}

void	join_nodes(t_token **token)
{
	t_token	*curr;
	t_token	*next;
	char	*joined;

	curr = *token;
	while (curr && curr->next)
	{
		if (curr->info)
		{
			next = curr->next;
			joined = gc_strjoin(curr->av, next->av);
			if (!joined)
				return ;
			curr->av = joined;
			curr->next = next->next;
			if (!next->info)
				curr->info = false;
			continue ;
		}
		curr = curr->next;
	}
}
