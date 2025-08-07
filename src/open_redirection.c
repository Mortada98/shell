/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfatih <helfatih@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:44:30 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/05 16:31:45 by helfatih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	open_red_in(int *fd_in, t_command **cmd)
{
	int	i;

	i = 0;
	while ((*cmd)->file_input && (*cmd)->file_input[i])
	{
		*fd_in = open((*cmd)->file_input[i], O_RDONLY);
		if (*fd_in < 0)
		{
			write(2, "minishell: ", 11);
			write(2, (*cmd)->file_input[i], ft_strlen((*cmd)->file_input[i]));
			if (errno == ENOTDIR)
				write(2, ": Not a directory\n", 18);
			else if (errno == ENOENT)
				write(2, ": No such file or directory\n", 28);
			else if (errno == EACCES)
				write(2, ": Permission denied\n", 20);
			else
				write(2, ": No such file or directory\n", 28);
			set_status(1);
			exit(1);
		}
		dup2(*fd_in, STDIN_FILENO);
		close(*fd_in);
		i++;
	}
}

void	open_red_out(t_command **cmd, int *fd_out)
{
	int	flags;

	if (is_directory(cmd))
		exit(1);
	flags = O_WRONLY | O_CREAT | append_or_trunc(cmd);
	*fd_out = open((*cmd)->file_output, flags, 0644);
	if (*fd_out < 0)
	{
		write(2, "minishell: ", 11);
		write(2, (*cmd)->file_output, ft_strlen((*cmd)->file_output));
		if (errno == ENOTDIR)
			write(2, ": Not a directory\n", 18);
		else if (errno == ENOENT)
			write(2, ": No such file or directory\n", 28);
		else if (errno == EACCES)
			write(2, ": Permission denied\n", 20);
		else if (errno == EISDIR)
			write(2, ": Is a directory\n", 17);
		else
			write(2, ": No such file or directory\n", 28);
		set_status(1);
		exit(1);
	}
	dup2(*fd_out, STDOUT_FILENO);
	close(*fd_out);
}

int	is_directory(t_command **cmd)
{
	DIR	*folder;

	folder = opendir((*cmd)->file_output);
	if (folder != NULL)
	{
		write(2, "minishell : ", 12);
		write(2, (*cmd)->file_output, ft_strlen((*cmd)->file_output));
		write(2, ":  Is a directory\n", 18);
		set_status(1);
		closedir(folder);
		return (1);
	}
	closedir(folder);
	return (0);
}

int	append_or_trunc(t_command **cmd)
{
	if ((*cmd)->append)
	{
		return (O_APPEND);
	}
	else
		return (O_TRUNC);
}

void	excute_redirection_of_child(t_command **cmd, t_data **data, int *fd_out,
		int *fd_in)
{
	(void)data;
	if ((*cmd)->file_input)
	{
		open_red_in(fd_in, cmd);
	}
	if ((*cmd)->file_output)
	{
		open_red_out(cmd, fd_out);
	}
}
