#include "../include/minishell.h"

static char	*get_path_display(char **env)
{
	char	*pwd;
	char	*home;
	char	*path;
	bool	from_getcwd;

	pwd = getcwd(NULL, 0);
	from_getcwd = (pwd != NULL);
	if (!pwd)
	{
		// If getcwd fails (directory deleted), try to use PWD env variable
		pwd = get_env("PWD", env);
		if (pwd)
			pwd = gc_strdup(pwd);
		else
			return (gc_strdup("(unknown)"));
	}
	if (!pwd)
		return (gc_strdup("(unknown)"));
	home = get_env("HOME", env);
	if (home && ft_strnstr(pwd, home, ft_strlen(pwd)))
	{
		path = ft_strjoin("~", pwd + ft_strlen(home));
		if (from_getcwd)
			free(pwd);  // Only free if we got it from getcwd, not gc_strdup
	}
	else
		path = pwd;
	return (path);
}

static char	*get_user_display(char **env)
{
	char	*user;
	char	*base;
	char	*full;

	user = get_env("USER", env);
	if (!user)
		user = "unknown";
	base = ft_strjoin(BLUE, user);
	if (!base)
		return (NULL);
	full = ft_strjoin(base, "@shell\033[0m\033[1;33m | \033[1;32m");
	free(base);
	return (full);
}

char	*prompt(char **env)
{
	char	*prompt;
	char	*path;
	char	*user;
	char	*p_arrow;

	path = get_path_display(env);
	if (!path)
		return (NULL);

	user = get_user_display(env);
	if (!user)
		return (free(path), NULL);

	p_arrow = ft_strjoin(path, "\e[1;33m ➜ \033[0m");
	free(path);
	if (!p_arrow)
		return (free(user), NULL);

	prompt = ft_strjoin(user, p_arrow);
	free(user);
	free(p_arrow);
	if (!prompt)
		return (NULL);

	gc_register_external(prompt);
	return (prompt);
}
