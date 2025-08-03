/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handle_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouizak <mbouizak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 17:26:30 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/02 21:57:48 by mbouizak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <uchar.h>

t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = (t_command *)gc_calloc(1, sizeof(t_command));
	if (cmd == NULL)
	{
		return (NULL);
	}
	cmd->args = NULL;
	cmd->file_input = NULL;
	cmd->file_output = NULL;
	cmd->herdoc = NULL;
	cmd->herdoc_file = NULL;
	cmd->file = false;
	cmd->redir_error = false;  // Initialize redirection error flag
	cmd->next = NULL;
	return (cmd);
}

void	make_the_envirement(char **result, char *valeur, size_t *old_size,
		size_t *new_size, bool flag, size_t *j)
{
	size_t	len;

	len = ft_strlen(valeur);
	if (valeur)
	{
		if (*j + len >= *old_size)
		{
			*new_size = len + (*j);
			*result = manual_realloc(*result, *new_size);
			*old_size = *new_size;
		}
		if (flag)
		{
			make_like_bash(*result, valeur, j);
		}
		else
		{
			ft_strlcpy(&(*result)[*j], valeur, len + 1);
			*j += len;
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

char	*expand_env(char *str, char **env)
{
	char *result, *string, *valeur, *status_str;
	bool condition, flag;
	size_t(i), j, old_size, new_size, start;
	init_var(str, &i, &j, &old_size, &condition, &flag);
	if (!(result = (char *)gc_calloc((old_size), 1)))
		return (NULL);
	while (str[i])
	{
		if (condition && str[i] == '$' && str[i + 1] == '?')
		{
			status_str = ft_itoa(get_status());
			if (status_str)
			{
				valeur = gc_strdup(status_str);
				free(status_str);
				if (valeur)
				{
					make_the_envirement(&result, valeur, &old_size, &new_size, flag,
						&j);
				}
			}
			i += 2;
			continue ;
		}
		if (condition && str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) || str[i
				+ 1] == '_'))
		{
			string = split_var(&i, str, &start);
			if (!string)
				return (NULL);
			valeur = get_env(string, env);
			if (valeur)
			{
				make_the_envirement(&result, valeur, &old_size, &new_size, flag,
					&j);
			}
			continue ;
		}
		result[j++] = str[i++];
	}
	return (result);
}

void	how_many(t_token *token, t_data **data)
{
	t_token	*cur;

	(*data)->count_herdoc = 0;
	(*data)->count_red_in = 0;
	cur = token;
	while (cur)
	{
		if (cur->type == TOKEN_HERDOC)
			(*data)->count_herdoc++;
		if (cur->type == TOKEN_REDIR_IN)
			(*data)->count_red_in++;
		cur = cur->next;
	}
}

t_command	*parsing_command(t_token *token, t_data **data)
{
	t_token		*current;
	t_command	*first_cmd;
	t_command	*current_cmd;
	int			i;

	reset_redir_error(0); // Reset redirection error flag for new command line
	current = token;
	current_cmd = create_command();
	if (!current_cmd)
		return (NULL);
	first_cmd = current_cmd;
	(*data)->count_red_in = 0;
	i = 0;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			i = 0;
			(*data)->count_red_in = 0;
			if (!handle_pipe(&current, &current_cmd, first_cmd, data))
				return (NULL);
		}
		else if (current->type == TOKEN_REDIR_IN)
		{
			if (!handle_redir_in(&current, current_cmd, data))
				return (NULL);
		}
		else if (current->type == TOKEN_REDIR_OUT)
		{
			if (!handle_redir_out(&current, current_cmd))
				return (NULL);
		}
		else if (current->type == TOKEN_REDIR_APPEND)
		{
			if (!handle_redir_append(&current, current_cmd))
				return (NULL);
		}
		else if (current->type == TOKEN_HERDOC)
		{
			if (!handle_heredoc(&current, current_cmd, &i))
				return (NULL);
		}
		else if (current->type == TOKEN_WORD)
		{
			append_arg(current_cmd, current->av, data);
			current = current->next;
		}
	}
	return (first_cmd);
}
