/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_util2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 09:45:02 by sel-bech          #+#    #+#             */
/*   Updated: 2025/07/11 11:45:35 by dkhalfao         ###   ########.fr       */
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
		ft_putstr_fd("Quit (core dumped)\n", 2);
	if (sigint)
		ft_putchar_fd('\n', 2);
}

void	print_not_found_and_exit(t_cmd *cmd, t_shell *shell)
{
	char	*buffer;

	buffer = ft_strjoin(cmd->args[0], " : command not found\n");
	write(2, buffer, ft_strlen(buffer));
	gc_free_all();
	free_env(shell->env);
	free_new_env(shell->new_env);
	exit(127);
}

void	handle_exec_errors(char *path, t_cmd *cmd, t_shell *shell)
{
	struct stat	sb;

	if (path && access(path, X_OK) == 0 && stat(path, &sb) == 0
		&& S_ISDIR(sb.st_mode))
	{
		write(1, path, ft_strlen(path));
		write(1, ": Is a directory\n", 18);
		gc_free_all();
		shell->exit_status = 127;
		exit(127);
	}
	if (!path)
		print_not_found_and_exit(cmd, shell);
}

void	handle_signals_and_exit_cases(t_shell *shell, t_cmd *cmd)
{
	signal(SIGQUIT, SIG_DFL);
	if (cmd->flag_amb == 1 && cmd->args[0] == NULL)
	{
		clean_shell(shell);
		exit(0);
	}
	if (cmd->flag_amb == 1 || cmd->outfile_fd == -1)
	{
		clean_shell(shell);	
		exit(1);
	}
}
