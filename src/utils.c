#include "../include/minishell.h"
#include <stdio.h>

void	handle_dollar(t_token **token, char *line, t_data **data, char **env)
{
	if ((*data)->end > (*data)->start)
	{
		handle_word_token(token, line, data, env);
	}
	(*data)->start = (*data)->end;
	(*data)->end++;
	while ((ft_isalnum(line[(*data)->end]) || line[(*data)->end] == '_')
		&& line[(*data)->end])
		(*data)->end++;
	if ((*data)->end > (*data)->start)
	{
		handle_word_token(token, line, data, env);
	}
	(*data)->start = (*data)->end;
}

void	handle_special_quot(t_token **token, char *line, t_data **data, char **env)
{
	char	q;

	if (line[(*data)->start] == '$' && (line[(*data)->end] == '\"'
			|| line[(*data)->end] == '\''))
		(*data)->start = (*data)->end;
	if ((*data)->end > (*data)->start)
	{
		if (line[(*data)->end - 1] == '$')
		{
			(*data)->end -= 1;
			handle_word_token(token, line, data, env);
			(*data)->end++;
		}
		else
			handle_word_token(token, line, data, env);
	}
	(*data)->start = (*data)->end;
	q = line[(*data)->end];
	(*data)->end++;
	while (line[(*data)->end] != q)
		(*data)->end++;
	if (line[(*data)->end] == q)
		(*data)->end++;
	handle_word_token(token, line, data, env);
	(*data)->start = (*data)->end;
}

void	handle_white_spaces(t_token **token, char *line, t_data **data, char **env)
{
	handle_word_token(token, line, data, env);
	while (line[(*data)->end] == ' ' || line[(*data)->end] == '\t')
		(*data)->end++;
	(*data)->start = (*data)->end;
}

bool	check_somthing(char *word)
{
	if (!is_closed_quotes(word))
	{
		printf("minishell : the quote does not close!\n");
		set_status(0);
		return (false);
	}
	return (true);
}

void	handle_some_cases(t_token **token, char *line, t_data **data, char **env)
{
	if ((*data)->end > (*data)->start)
	{
		handle_word_token(token, line, data, env);
		(*data)->start = (*data)->end;
	}
	if (line[(*data)->end + 1] == '?')
	{
		(*data)->end += 2;
		handle_word_token(token, line, data, env);
		(*data)->start = (*data)->end;
	}
}
