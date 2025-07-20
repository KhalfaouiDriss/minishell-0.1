/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:37:47 by sel-bech          #+#    #+#             */
/*   Updated: 2025/07/15 14:33:59 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_pwd(t_shell *shell, char **args)
{
	char	*dir;
	char	*pwd;

	(void)args;

	dir = getcwd(NULL, 0);
	if (!dir)
	{
		pwd = find_env_node(shell->env, "PWD");
		if (pwd)
		{
			printf("%s\n", pwd);
			shell->exit_status = 0;
		}
		else
		{
			printf("pwd: error retrieving working directory\n");
			shell->exit_status = 1;
		}
		return ;
	}
	printf("%s\n", dir);
	free(dir);
}
