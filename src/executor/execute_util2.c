/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_util2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 09:45:02 by sel-bech          #+#    #+#             */
/*   Updated: 2025/07/26 21:24:51 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	sig_val(int sig, int *sigquit, int *sigint)
{
	if (sig == SIGQUIT)
		*sigquit = 1;
	else if (sig == SIGINT)
		*sigint = 1;
}

void	affiche_sig(int sigquit, int sigint)
{
	if (sigquit)
		ft_putstr_fd("Quit (core dumped)\n", 1);
	if (sigint)
		ft_putchar_fd('\n', 1);
}

void	print_not_found_and_exit(t_cmd *cmd, t_shell *shell)
{
	char	*buffer;

	if (ft_strncmp(cmd->args[0], "sudo", 5) == 0)
	{
		buffer = ft_strjoin(cmd->args[0], ": Permission denied\n");
		write(2, buffer, ft_strlen(buffer));
		close_all(shell->cmd_list, cmd);
		clean_shell(shell);
		exit(126);
	}
	if (!shell->is_dir)
		buffer = ft_strjoin(cmd->args[0], " : command not found\n");
	else if (shell->is_dir == 1)
		buffer = ft_strjoin(cmd->args[0], ": No such file or directory\n");
	write(2, buffer, ft_strlen(buffer));
	close_all(shell->cmd_list, cmd);
	clean_shell(shell);
	exit(127);
}

void	handle_exec_errors(char *path, t_cmd *cmd, t_shell *shell)
{
	struct stat	sb;

	if (!path)
		print_not_found_and_exit(cmd, shell);
	if (path)
	{
		if (stat(path, &sb) != 0)
		{
			write(2, path, ft_strlen(path));
			write(2, ": No such file or directory\n", 29);
			close_all(shell->cmd_list, cmd);
			clean_shell(shell);
			exit(127);
		}
		if (S_ISDIR(sb.st_mode))
		{
			write(2, path, ft_strlen(path));
			write(2, ": Is a directory\n", 18);
			close_all(shell->cmd_list, cmd);
			clean_shell(shell);
			exit(126);
		}
	}
}

void	handle_signals_and_exit_cases(t_shell *shell, t_cmd *cmd, int prev_pipe,
		int *fd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (cmd->flag_amb == 1)
	{
		if (prev_pipe != -1)
			close(prev_pipe);
		if (cmd->next)
		{
			close(fd[0]);
			close(fd[1]);
		}
		close_all(shell->cmd_list, cmd);
		clean_shell(shell);
		exit(1);
	}
}
