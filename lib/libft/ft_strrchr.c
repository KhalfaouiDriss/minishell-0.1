/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 08:42:50 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/06/14 14:44:32 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	int		i;
	char	*res;

	i = 0;
	res = NULL;
	while (str[i])
	{
		if (str[i] == (unsigned char)c)
			res = (char *)&str[i];
		i++;
	}
	if (c == '\0')
	{
		return ((char *)&str[i]);
	}
	return (res);
}
