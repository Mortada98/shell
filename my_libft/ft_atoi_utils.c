/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfatih <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 11:20:46 by helfatih          #+#    #+#             */
/*   Updated: 2025/07/22 11:20:53 by helfatih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	handle_condition(int i, char *str, int *len1, int *len2)
{
	if ((str[i - 1] - '0') > 8)
		(*len1)++;
	if ((str[i - 1] - '0') < 8)
		(*len2)++;
}

static int	long_min(char *str, bool *flags, int i)
{
	long	res;

	int (len1), len2;
	res = 922337203685477580;
	len1 = 0;
	len2 = 0;
	handle_condition(i, str, &len1, &len2);
	i--;
	while (i > 0)
	{
		handle_condition(i, str, &len1, &len2);
		res = res / 10;
		i--;
	}
	if (len1 > 0 && len2 == 0)
		return (2);
	if (len2 > 0)
		return (0);
	*flags = true;
	return (0);
}

static int	handle_exit_status(char *str, int sign, bool *flags_sign)
{
	int	ret_val;
	int	count;

	count = counter(str);
	if (count == 19 || count > 19)
	{
		if (sign == 1)
		{
			ret_val = long_max(str, count);
			return (ret_val);
		}
		if (sign == -1)
		{
			ret_val = long_min(str, flags_sign, count);
			return (ret_val);
		}
	}
	return (0);
}

int	fix_norm(int i, char *str, int sign, bool *flags_sign)
{
	int	size;

	if (str[i] == '9' || counter(&str[i]) > 19)
	{
		size = handle_exit_status(&str[i], sign, flags_sign);
		if (size != 0)
		{
			if (*flags_sign)
				return (0);
			printf("minishell: exit: %s: numeric argument required\n", str);
			return (size);
		}
	}
	return (0);
}

void	atoi_utils(int *i, char *str, int *sign)
{
	while (str[*i] == '0' || str[*i] == ' ' || ('\t' <= str[*i]
			&& str[*i] <= '\r'))
		(*i)++;
	if (str[*i] == '+')
		(*i)++;
	else if (str[*i] == '-')
	{
		*sign *= -1;
		(*i)++;
	}
}
