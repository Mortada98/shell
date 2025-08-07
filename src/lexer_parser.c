/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfatih <helfatih@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 20:41:03 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/03 21:45:54 by helfatih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	check_next_element(t_token *cur, t_data **data)
{
	while (cur->next)
	{
		cur = cur->next;
	}
	if (cur->type == TOKEN_HERDOC)
		(*data)->should_expand_outside = true;
	else if ((cur->type == TOKEN_REDIR_APPEND || cur->type == TOKEN_REDIR_OUT
			|| cur->type == TOKEN_REDIR_IN) && cur->next == NULL)
	{
		(*data)->ambigiouse = true;
	}
}

void	make_list(char *word, t_token **token)
{
	bool	should_join;
	int		start;
	int		i;

	init_var2(&start, &i, &should_join);
	while (word[i])
	{
		if (word[i] == ' ' || word[i] == '\t')
		{
			lexe_with_space(token, &start, &i, word);
			should_join = true;
			continue ;
		}
		i++;
	}
	lexe_with_space(token, &start, &i, word);
}

void	check_the_last_element(t_token **token, t_data **data)
{
	t_token	*cur;

	cur = *token;
	if (!*token)
		return ;
	if (cur->type == TOKEN_HERDOC)
	{
		(*data)->should_expand_outside = true;
		return ;
	}
	if (cur->next == NULL && (cur->type == TOKEN_REDIR_APPEND
			|| cur->type == TOKEN_REDIR_OUT || cur->type == TOKEN_REDIR_IN))
	{
		(*data)->ambigiouse = true;
		return ;
	}
	else if (cur->next)
	{
		check_next_element(cur, data);
	}
}
