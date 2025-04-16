/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 08:30:32 by dkhalfao          #+#    #+#             */
/*   Updated: 2024/11/13 18:39:30 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *str, int c, size_t n)
{
	size_t				i;
	const unsigned char	*sr;

	sr = str;
	i = 0;
	while (i < n)
	{
		if (sr[i] == (unsigned char)c)
			return ((void *)str + i);
		i++;
	}
	return (NULL);
}
