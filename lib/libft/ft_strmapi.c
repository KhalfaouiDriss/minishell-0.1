/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 08:42:11 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/04 13:52:23 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../../include/minishell.h"

char	*ft_strmapi(const char *str, char (*f)(unsigned int, char))
{
	size_t	i;
	char	*s;

	i = 0;
	if (!str)
		return (NULL);
	s = ft_malloc(ft_strlen(str) + 1);
	if (!s)
		return (0);
	while (str[i])
	{
		s[i] = f(i, str[i]);
		i++;
	}
	s[i] = '\0';
	return (s);
}
