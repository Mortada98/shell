/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouizak <mbouizak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 20:20:58 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/07 11:24:45 by mbouizak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	add_token(t_token **token, t_token *new_token)
{
	t_token	*current;

	if (!*token)
	{
		*token = new_token;
		return ;
	}
	current = *token;
	while (current->next)
		current = current->next;
	current->next = new_token;
	new_token->prev = current;
}

t_token	*creat_token(char *line, t_token_type type, bool should_join, bool found)
{
	t_token	*new_token;

	new_token = (t_token *)gc_calloc(1, sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->av = gc_strdup(line);
	if (!new_token->av)
		return (NULL);
	new_token->type = type;
	new_token->found = found;
	new_token->info = should_join;
	new_token->next = NULL;
	new_token->prev = NULL;
	return (new_token);
}

void	handle_word_token(t_token **token, char *line, t_data **data,
		char **env)
{
	t_word_processing	wp;

	wp.should_join = false;
	wp.flag = 0;
	wp.value = TOKEN_WORD;
	wp.found = false;
	(*data)->should_expand_outside = false;
	check_the_last_element(token, data);
	if ((*data)->end <= (*data)->start)
		return ;
	mix(&wp.should_join, line, (*data)->end);
	wp.word = make_content(line, data);
	if (!wp.word)
		return ;
	convert_exit_status(&wp.word);
	if (!wp.word || *wp.word == '\0')
		return ;
	wp.str = handle_expansion(*data, wp.word, env);
	if (!wp.str)
		return ;
	if (wp.str[0] == '\0')
	{
		wp.found = true;
	}
	process_word(token, &wp);
}

int	handle_speciale_token(t_token **token, char *line, int i, t_data **data)
{
	char	*special;
	bool	should_join;

	(*data)->exit = 0;
	special = gc_malloc(3 * sizeof(char));
	if (!special)
		return (0);
	should_join = false;
	if ((line[i] == '>' || line[i] == '<') && (line[i + 1] == line[i]))
	{
		special[0] = line[i];
		special[1] = line[i];
		special[2] = '\0';
		add_token(token, creat_token(special, get_token_type(special),
				should_join, false));
		return (i + 2);
	}
	else
	{
		special[0] = line[i];
		special[1] = '\0';
		add_token(token, creat_token(special, get_token_type(special),
				should_join, false));
		return (i + 1);
	}
}
