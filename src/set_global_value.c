/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_global_value.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfatih <helfatih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:58:13 by helfatih          #+#    #+#             */
/*   Updated: 2025/07/28 16:58:59 by helfatih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	*init_status(void)
{
	static int	var = 0;

	return (&var);
}

void	set_status(int val)
{
	int	*status;

	status = init_status();
	*status = val;
}

int	get_status(void)
{
	return (*init_status());
}