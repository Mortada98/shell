/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfatih <helfatih@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 20:43:25 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/03 21:39:32 by helfatih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	ft_space(char c)
{
	return ((c >= 9 && c <= 13) || c == ' ');
}

bool	ft_meta_c(char c)
{
	return (c == '>' || c == '|' || c == '<');
}

void	init_var2(int *start, int *i, bool *should_join)
{
	*start = 0;
	*i = 0;
	*should_join = false;
}

void	convert_exit_status(char **word)
{
	char	*convert;
	char	*new_word;

	if (ft_strncmp(*word, "$?", 2) == 0)
	{
		convert = ft_itoa(get_status());
		new_word = gc_strdup(convert);
		set_status(0);
		free(convert);
		*word = new_word;
	}
}

void	mix(bool *should_join, char *line, int i)
{
	if (!ft_space(line[i]) && !ft_meta_c(line[i]) && line[i] != '\0')
		*should_join = true;
}
