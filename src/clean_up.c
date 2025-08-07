/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouizak <mbouizak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 22:37:59 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/07 14:12:43 by mbouizak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	if (!g_gc || !*g_gc)
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
	if (!g_gc || !*g_gc)
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
	t_gc		**g_gc;
	t_gc_node	*new_node;

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
