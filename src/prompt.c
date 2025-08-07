/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: mbouizak <mbouizak@student.42.fr>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2025/08/07 14:16:43 by mbouizak          #+#    #+#             */
/*   Updated: 2025/08/07 14:16:43 by mbouizak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*apply_tilde_replacement(char *pwd, char **env)
{
	char	*home;
	size_t	home_len;

	if (!pwd)
		return (gc_strdup("(unknown)"));
	home = get_env("HOME", env);
	if (home)
	{
		home_len = ft_strlen(home);
		if (ft_strncmp(pwd, home, home_len) == 0 && (pwd[home_len] == '/'
				|| pwd[home_len] == '\0'))
		{
			return (gc_strjoin("~", pwd + home_len));
		}
	}
	return (pwd);
}

static char	*get_pwd_value(char **env)
{
	char	*pwd;
	char	*result;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		pwd = get_env("PWD", env);
		if (pwd)
		{
			result = gc_strdup(pwd);
			return (apply_tilde_replacement(result, env));
		}
		else
			return (gc_strdup("(unknown)"));
	}
	result = gc_strdup(pwd);
	free(pwd);
	return (apply_tilde_replacement(result, env));
}

static char	*get_path_display(char **env)
{
	char	*pwd;
	char	*temp;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		return (get_pwd_value(env));
	}
	temp = gc_strdup(pwd);
	free(pwd);
	return (apply_tilde_replacement(temp, env));
}

static char	*get_user_display(char **env)
{
	char	*user;
	char	*base;
	char	*full;

	user = get_env("USER", env);
	if (!user)
		user = "unknown";
	base = gc_strjoin(BLUE, user);
	if (!base)
		return (NULL);
	full = gc_strjoin(base, "@shell\033[0m\033[1;33m | \033[1;32m");
	return (full);
}

char	*prompt(char **env)
{
	char	*prompt_str;
	char	*path;
	char	*user;
	char	*p_arrow;

	path = get_path_display(env);
	if (!path)
		return (NULL);
	user = get_user_display(env);
	if (!user)
		return (NULL);
	p_arrow = gc_strjoin(path, "\e[1;33m ➜ \033[0m");
	if (!p_arrow)
		return (NULL);
	prompt_str = gc_strjoin(user, p_arrow);
	return (prompt_str);
}
