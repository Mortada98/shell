/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handle_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouizak <mbouizak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 15:10:54 by mbouizak          #+#    #+#             */
/*   Updated: 2025/08/07 11:24:10 by mbouizak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	lexe_with_space(t_token **token, int *start, int *i, char *word)
{
	char	*str;

	if (*i > *start)
	{
		str = gc_substr(word, *start, *i - *start);
		add_token(token, creat_token(str, TOKEN_WORD, false, false));
	}
	while (word[*i] == ' ' || word[*i] == '\t')
		(*i)++;
	*start = *i;
}

void	join_expansion(char *str, t_token **token)
{
	t_token	*cur;

	cur = *token;
	if (!*token)
		return ;
	if (str[0] == ' ')
	{
		while ((*token)->next)
		{
			(*token) = (*token)->next;
		}
		(*token)->info = false;
		*token = cur;
	}
}

char	*make_content(char *line, t_data **data)
{
	char	*word;

	word = gc_substr(line, (*data)->start, (*data)->end - (*data)->start);
	if (ft_strchr(word, '\'') || ft_strchr(word, '\"'))
		(*data)->should_expand_inside = true;
	if (!word)
		return (NULL);
	return (word);
}

size_t	count_word(char const *s, char c, char k)
{
	size_t	words;
	size_t	i;

	words = 0;
	i = 0;
	while (s[i])
	{
		if ((s[i] != c && s[i]) && (s[i + 1] == c || s[i + 1] == k
				|| s[i + 1] == '\0'))
			words++;
		i++;
	}
	return (words);
}
