/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excute.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouizak <mbouizak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:00:25 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/07 14:52:04 by mbouizak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	built_in_part2(t_command *cmd, char ***env)
{
	int	i;

	if (ft_strcmp(cmd->args[0], "export") == 0)
	{
		if (cmd->args[1])
		{
			i = 0;
			while (cmd->args[++i])
				my_export(cmd->args[i], env);
		}
		else
			print_export_env(*env);
	}
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
	{
		if (cmd->args[1])
		{
			i = 0;
			while (cmd->args[++i])
				my_unset(cmd->args[i], env);
		}
	}
}

void	built_in_part1(t_command *cmd, char ***env)
{
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		my_echo(cmd);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
	{
		if (cmd->args[1])
			cd(cmd->args[1], env);
		else
			cd(NULL, env);
	}
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		print_env(*env);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		my_pwd();
}

void	execute_builtin_command(t_command *cmd, char ***env)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	built_in_part1(cmd, env);
	built_in_part2(cmd, env);
}

void	iterate_on_env(char **env, char **path_env, char *cmd)
{
	int	i;
	int j;

	if (!env || !*env)
		return ;
	*path_env = NULL;
	i = 0;
	j = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			*path_env = env[i] + 5;
			j++;
			break ;
		}
		i++;
	}
	if (j == 0)
	{
		write(2, "minishell: ", 12);
		write(2, cmd, ft_strlen(cmd));
		write(2, ": No such file pr directory\n", 29);
	}
}

char	*get_command(char *cmd, char **env)
{
	t_cmd_var	var;
	int			i;

	i = 0;
	if (ft_strchr(cmd, '/'))
		return (check_file(cmd));
	iterate_on_env(env, &var.path_env, cmd);
	var.split_env = ft_split(var.path_env, ':');
	var.first_join = gc_strjoin("/", cmd);
	i = 0;
	while (var.split_env[i])
	{
		var.complete_path = gc_strjoin(var.split_env[i], var.first_join);
		stat(var.complete_path, &var.sb);
		if (!access(var.complete_path, X_OK) && !S_ISDIR(var.sb.st_mode))
			return (var.complete_path);
		if (!access(var.complete_path, F_OK) && !S_ISDIR(var.sb.st_mode))
			return (var.complete_path);
		i++;
	}
	write(2, "minishell: ", 11);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": command not found\n", 20);
	set_status(127);
	return (NULL);
}
