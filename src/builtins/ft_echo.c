/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:32:29 by sel-bech          #+#    #+#             */
/*   Updated: 2025/07/11 20:33:44 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_strchar(char *s, int f)
{
	int	len;

	len = ft_strlen(s);
	write(1, s, len);
	if (f == 1)
		write(1, " ", 1);
}

void	ft_echo(char **args)
{
	int	i;
	int	no_newline;
	int	j;

	i = 1;
	no_newline = 0;
	while (args[i] && args[i][0] == '-')
	{
		j = 1;
		while (args[i][j] == 'n')
			j++;
		if (args[i++][j] == '\0')
			no_newline = 1;
		else
			break ;
	}
	while (args[i])
	{
		ft_strchar(args[i], args[i + 1] != NULL);
		i++;
	}
	if (i == 1 || !no_newline)
		write(1, "\n", 1);
}
