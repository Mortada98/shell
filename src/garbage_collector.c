#include "../include/minishell.h"

t_gc	*gc_init(void)
{
	t_gc	*gc;

	gc = malloc(sizeof(t_gc));
	if (!gc)
		return (NULL);
	gc->allocations = NULL;
	gc->count = 0;
	return (gc);
}

t_gc	**gc_get(void)
{
	static t_gc	*g_gc = NULL;

	return (&g_gc);
}
void	*gc_malloc(size_t size)
{
	void		*ptr;
	t_gc_node	*new_node;
	t_gc		**g_gc;

	g_gc = gc_get();
	if (!*g_gc)
	{
		*g_gc = gc_init();
		if (!*g_gc)
			return (NULL);
	}
	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	new_node = malloc(sizeof(t_gc_node));
	if (!new_node)
	{
		free(ptr);
		return (NULL);
	}
	new_node->ptr = ptr;
	new_node->next = (*g_gc)->allocations;
	(*g_gc)->allocations = new_node;
	(*g_gc)->count++;
	return (ptr);
}

char	*gc_strdup(const char *s)
{
	char	*new_str;
	size_t	len;
	size_t	i;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	new_str = gc_malloc(len + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_str[i] = s[i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

void	*gc_calloc(size_t count, size_t size)
{
	void	*ptr;
	size_t	total_size;

	total_size = count * size;
	ptr = gc_malloc(total_size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, total_size);
	return (ptr);
}

char	*gc_substr(char const *s, unsigned int start, size_t len)
{
	char	*new_str;
	size_t	s_len;
	size_t	i;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (gc_strdup(""));
	if (len > s_len - start)
		len = s_len - start;
	new_str = gc_malloc(len + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_str[i] = s[start + i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

char	*gc_strjoin(char const *s1, char const *s2)
{
	char	*new_str;
	size_t	len1;
	size_t	len2;
	size_t	i;
	size_t	j;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (gc_strdup(s2));
	if (!s2)
		return (gc_strdup(s1));
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	new_str = gc_malloc(len1 + len2 + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < len1)
	{
		new_str[i] = s1[i];
		i++;
	}
	j = 0;
	while (j < len2)
	{
		new_str[i + j] = s2[j];
		j++;
	}
	new_str[i + j] = '\0';
	return (new_str);
}

void	gc_free(void *ptr)
{
	t_gc_node	*current;
	t_gc_node	*prev;
	t_gc		**g_gc;

	g_gc = gc_get();
	if (!ptr || !g_gc)
		return ;
	current = (*g_gc)->allocations;
	prev = NULL;
	while (current)
	{
		if (current->ptr == ptr)
		{
			if (prev)
				prev->next = current->next;
			else
				(*g_gc)->allocations = current->next;
			free(current->ptr);
			free(current);
			(*g_gc)->count--;
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	gc_cleanup(void)
{
	t_gc_node	*current;
	t_gc_node	*next;
	t_gc		**g_gc;

	g_gc = gc_get();
	if (!*g_gc)
		return ;
	current = (*g_gc)->allocations;
	while (current)
	{
		next = current->next;
		free(current->ptr);
		free(current);
		current = next;
	}
	free(*g_gc);
	*g_gc = NULL;
}

void	gc_cleanup_partial(void)
{
	t_gc_node	*current;
	t_gc_node	*next;
	t_gc		**g_gc;

	g_gc = gc_get();
	if (!g_gc)
		return ;
	current = (*g_gc)->allocations;
	while (current)
	{
		next = current->next;
		free(current->ptr);
		free(current);
		current = next;
	}
	(*g_gc)->allocations = NULL;
	(*g_gc)->count = 0;
}


void	gc_register_external(void *ptr)
{
	t_gc_node	*new_node;
	t_gc		**g_gc;

	if (!ptr)
		return ;
	g_gc = gc_get();
	if (!*g_gc)
	{
		*g_gc = gc_init();
		if (!*g_gc)
			return ;
	}
	new_node = malloc(sizeof(t_gc_node));
	if (!new_node)
		return ;
	new_node->ptr = ptr;
	new_node->next = (*g_gc)->allocations;
	(*g_gc)->allocations = new_node;
	(*g_gc)->count++;
}
