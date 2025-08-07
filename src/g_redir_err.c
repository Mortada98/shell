/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_redir_err.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfatih <helfatih@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 09:26:10 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/04 09:26:11 by helfatih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	*set_redir_error(void)
{
	static int	g_redir_error = 0;

	return (&g_redir_error);
}

void	reset_redir_error(int value)
{
	int	*g_redir_error;

	g_redir_error = set_redir_error();
	*g_redir_error = value;
}

int	get_redir_error(void)
{
	return (*set_redir_error());
}
