/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:09:40 by dkhalfao          #+#    #+#             */
/*   Updated: 2024/11/20 18:57:13 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	unsigned int	i;

	i = 0;
	if (size == 0)
		return (ft_strlen(src));
	while (i < size - 1 && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}

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

char	*ft_strdup(const char *str)
{
	size_t	size;
	char	*dest;

	size = ft_strlen(str);
	dest = malloc((sizeof(char) * size) + 1);
	if (!dest)
		return (NULL);
	ft_strlcpy(dest, str, size + 1);
	return (dest);
}

char	*ft_strjoin(char *s1, const char *s2)
{
	size_t	lens1;
	size_t	lens2;
	char	*res;

	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	lens1 = ft_strlen(s1);
	lens2 = ft_strlen(s2);
	res = malloc(lens1 + lens2 + 1);
	if (!res)
		return (NULL);
	ft_strlcpy(res, s1, ft_strlen(s1) + 1);
	ft_strlcat(res, s2, lens1 + lens2 + 1);
	return (res);
}

char	*ft_substr(const char *str, unsigned int start, size_t n)
{
	size_t	i;
	char	*dest;
	size_t	size;

	dest = NULL;
	if (!str)
		return (NULL);
	size = ft_strlen(str);
	if (start >= size)
		return (ft_strdup(""));
	if (start + n > size)
		n = size - start;
	dest = malloc(n + 1);
	if (!dest)
		return (NULL);
	i = 0;
	while (str[start] && i < n)
	{
		dest[i] = str[start];
		i++;
		start++;
	}
	dest[i] = '\0';
	return (dest);
}
