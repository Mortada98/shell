/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfatih <helfatih@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 00:44:51 by tchevrie          #+#    #+#             */
/*   Updated: 2025/08/04 18:15:40 by helfatih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(char *str)
{
	long double	nbr;
	int			sign;
	int			i;

	nbr = 0;
	sign = 1;
	i = 0;
	if (str[0] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[0] == '+')
		i++;
	while ('0' <= str[i] && str[i] <= '9')
	{
		if (nbr > LONG_MAX || (nbr * sign) < LONG_MIN)
		{
			printf("minishell: exit: %s: numeric argument required\n", str);
			return (2);
		}
		nbr = nbr * 10 + str[i] - '0';
		i++;
	}
	return (nbr * sign);
}
