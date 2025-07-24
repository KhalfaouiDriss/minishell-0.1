/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_util3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 09:49:42 by sel-bech          #+#    #+#             */
/*   Updated: 2025/07/24 16:33:20 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	dupping(int in, int out)
{
	if (in > 2)
	{
		dup2(in, 0);
		close(in);
	}
	if (out > 2)
	{
		dup2(out, 1);
		close(out);
	}
}

void	execve_fail(t_cmd *cmd)
{
	t_shell	*shell;

	perror(cmd->args[0]);
	shell = get_shell();
	close_all(shell->cmd_list, cmd);
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

void	close_all(t_cmd *head, t_cmd *curr)
{
	t_cmd	*tmp;

	tmp = head;
	while (tmp)
	{
		if (tmp != curr)
		{
			if (tmp->infile_fd > 2)
				close(tmp->infile_fd);
			if (tmp->outfile_fd > 2)
				close(tmp->outfile_fd);
			if (tmp->heredoc_fd > 2)
				close(tmp->heredoc_fd);
		}
		tmp = tmp->next;
	}
}

void	close_no_args(t_cmd *cmd)
{
	if (cmd->infile_fd > 2)
		close(cmd->infile_fd);
	if (cmd->outfile_fd > 2)
		close(cmd->outfile_fd);
	if (cmd->heredoc_fd > 2)
		close(cmd->heredoc_fd);
}
