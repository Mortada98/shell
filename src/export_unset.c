#include "../include/minishell.h"

void print_export_env(char **env)
{
	int i, j, count = 0;
	char **sorted_env;
	char *temp;
	char *equal_pos;
	
	// Count environment variables
	while (env[count])
		count++;
	
	// Create a copy for sorting
	sorted_env = malloc(sizeof(char *) * (count + 1));
	if (!sorted_env)
		return;
	
	// Copy environment variables
	for (i = 0; i < count; i++)
	{
		sorted_env[i] = ft_strdup(env[i]);
		if (!sorted_env[i])
		{
			// Free already allocated strings on failure
			for (j = 0; j < i; j++)
				free(sorted_env[j]);
			free(sorted_env);
			return;
		}
	}
	sorted_env[count] = NULL;
	
	// Sort the environment variables (simple bubble sort)
	for (i = 0; i < count - 1; i++)
	{
		for (j = 0; j < count - i - 1; j++)
		{
			if (ft_strcmp(sorted_env[j], sorted_env[j + 1]) > 0)
			{
				temp = sorted_env[j];
				sorted_env[j] = sorted_env[j + 1];
				sorted_env[j + 1] = temp;
			}
		}
	}
	
	// Print in declare -x format
	for (i = 0; i < count; i++)
	{
		equal_pos = ft_strchr(sorted_env[i], '=');
		if (equal_pos)
		{
			*equal_pos = '\0';  // Temporarily split the string
			printf("declare -x %s=\"%s\"\n", sorted_env[i], equal_pos + 1);
			*equal_pos = '=';   // Restore the string
		}
		else
		{
			printf("declare -x %s\n", sorted_env[i]);
		}
		free(sorted_env[i]);
	}
	
	free(sorted_env);
}

int update_existing_var(char *name, char *value, char **env)
{
	int		i;
	size_t	len = ft_strlen(name);
	char	*new_entry;
	char	*tmp;

	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (0);
	new_entry = ft_strjoin(tmp, value);
	free(tmp);
	if (!new_entry)
		return (0);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
		{
			free(env[i]);
			env[i] = new_entry;
			return (1);
		}
		i++;
	}
	free(new_entry);
	return (0);
}

int is_valid_identifier(char *name)
{
	int i;

	if (!name || !*name)
		return (0);
	
	// First character must be letter or underscore
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	
	// Rest can be alphanumeric or underscore
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int export_without_value(char *name, char ***env)
{
	int i;
	size_t len = ft_strlen(name);
	char *new_var;
	char *tmp;
	
	// Check if variable already exists
	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], name, len) == 0 && 
			((*env)[i][len] == '=' || (*env)[i][len] == '\0'))
			return (1); // Already exists, do nothing
		i++;
	}
	
	// Add variable with empty value (like bash does)
	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (0);
	new_var = ft_strjoin(tmp, "");
	free(tmp);
	if (!new_var)
		return (0);
	add_env_variable(new_var, env);
	return (1);
}

int my_export(char *arg, char ***env)
{
	char	*equal_sign;
	char	*name;
	char	*value;
	char	*new_var;
	char	*tmp;

	if (!arg || !*arg)
	{
		print_export_env(*env);
		return (1);
	}
	
	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
	{
		// Export without value - just declare the variable
		if (!is_valid_identifier(arg))
		{
			write(2, "minishell: export: `", 20);
			write(2, arg, ft_strlen(arg));
			write(2, "': not a valid identifier\n", 26);
			set_status(1);
			return (0);
		}
		return (export_without_value(arg, env));
	}
	
	// Export with value
	name = ft_substr(arg, 0, equal_sign - arg);
	if (!name)
		return (0);
		
	if (!is_valid_identifier(name))
	{
		write(2, "minishell: export: `", 20);
		write(2, arg, ft_strlen(arg));
		write(2, "': not a valid identifier\n", 26);
		free(name);
		set_status(1);
		return (0);
	}
	
	value = ft_strdup(equal_sign + 1);
	if (!value)
		return (free(name), 0);
		
	if (!update_existing_var(name, value, *env))
	{
		tmp = ft_strjoin(name, "=");
		new_var = ft_strjoin(tmp, value);
		free(tmp);
		if (!new_var)
			return (free(name), free(value), 0);
		add_env_variable(new_var, env);
	}
	free(name);
	free(value);
	set_status(0);
	return (1);
}

int my_unset(char *name, char ***env)
{
	int		i = 0, j = 0;
	size_t	len = ft_strlen(name);
	char	**new_env;

	while ((*env)[i])
		i++;
	new_env = malloc(sizeof(char *) * i);
	if (!new_env)
		return (0);
	i = 0;
	while ((*env)[i])
	{
		if (!(ft_strncmp((*env)[i], name, len) == 0 && (*env)[i][len] == '='))
			new_env[j++] = (*env)[i];
		else
			free((*env)[i]);
		i++;
	}
	new_env[j] = NULL;
	free(*env);
	*env = new_env;
	return (1);
}

void	my_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
	}
	else
	{
		perror("pwd");
		set_status(1);
	}
}
