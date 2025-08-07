/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfatih <helfatih@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:36:39 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/05 16:37:55 by helfatih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*manual_realloc(char *old, size_t len)
{
	char	*new;

	new = gc_malloc(len + 1);
	if (new == NULL)
		return (NULL);
	if (old)
	{
		ft_memcpy(new, old, len);
	}
	new[len] = '\0';
	return (new);
}

bool	con(char *str)
{
	if (str[0] == '\'')
		return (false);
	else
		return (true);
}

bool	flaging(char *str)
{
	if (str[0] != '\'' && str[0] != '\"')
	{
		return (true);
	}
	else
	{
		return (false);
	}
}

void	make_like_bash(char *result, char *valeur, size_t *j)
{
	int	k;

	k = 0;
	while (valeur[k])
	{
		if (valeur[k] == ' ' || valeur[k] == '\t')
		{
			while (valeur[k] == ' ' || valeur[k] == '\t')
			{
				k++;
			}
			result[(*j)++] = ' ';
		}
		else
		{
			result[(*j)++] = valeur[k++];
		}
	}
}

int	init_var(char *str, t_var *var)
{
	var->flag = false;
	var->old_size = (ft_strlen(str) + 2 + 100);
	var->i = 0;
	var->j = 0;
	var->condition = con(str);
	var->flag = flaging(str);
	var->result = (char *)gc_calloc((var->old_size), 1);
	if (var->result == NULL)
	{
		return (0);
	}
	return (1);
}
