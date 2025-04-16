/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 08:40:36 by dkhalfao          #+#    #+#             */
/*   Updated: 2024/11/17 11:08:50 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t n)
{
	unsigned int	i;
	unsigned int	k;
	unsigned int	dl;

	i = ft_strlen(dst);
	dl = i;
	k = 0;
	if (n && dl <= n)
	{
		while (src[k] != '\0' && i < (n - 1))
		{
			dst[i] = src[k];
			i++;
			k++;
		}
		dst[i] = '\0';
	}
	if (n == 0)
		dl = 0;
	if (n < dl)
		dl = n;
	return (dl + ft_strlen((char *)src));
}
