#include "../include/minishell.h"

void	update_oldpwd(char **env)
{
	int		i;
	char	*oldpwd;
	char	*new_oldpwd;

	oldpwd = get_env("PWD", env);
	if (!oldpwd)
		return ;
	new_oldpwd = ft_strjoin("OLDPWD=", oldpwd);
	if (!new_oldpwd)
		return ;
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "OLDPWD=", 7) == 0)
		{
			free(env[i]);
			env[i] = new_oldpwd;
			return ;
		}
		i++;
	}
	add_env_variable(new_oldpwd, &env);
}


void	update_pwd(char **env)
{
	int		i;
	char	*cwd;
	char	*new_pwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		write(2, "cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n", 108);
		set_status(1);
		return ;
	}
	new_pwd = ft_strjoin("PWD=", cwd);
	free(cwd);
	if (!new_pwd)
		return ;
	i = 0;
	while (env[i])
	{
		if (strncmp(env[i], "PWD=", 4) == 0)
		{
			free(env[i]);
			env[i] = new_pwd;
			return ;
		}
		i++;
	}
	add_env_variable(new_pwd, &env);
}


void	cd(char *cmd, char **env)
{
	char	*target;

	if (!cmd || ft_strcmp(cmd, "~") == 0)
	{
		target = get_env("HOME", env);
		if (!target)
			return (write(2, "cd: HOME not set\n", 18), (void)0);
	}
	else if (ft_strcmp(cmd, "-") == 0)
	{
		target = get_env("OLDPWD", env);
		if (!target)
			return (write(2, "cd: OLDPWD not set\n", 20), (void)0);
		printf("%s\n", target);
	}
	else
		target = cmd;

	if (chdir(target) == -1)
	{
		set_status(1);
		perror(target);
		return ;
	}
	update_oldpwd(env);
	update_pwd(env);
}
