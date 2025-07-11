/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:34:17 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/11 20:40:05 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "libft.h"

static int	count_words(const char *str, char c)
{
	int	count;
	int	flag;

	count = 0;
	flag = 0;
	while (*str)
	{
		if (*str != c && flag == 0)
		{
			flag = 1;
			count++;
		}
		else if (*str == c)
			flag = 0;
		str++;
	}
	return (count);
}

char	*ft_substr2(const char *str, unsigned int start, size_t n)
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

// static void	free_split(char **lst)
// {
// 	int	i;

// 	i = 0;
// 	if (!lst)
// 		return ;
// 	while (lst[i])
// 	{
// 		free(lst[i]);
// 		i++;
// 	}
// 	free(lst);
// }

static void	fill(char const *s, char c, char **lst)
{
	size_t	word_len;
	int		i;

	i = 0;
	while (*s)
	{
		if (*s && *s != c)
		{
			word_len = ft_strlen(s);
			if (ft_strchr(s, c))
				word_len = ft_strchr(s, c) - s;
			lst[i] = ft_substr(s, 0, word_len);
			if (!lst[i])
			{
				free_split(lst);
				return ;
			}
			i++;
			s += word_len;
		}
		else
			s++;
	}
	lst[i] = NULL;
}

char	**ft_split(char const *s, char c)
{
	char	**lst;

	if (!s)
		return (NULL);
	lst = ft_malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!lst)
		return (NULL);
	fill(s, c, lst);
	return (lst);
}
