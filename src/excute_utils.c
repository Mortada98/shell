/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excute_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfatih <helfatih@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 13:56:53 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/06 09:14:48 by helfatih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	process_options(char **args, bool *has_n, int *idx)
{
	int	i;
	int	k;

	i = 1;
	while (args[i] && args[i][0] == '-')
	{
		if (args[i][1] == '\0')
			break;		
		k = 1;
		while (args[i][k] == 'n')
			k++;		
		if (args[i][k] != '\0')
			break;		
		*has_n = true;
		i++;
	}
	*idx = i;
}

void	print_args(char **args, int idx)
{
	while (args[idx])
	{
		ft_putstr_fd(args[idx], 1);
		if (args[idx + 1])
			ft_putstr_fd(" ", 1);
		idx++;
	}
}

char	*check_file(char *cmd)
{
	struct stat	st;
	DIR			*folder;

	if (stat(cmd, &st) == -1)
	{
		perror("minishell: ");
		set_status(127);
		return (NULL);
	}
	folder = opendir(cmd);
	if (folder != NULL)
	{
		write(2, "minishell: ", 11);
		write(2, cmd, ft_strlen(cmd));
		write(2, ": Is a directory\n", 17);
		set_status(126);
		closedir(folder);
		return (NULL);
	}
	closedir(folder);
	if (access(cmd, X_OK) == 0)
		return (gc_strdup(cmd));
	if (access(cmd, F_OK) == 0)
		return (gc_strdup(cmd));
	return (NULL);
}


void	check_n_option(char *str, bool *has_n)
{
	int	i;

	if (ft_strncmp(str, "-n", 2) != 0)
		return;
	
	i = 1;
	while (str[i] == 'n')
		i++;
	
	if (str[i] == '\0')
		*has_n = true;
}

void	my_echo(t_command *cmd)
{
	int		i;
	bool	has_n;

	has_n = false;
	i = 1;
	if (!cmd->args[i])
	{
		ft_putstr_fd("\n", 1);
		return;
	}
	process_options(cmd->args, &has_n, &i);
	print_args(cmd->args, i);
	if (!has_n)
		ft_putstr_fd("\n", 1);
	
	set_status(0);
}

bool	built_in(char *cmd)
{
	if (ft_strcmp(cmd, "exit") == 0)
		return (true);
	if (ft_strcmp(cmd, "echo") == 0)
		return (true);
	if (ft_strcmp(cmd, "cd") == 0)
		return (true);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (true);
	if (ft_strcmp(cmd, "export") == 0)
		return (true);
	if (ft_strcmp(cmd, "unset") == 0)
		return (true);
	if (ft_strcmp(cmd, "env") == 0)
		return (true);
	else
	{
		return (false);
	}
}

void	free_2d_array(char **str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}
