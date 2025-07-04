/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 08:40:18 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/04 13:52:05 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../../include/minishell.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	size_t	lens1;
	size_t	lens2;
	char	*res;

	if (!s1 || !s2)
		return (NULL);
	lens1 = ft_strlen(s1);
	lens2 = ft_strlen(s2);
	res = ft_malloc(lens1 + lens2 + 1);
	if (!res)
		return (NULL);
	ft_strlcpy(res, s1, ft_strlen(s1) + 1);
	ft_strlcat(res, s2, lens1 + lens2 + 1);
	return (res);
}
