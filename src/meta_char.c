

#include "../include/minishell.h"

int 	*set_redir_error(void)
{
	static int g_redir_error = 0;
	return (&g_redir_error);
}


void	reset_redir_error(int value)
{
	int *g_redir_error = set_redir_error();
	*g_redir_error = value;
}

int	get_redir_error(void)
{
	return (*set_redir_error());
}

int	handle_pipe(t_token **current, t_command **current_cmd,
		t_command *first_cmd, t_data **data)
{
	t_command	*new_cmd;

	if ((*current)->type != TOKEN_PIPE)
		return (1);
	if ((*current)->next == NULL)
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		(*data)->exit = 2;
		return (0);
	}
	new_cmd = create_command();
	if (!new_cmd)
	{
		free_cmd(first_cmd);
		return (0);
	}
	(*current_cmd)->next = new_cmd;
	*current_cmd = new_cmd;
	*current = (*current)->next;
	(*data)->exit = 0;
	reset_redir_error(0); // Reset error flag for new command
	return (1);
}

int	handle_redir_in(t_token **current, t_command *cmd, t_data **data)
{
	if (!(*current)->next)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		set_status(2);
		return (0);
	}
	if ((*current)->next->type != TOKEN_WORD)
	{
		printf("minishell: syntax error near unexpected token `%s'\n",
			(*current)->next->av);
		set_status(2);
		return (0);
	}
	if (red_in_realloc(cmd, data, current) == 0)
		return (0);
	(*current) = (*current)->next->next;
	return (1);
}

int	handle_redir_out(t_token **current, t_command *cmd)
{
	int	fd;
	char *filename;

	if (!(*current)->next)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		set_status(2);
		return (0);
	}
	if ((*current)->next->type != TOKEN_WORD)
	{
		printf("minishell: syntax error near unexpected token `%s'\n",
			(*current)->next->av);
		set_status(2);
		return (0);
	}
	
	// If we already had a redirection error, skip processing but continue parsing
	if (get_redir_error())
	{
		(*current) = (*current)->next->next;
		return (1);
	}
	
	filename = gc_strdup((*current)->next->av);
	if (!filename)
		return (0);
	
	// Create/truncate the file immediately (like bash does) - only if no previous error
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		// Show error message like bash does and mark error
		reset_redir_error(1); // Set error flag
		cmd->redir_error = true; // Mark this command as having redirection error
		if (errno == EISDIR)
		{
			write(2, "minishell: ", 11);
			write(2, filename, ft_strlen(filename));
			write(2, ": Is a directory\n", 17);
		}
		else if (errno == ENOTDIR)
		{
			write(2, "minishell: ", 11);
			write(2, filename, ft_strlen(filename));
			write(2, ": Not a directory\n", 18);
		}
		else if (errno == ENOENT)
		{
			write(2, "minishell: ", 11);
			write(2, filename, ft_strlen(filename));
			write(2, ": No such file or directory\n", 28);
		}
		else if (errno == EACCES)
		{
			write(2, "minishell: ", 11);
			write(2, filename, ft_strlen(filename));
			write(2, ": Permission denied\n", 20);
		}
		else
		{
			write(2, "minishell: ", 11);
			write(2, filename, ft_strlen(filename));
			write(2, ": ", 2);
			write(2, strerror(errno), ft_strlen(strerror(errno)));
			write(2, "\n", 1);
		}
		// Don't store failed redirection, but continue parsing
		cmd->file_output = NULL;
	}
	else
	{
		close(fd);
		// Store this as the output target (overwriting previous ones)
		cmd->file_output = filename;
	}
	
	cmd->append = 0;
	(*current) = (*current)->next->next;
	return (1);  // Continue parsing even if redirection failed
}

int	handle_redir_append(t_token **current, t_command *cmd)
{
	int	fd;
	char *filename;

	if (!(*current)->next)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		set_status(2);
		return (0);
	}
	if ((*current)->next->type != TOKEN_WORD)
	{
		printf("minishell: syntax error near unexpected token `%s'\n",
			(*current)->next->av);
		set_status(2);
		return (0);
	}
	
	// If we already had a redirection error, skip processing but continue parsing
	if (get_redir_error())
	{
		(*current) = (*current)->next->next;
		return (1);
	}
	
	filename = gc_strdup((*current)->next->av);
	if (!filename)
		return (0);
	
	// Test if we can create the file (like bash does) - but append mode touches file
	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		// Show error message like bash does and mark error
		reset_redir_error(1); // Set error flag
		cmd->redir_error = true; // Mark this command as having redirection error
		if (errno == EISDIR)
		{
			write(2, "minishell: ", 11);
			write(2, filename, ft_strlen(filename));
			write(2, ": Is a directory\n", 17);
		}
		else if (errno == ENOTDIR)
		{
			write(2, "minishell: ", 11);
			write(2, filename, ft_strlen(filename));
			write(2, ": Not a directory\n", 18);
		}
		else if (errno == ENOENT)
		{
			write(2, "minishell: ", 11);
			write(2, filename, ft_strlen(filename));
			write(2, ": No such file or directory\n", 28);
		}
		else if (errno == EACCES)
		{
			write(2, "minishell: ", 11);
			write(2, filename, ft_strlen(filename));
			write(2, ": Permission denied\n", 20);
		}
		else
		{
			write(2, "minishell: ", 11);
			write(2, filename, ft_strlen(filename));
			write(2, ": ", 2);
			write(2, strerror(errno), ft_strlen(strerror(errno)));
			write(2, "\n", 1);
		}
		// Don't store failed redirection, but continue parsing
		cmd->file_output = NULL;
	}
	else
	{
		close(fd);
		// Store this as the output target (overwriting previous ones)
		cmd->file_output = filename;
	}
	
	cmd->append = 1;
	(*current) = (*current)->next->next;
	return (1);  // Continue parsing even if redirection failed
}

int	handle_heredoc(t_token **current, t_command *cmd, int *i)
{
	if (!(*current)->next)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		set_status(2);
		return (0);
	}
	if ((*current)->next->type != TOKEN_WORD)
	{
		printf("minishell: syntax error near unexpected token `%s'\n",
			(*current)->next->av);
		set_status(2);
		return (0);
	}
	if (heredoc_realloc(i, cmd, current) == 0)
		return (0);
	(*current) = (*current)->next->next;
	return (1);
}
