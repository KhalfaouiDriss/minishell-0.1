/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_util3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 09:49:42 by sel-bech          #+#    #+#             */
/*   Updated: 2025/07/20 10:49:16 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	dupping2(int fd, int a)
{
	dup2(fd, a);
	close(fd);
	fd = -1;
}

void	dupping(int in, int out)
{
	dup2(in, 0);
	close(in);
	dup2(out, 1);
	close(out);
}

void	handle_pipes_and_fds(t_cmd *cmd, int prev_pipe, int *fd)
{
	if (cmd->next)
	{
		dupping2(fd[1], 1);
		close(fd[0]);
	}
	if (prev_pipe != -1)
		dupping2(prev_pipe, 0);
	if (cmd->heredoc_fd != -1)
		dupping2(cmd->heredoc_fd, 0);
	if (cmd->outfile_fd)
	{
		dup2(cmd->outfile_fd, 1);
		close(cmd->outfile_fd);
	}
}

void	execve_fail(t_cmd *cmd)
{
	t_shell	*shell;

	perror(cmd->args[0]);
	shell = get_shell();
	clean_shell(shell);
	exit(126);
}

int	builtin_free_exit(t_shell *shell, t_cmd *cmd)
{
	int	n;

	n = 0;
	n = execute_builtin(shell, cmd->args[0], cmd->args);
	clean_shell(shell);
	return (n);
}
