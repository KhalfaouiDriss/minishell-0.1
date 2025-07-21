/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 11:27:39 by sel-bech          #+#    #+#             */
/*   Updated: 2025/07/20 10:53:52 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_exit_error(t_shell *shell, char **args, char *trimmed)
{
	(void)shell;
	(void)args;
	free(trimmed);
	return (write(2, "exit\n", 5), write(2,
			"minishell: exit: too many arguments\n", 36),
		shell->exit_status = 1, 1);
}

void	exit_success(t_shell *shell, long long code)
{
	close_fd_bin(shell->in, shell->out);
	printf("exit\n");
	clean_shell(shell);
	exit((unsigned char)code);
}
