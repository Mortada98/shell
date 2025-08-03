/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 00:44:51 by tchevrie          #+#    #+#             */
/*   Updated: 2022/10/05 04:32:47 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	counter(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}

int	long_max(char *str, int i)
{
	long	res;

	int (len1), len2;
	res = 9223372036854775807;
	len1 = 0;
	len2 = 0;
	while (i > 0)
	{
		if ((str[i - 1] - '0') > res % 10)
			len1++;
		if ((str[i - 1] - '0') < res % 10)
			len2++;
		res = res / 10;
		i--;
	}
	if (len1 > 0 && len2 == 0)
		return (2);
	return (0);
}

int	ft_atoi(char *str)
{
	long	nbr;
	int		sign;
	int		i;
	int		size;
	bool	flags_sign;

	flags_sign = false;
	nbr = 0;
	sign = 1;
	i = 0;
	atoi_utils(&i, str, &sign);
	size = fix_norm(i, str, sign, &flags_sign);
	if (size != 0)
		return (size);
	if (flags_sign)
		return (0);
	while ('0' <= str[i] && str[i] <= '9')
	{
		nbr = nbr * 10 + str[i] - '0';
		i++;
	}
	return (nbr * sign);
}
