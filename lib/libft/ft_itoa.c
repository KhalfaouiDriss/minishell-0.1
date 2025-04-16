/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 08:30:22 by dkhalfao          #+#    #+#             */
/*   Updated: 2024/11/13 18:44:36 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	int_len(int nbr)
{
	int	i;

	i = 0;
	if (nbr == 0)
		return (1);
	if (nbr < 0)
		i++;
	while (nbr != 0)
	{
		nbr /= 10;
		i++;
	}
	return (i);
}

static char	*norm_func(char *str, long i)
{
	int		j;
	int		l;
	char	tmp;

	j = 0;
	if (str[j] == '-')
		j++;
	while (i != 0)
	{
		str[j++] = i % 10 + '0';
		i /= 10;
	}
	str[j] = '\0';
	j -= 1;
	l = 0;
	if (str[0] == '-')
		l++;
	while (l < j)
	{
		tmp = str[l];
		str[l++] = str[j];
		str[j--] = tmp;
	}
	return (str);
}

char	*ft_itoa(int nbr)
{
	char	*res;
	long	r;

	res = malloc(sizeof(char) * (int_len(nbr) + 1));
	if (!res)
		return (0);
	r = nbr;
	if (nbr == 0)
		return (ft_strdup("0"));
	if (nbr < 0)
	{
		res[0] = '-';
		r *= -1;
	}
	return (norm_func(res, r));
}
