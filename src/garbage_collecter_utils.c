/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collecter_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfatih <helfatih@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 22:35:07 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/03 22:40:07 by helfatih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	t_gc		**g_gc;
	t_gc_node	*new_node;

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
