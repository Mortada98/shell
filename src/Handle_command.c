/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handle_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouizak <mbouizak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 17:26:30 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/07 11:33:55 by mbouizak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	cmd->redir_error = false;
	cmd->next = NULL;
	return (cmd);
}

char	*expand_env(char *str, char **env)
{
	t_var	var;

	// (void)cmd_found; // Avoid unused parameter warning
	if (!init_var(str, &var))
		return (NULL);
	while (str[var.i])
	{
		if (var.condition && str[var.i] == '$' && str[var.i + 1] == '?')
		{
			remplace_var(&var);
			continue ;
		}
		if (var.condition && str[var.i] == '$' && str[var.i + 1]
			&& (ft_isalnum(str[var.i + 1]) || str[var.i + 1] == '_'))
		{
			var.string = split_var(&var.i, str, &var.start);
			if (!var.string)
				return (NULL);
			var.valeur = get_env(var.string, env);
			if (var.valeur)
				make_the_envirement(&var);
			else if (!var.valeur)
				var.result[var.j++] = '\0';
			continue ;
		}
		var.result[var.j++] = str[var.i++];
	}
	return (var.result);
}
t_command	*parsing_command(t_token *token, t_data **data)
{
	t_parse	var;

	if (reset_value(&var.current_cmd, &var.current, data, token) == 0)
		return (NULL);
	var.first_cmd = var.current_cmd;
	var.i = 0;
	while (var.current)
	{
		if (!process_current_token(&var, data))
			return (NULL);
	}
	return (var.first_cmd);
}
