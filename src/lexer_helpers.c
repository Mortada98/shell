/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouizak <mbouizak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 21:34:54 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/07 11:22:55 by mbouizak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	small_condition(char *str, t_data *data)
{
	if (str[0] == 0)
	{
		data->flags = 1;
		return (0);
	}
	return (1);
}

char	*handle_expansion(t_data *data, char *word, char **env)
{
	char	*str;
	int		count;

	if (!data->should_expand_outside)
	{
		str = expand_env(word, env);
		if (data->ambigiouse && ft_strcmp(str, word) != 0)
		{
			count = count_word(str, ' ', '\t');
			if (data->ambigiouse && (count != 1 || !small_condition(str, data)))
			{
				write(2, "minishell : ", 12);
				write(2, word, ft_strlen(word));
				write(2, ": ambiguous redirect\n", 21);
				data->flags = 1;
				return (NULL);
			}
		}
		// if (!small_condition(str, data))
		// 	return (NULL);
		return (str);
	}
	return (word);
}

char	*process_quotes(char *str, int *flag)
{
	if (str[0] == '"' || str[0] == '\'')
	{
		str = remove_quotes(str);
		*flag = 1;
	}
	return (str);
}

void	create_add_token(t_token **token, char *word, t_token_type value, t_word_processing *wp)
{
	t_token	*new;

	new = creat_token(word, value, wp->should_join, wp->found);
	if (new)
		add_token(token, new);
}

void	process_word(t_token **token, t_word_processing *wp)
{
	join_expansion(wp->str, token);
	// if ((wp->str[0] != '"' || wp->str[0] != '\'') && wp->str[0] == '\0')
	// 	return ;
	wp->str = process_quotes(wp->str, &wp->flag);
	if (wp->str && wp->str != wp->word)
		wp->word = wp->str;
	if (!wp->flag)
		wp->value = get_token_type(wp->word);
	if (!wp->flag && (ft_strchr(wp->word, ' ') || ft_strchr(wp->word, '\t')))
	{
		make_list(wp->word, token);
		return ;
	}
	create_add_token(token, wp->word, wp->value, wp);
}
