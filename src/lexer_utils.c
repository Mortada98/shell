/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfatih <helfatih@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 20:27:26 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/03 21:48:51 by helfatih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_quote(bool *in_quot, char *quot_char, int *i, char *line)
{
	if ((line[*i] == '"' || line[*i] == '\''))
	{
		if (*quot_char == '\0')
		{
			*in_quot = true;
			*quot_char = line[*i];
		}
		else if (*quot_char == line[*i])
		{
			*in_quot = false;
			*quot_char = 0;
		}
	}
}

int	process_quote(char *str, int *i, bool *in_double, bool *in_single)
{
	if (str[*i] == '\'' && !*in_double)
	{
		*in_single = !*in_single;
		(*i)++;
		return (1);
	}
	else if (str[*i] == '"' && !*in_single)
	{
		*in_double = !*in_double;
		(*i)++;
		return (1);
	}
	return (0);
}

void	reset_var3(int *i, int *j, bool *in_single, bool *in_double)
{
	*i = 0;
	*j = 0;
	*in_single = false;
	*in_double = false;
}

char	*remove_quotes(char *str)
{
	bool	in_single;
	bool	in_double;
	char	*result;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	reset_var3(&i, &j, &in_single, &in_double);
	result = gc_malloc(strlen(str) + 1);
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (process_quote(str, &i, &in_double, &in_single))
			continue ;
		if (in_single)
			result[j++] = str[i++];
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

bool	is_closed_quotes(char *str)
{
	bool	in_single;
	bool	in_double;
	int		i;

	i = 0;
	in_double = false;
	in_single = false;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '"' && !in_single)
			in_double = !in_double;
		i++;
	}
	return (!in_single && !in_double);
}
