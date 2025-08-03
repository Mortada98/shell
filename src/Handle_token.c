/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handle_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouizak <mbouizak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 15:10:54 by mbouizak          #+#    #+#             */
/*   Updated: 2025/07/30 20:46:06 by mbouizak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

t_token	*creat_token(char *line, t_token_type type, bool should_join)
{
	t_token	*new_token;

	if (!(new_token = (t_token *)gc_calloc(1, sizeof(t_token))))
		return (NULL);
	if (!(new_token->av = gc_strdup(line)))
		return (NULL);
	new_token->type = type;
	new_token->info = should_join;
	// printf("%d->%d->%s\n", *j, should_join, new_token->av);
	new_token->next = NULL;
	new_token->prev = NULL;
	return (new_token);
}

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

void	init_variables(char *str, int *i, int *j, bool *in_quotes,
		char *quote_char, size_t *len, char **result)
{
	*in_quotes = false;
	*j = 0;
	*i = 0;
	*quote_char = 0;
	*len = ft_strlen(str);
	*result = gc_malloc(*len + 1);
}

char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	bool	in_single;
	bool	in_double;
	char	*result;

	if (!str)
		return (NULL);
	i = 0, j = 0;
	in_single = false;
	in_double = false;
	result = gc_malloc(strlen(str) + 1);
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
		{
			in_single = !in_single;
			i++;
			continue ;
		}
		else if (str[i] == '"' && !in_single)
		{
			in_double = !in_double;
			i++;
			continue ;
		}
		if (in_single)
		{
			result[j++] = str[i++];
		}
		else
		{
			result[j++] = str[i++];
		}
	}
	result[j] = '\0';
	return (result);
}

bool	is_closed_quotes(char *str)
{
	int		i;
	bool	in_single;
	bool	in_double;

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

bool	special_character(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\\' || str[i] == ';')
			return (false);
		i++;
	}
	return (true);
}

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

void	lexe_with_space(t_token **token, int *start, int *i, char *word,
		bool *should_join, t_token_type value)
{
	char	*str;

	if (*i > *start)
	{
		str = gc_substr(word, *start, *i - *start);
		add_token(token, creat_token(str, value, *should_join));
	}
	while (word[*i] == ' ' || word[*i] == '\t')
		(*i)++;
	*start = *i;
	*should_join = true;
}

void	make_list(char *word, t_token **token, t_token_type value)
{
	bool	should_join;

	int start, i;
	init_var2(&start, &i, &should_join);
	while (word[i])
	{
		if (word[i] == ' ' || word[i] == '\t')
		{
			should_join = false;
			lexe_with_space(token, &start, &i, word, &should_join, value);
			continue ;
		}
		i++;
	}
	lexe_with_space(token, &start, &i, word, &should_join, value);
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
		while (cur->next)
		{
			cur = cur->next;
		}
		if (cur->type == TOKEN_HERDOC)
			(*data)->should_expand_outside = true;
		else if ((cur->type == TOKEN_REDIR_APPEND
				|| cur->type == TOKEN_REDIR_OUT || cur->type == TOKEN_REDIR_IN)
			&& cur->next == NULL)
		{
			(*data)->ambigiouse = true;
		}
	}
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
		free(convert); // ft_itoa uses malloc, so we still need to free this
		*word = new_word;
	}
}

void	mix(bool *should_join, char *line, int i)
{
	if (!ft_space(line[i]) && !ft_meta_c(line[i]) && line[i] != '\0')
		*should_join = true;
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

static size_t	count_word(char const *s, char c, char k)
{
	size_t	words;
	size_t	i;

	words = 0;
	i = 0;
	while (s[i])
	{
		if ((s[i] != c && s[i]) && (s[i + 1] == c || s[i + 1] == k || s[i
				+ 1] == '\0'))
			words++;
		i++;
	}
	return (words);
}

void	handle_word_token(t_token **token, char *line, t_data **data, char **env)
{
	bool			should_join;
	t_token			*new;
	int				flag;
	t_token_type	value;
	int				count;

	should_join = false;
	flag = 0;
	char *str, *word;
	value = TOKEN_WORD;
	(*data)->should_expand_outside = false;
	check_the_last_element(token, data);
	if ((*data)->end > (*data)->start)
	{
		mix(&should_join, line, (*data)->end);
		word = make_content(line, data);
		if (!word)
			return ;
		convert_exit_status(&word);
		if (word && *word != '\0')
		{
			if (!(*data)->should_expand_outside)
			{
				str = expand_env(word, env);
				if (ft_strcmp(str, word) != 0)
				{
					count = count_word(str, ' ', '\t');
					if ((*data)->ambigiouse && (count > 1 || count == 0))
					{
						printf("minishell : %s: ambiguous redirect\n", word);
						(*data)->flags = 1;
						return ;
					}
				}
			}
			else
				str = word;
			join_expansion(str, token);
			if ((str[0] != '"' || str[0] != '\'') && str[0] == '\0')
			{
				str = NULL;
				return ;
			}
			if (str[0] == '"' || str[0] == '\'')
			{
				// value = get_token_type(str);
				str = remove_quotes(str);
				flag = 1;
			}
			if (str && str != word)
			{
				word = str;
			}
			if (!flag)
				value = get_token_type(word);
			if (!flag && (ft_strchr(word, ' ') || ft_strchr(word, '\t')))
			{
				make_list(word, token, value);
				return ;
			}
			new = creat_token(word, value, should_join);
			if (new)
				add_token(token, new);
		}
	}
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
				should_join));
		return (i + 2);
	}
	else
	{
		special[0] = line[i];
		special[1] = '\0';
		add_token(token, creat_token(special, get_token_type(special),
				should_join));
		return (i + 1);
	}
}
