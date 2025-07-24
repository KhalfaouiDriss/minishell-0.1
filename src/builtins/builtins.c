/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:30:32 by sel-bech          #+#    #+#             */
/*   Updated: 2025/07/24 13:41:26 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0 || ft_strncmp(cmd, "pwd", 4) == 0
		|| ft_strncmp(cmd, "cd", 3) == 0 || ft_strncmp(cmd, "exit", 5) == 0
		|| ft_strncmp(cmd, "export", 7) == 0 || ft_strncmp(cmd, "env", 4) == 0
		|| ft_strncmp(cmd, "unset", 6) == 0)
	{
		return (1);
	}
	return (0);
}

int	execute_builtin(t_shell *shell, char *cmd, char **args)
{

	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (ft_echo(args), 0);
	else if (ft_strncmp(cmd, "cd", 3) == 0)
		return (ft_cd(shell, args), shell->exit_status);
	else if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (ft_pwd(shell, args), 0);
	else if (ft_strncmp(cmd, "export", 6) == 0)
		return (ft_export(shell, args), shell->exit_status);
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		return (ft_unset(&(shell->env), args), 0);
	else if (ft_strncmp(cmd, "env", 3) == 0)
		return (ft_env(shell->env, 1), shell->exit_status);
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		return (ft_exit(shell, args));
	return (127);
}
