/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_herdoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouizak <mbouizak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 09:45:55 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/07 11:10:11 by mbouizak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int			g_value = 0;

static char	*generate_file_name(void)
{
	unsigned char	c;
	char			*buffer;
	int				fd;
	int				i;

	buffer = gc_malloc(11);
	if (!buffer)
		return (NULL);
	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
	{
		return (buffer);
	}
	i = 0;
	while (i < 10)
		read_and_convert(buffer, &fd, &c, &i);
	buffer[i] = '\0';
	close(fd);
	return (buffer);
}

static void	herdoc_expansion(int *fd, char **line, t_data **data, char **env)
{
	char	*str;
	char	*expanded_line;

	str = *line;
	if (!(*data)->should_expand_inside)
	{
		expanded_line = expand_env(str, env);
		if (expanded_line && expanded_line != str)
		{
			write(*fd, expanded_line, ft_strlen(expanded_line));
		}
		else
		{
			write(*fd, *line, ft_strlen(*line));
		}
	}
	else
		write(*fd, *line, ft_strlen(*line));
}

static void	make_loop(t_command **cmd, int *fd, t_data **data, char **env)
{
	char	*line;

	g_value = 0;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			if (!g_value)
				printf("warning: delimited by end-of-file (wanted `%s')\n",
					(*cmd)->herdoc[(*data)->start]);
			break ;
		}
		gc_register_external(line);
		if (!(*cmd)->herdoc || (*cmd)->herdoc[(*data)->start] == NULL)
			return ;
		if (ft_strcmp(line, (*cmd)->herdoc[(*data)->start]) == 0)
		{
			return ;
		}
		herdoc_expansion(fd, &line, data, env);
		write(*fd, "\n", 1);
	}
}

static int	minishell_init(char **buffer, char **join, int *fd)
{
	*buffer = generate_file_name();
	if (!*buffer)
	{
		*fd = -1;
		return (0);
	}
	*join = gc_strjoin("/tmp/", *buffer);
	if (!*join)
	{
		*buffer = NULL;
		*fd = -1;
		return (0);
	}
	*fd = open(*join, O_RDWR | O_CREAT, 0644);
	if (*fd < 0)
	{
		*buffer = NULL;
		*join = NULL;
		return (0);
	}
	return (1);
}

void	excute_herdoc_for_child(t_command **cmd, t_data **data, char **env)
{
	int		save;
	int		fd;
	char	*buffer;
	char	*join;

	(*data)->start = 0;
	save = dup(0);
	while ((*data)->start < (*data)->count_herdoc)
	{
		if (!minishell_init(&buffer, &join, &fd))
			return ;
		signal(SIGINT, my_server);
		make_loop(cmd, &fd, data, env);
		close(fd);
		herdoc_condition_1(cmd, data, join);
		if (!herdoc_condition_2(cmd, data))
			break ;
		(*data)->start++;
	}
	dup2(save, 0);
	close(save);
	signal(SIGINT, my_handler);
	(*data)->count_herdoc = 0;
	(*data)->should_expand_inside = false;
}
