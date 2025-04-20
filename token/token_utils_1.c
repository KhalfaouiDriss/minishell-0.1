/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutaik <rboutaik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 16:10:36 by aachalla          #+#    #+#             */
/*   Updated: 2024/06/02 19:31:31 by rboutaik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_strncpy(char *dst, char *src, int size)
{
	int	i;

	i = 0;
	while (i < size && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

int	ft_strncmp(char *str_1, char *str_2, int size)
{
	int	i_dice;

	i_dice = -1;
	while (++i_dice < size)
		if (str_1[i_dice] != str_2[i_dice])
			return (0);
	return (1);
}

int	ft_strlen(char *str)
{
	int	size;

	size = 0;
	while (str && *(str++))
		size++;
	return (size);
}

int	ft_strcmp_delm(char *str, char *delem)
{
	int	i_dice;
	int	size;

	i_dice = 0;
	size = ft_strlen(delem);
	while (i_dice < size)
	{
		if (str[i_dice] != delem[i_dice])
			return (-1);
		i_dice++;
	}
	return (size);
}

char	**free_echoue(char **p, int i_final)
{
	int	i_dice;

	i_dice = 0;
	while (i_dice < i_final)
	{
		free(p[i_dice]);
		p[i_dice++] = NULL;
	}
	free(p);
	p = NULL;
	return (NULL);
}
