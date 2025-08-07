/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfatih <helfatih@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 10:52:21 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/04 12:11:23 by helfatih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	make_the_envirement(t_var *var)
{
	size_t	len;

	len = ft_strlen(var->valeur);
	if (var->valeur)
	{
		if (var->j + len >= var->old_size)
		{
			var->new_size = len + (var->j);
			var->result = manual_realloc(var->result, var->new_size);
			var->old_size = var->new_size;
		}
		if (var->flag)
		{
			make_like_bash(var->result, var->valeur, &var->j);
		}
		else
		{
			ft_strlcpy(&var->result[var->j], var->valeur, len + 1);
			var->j += len;
		}
	}
}

char	*split_var(size_t *i, char *str, size_t *start)
{
	char	*string;

	(*i)++;
	*start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	string = gc_substr(str, *start, (*i) - (*start));
	if (!string)
		return (NULL);
	return (string);
}

void	remplace_var(t_var *var)
{
	var->status_str = ft_itoa(get_status());
	if (var->status_str)
	{
		var->valeur = gc_strdup(var->status_str);
		free(var->status_str);
		if (var->valeur)
		{
			make_the_envirement(var);
		}
	}
	var->i += 2;
}
