/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouizak <mbouizak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 17:26:13 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/07 14:12:43 by mbouizak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	parsing_process(t_token **token, char **env, t_data *data, t_command **cmd)
{
	int	check;

	if (data->flags)
		return (0);
	join_nodes(token);
	*cmd = parsing_command(*token, &data);
	if (!*cmd)
		return (0);
	check = interupte_herdoc(*cmd, data, env);
	if (check == 1)
		return (0);
	return (1);
}

void	part_execution(t_command **cmd, char ***env, t_data **data)
{
	g_value = 0;
	if (*cmd)
	{
		if (*cmd)
			execute_command(*cmd, env, data);
		else if ((*cmd)->herdoc_file)
			unlink((*cmd)->herdoc_file);
	}
	gc_cleanup_partial();
}

void	clean_up(char **env)
{
	gc_cleanup();
	if (env)
	{
		free_2d_array(env);
	}
	rl_clear_history();
}

void	make_prompt(char ***env)
{
	char		*line;
	t_token		*token;
	t_command	*cmd;
	t_data		*data;
	int			refer;

	while (1)
	{
		data = gc_malloc(sizeof(t_data));
		if (!data)
			return ;
		ft_memset(data, 0, sizeof(t_data));
		refer = process_line(&token, &line, *env, &data);
		if (refer == 0)
			continue ;
		if (refer == 2)
			break ;
		if (!parsing_process(&token, *env, data, &cmd))
			continue ;
		part_execution(&cmd, env, &data);
	}
	clean_up(*env);
}

int	main(int ac, char **av, char **env)
{
	char	**my_env;

	(void)ac;
	(void)av;
	rl_catch_signals = 0;
	signal(SIGINT, my_handler);
	signal(SIGQUIT, SIG_IGN);
	my_env = copy_env(env);
	make_prompt(&my_env);
}
