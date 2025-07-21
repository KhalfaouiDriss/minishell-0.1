/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 17:44:28 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/07 15:29:12 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../../include/minishell.h"

char	*ft_substr(const char *str, unsigned int start, size_t n)
{
	size_t	i;
	char	*dest;
	size_t	size;

	if (!str)
		return (NULL);
	size = ft_strlen(str);
	if (start >= size)
		return (ft_strdup(""));
	if (start + n > size)
		n = size - start;
	dest = ft_malloc(n + 1);
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
