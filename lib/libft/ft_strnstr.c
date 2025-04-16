/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 08:42:36 by dkhalfao          #+#    #+#             */
/*   Updated: 2024/11/17 11:27:35 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str, const char *TF, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (TF[i] == '\0')
		return ((char *)str);
	while (i < len && str[i])
	{
		while (str[i + j] == TF[j] && i + j < len)
		{
			if (!TF[j + 1])
				return ((char *) &str[i]);
			j++;
		}
		j = 0;
		i++;
	}
	return (0);
}
