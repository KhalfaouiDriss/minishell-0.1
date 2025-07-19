/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 11:27:39 by sel-bech          #+#    #+#             */
/*   Updated: 2025/07/19 11:28:04 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_exit_error(t_shell *shell, char **args, char *trimmed)
{
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
