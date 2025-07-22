/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:24:35 by sel-bech          #+#    #+#             */
/*   Updated: 2025/07/21 16:32:30 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_builtin_redirs(t_cmd *cmd, t_shell *shell)
{
	int	in;
	int	out;

	if (cmd->c_flag == 1 || cmd->flag_amb == 1 || cmd->infile_fd == -1
		|| cmd->outfile_fd == -1)
		return (shell->exit_status = 1, (void)0);
	in = -1;
	out = -1;
	in = dup(0);
	out = dup(1);
	if (cmd->infile_fd > 2)
		(dup2(cmd->infile_fd, 0), close(cmd->infile_fd));
	if (cmd->outfile_fd > 2)
		(dup2(cmd->outfile_fd, 1), close(cmd->outfile_fd));
	if (cmd->heredoc_fd != -1)
		close(cmd->heredoc_fd);
	shell->in = in;
	shell->out = out;
	shell->exit_status = execute_builtin(shell, cmd->args[0], cmd->args);
	dupping(in, out);
}

static void	handle_child(t_cmd *cmd, t_shell *shell, int prev_pipe, int *fd)
{
	char	*path;

	handle_signals_and_exit_cases(shell, cmd, prev_pipe, fd);
	if (cmd->next)
		(dup2(fd[1], 1), close(fd[1]), close(fd[0]));
	if (prev_pipe != -1)
		(dup2(prev_pipe, 0), close(prev_pipe));
	if (!cmd->args[0])
		(close_all(shell->cmd_list, cmd), exit(clean_exit(cmd, shell, 0)));
	if (cmd->heredoc_fd > 2)
		(dup2(cmd->heredoc_fd, 0), close(cmd->heredoc_fd));
	if (cmd->outfile_fd > 2)
		(dup2(cmd->outfile_fd, 1), close(cmd->outfile_fd));
	if (cmd->infile_fd > 2)
		(dup2(cmd->infile_fd, 0), close(cmd->infile_fd));
	if (is_builtin(cmd->args[0]))
		(close_all(shell->cmd_list, cmd), exit(builtin_free_exit(shell, cmd)));
	path = find_command_path(cmd->args[0], shell->env);
	handle_exec_errors(path, cmd, shell);
	execve(path, cmd->args, shell->new_env);
	execve_fail(cmd);
}

static void	exec_loop(t_shell *shell)
{
	int		fd[2];
	int		prev_pipe;
	int		pid;
	t_cmd	*cmd;

	cmd = shell->cmd_list;
	prev_pipe = -1;
	if (is_builtin(cmd->args[0]) && !cmd->next)
		return (handle_builtin_redirs(cmd, shell), (void)0);
	while (cmd)
	{
		if (cmd->next && pipe(fd) == -1)
			return (perror("pipe error"), clean_shell(shell), (void)0);
		if (cmd->next == NULL)
			close_all(shell->cmd_list, cmd);
		pid = fork();
		if (pid == 0)
			handle_child(cmd, shell, prev_pipe, fd);
		close_parent_fds(cmd, prev_pipe);
		if (cmd->next)
		{
			close(fd[1]);
			prev_pipe = fd[0];
		}
		cmd = cmd->next;
	}
	wait_all(pid, shell);
}

void	wait_all(int last_pid, t_shell *shell)
{
	int	status;
	int	sigint;
	int	sigquit;
	int	pid;
	int	sig;

	sigint = 0;
	sigquit = 0;
	pid = waitpid(-1, &status, 0);
	while (pid != -1)
	{
		if (WIFEXITED(status) && pid == last_pid)
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			if (pid == last_pid)
				shell->exit_status = 128 + WTERMSIG(status);
			sig = WTERMSIG(status);
			sig_val(sig, &sigquit, &sigint);
		}
		pid = waitpid(-1, &status, 0);
	}
	affiche_sig(sigquit, sigint);
}


void	execute_pipeline(t_shell *shell)
{
	t_cmd	*cmd;

	cmd = shell->cmd_list;
	while (cmd)
	{
		if (cmd->flag_amb == 1)
		{
			if (cmd->infile_fd != -1 && cmd->outfile_fd != -1)
				handle_ambiguous(shell);
		}
		cmd = cmd->next;
	}
	exec_loop(shell);
	close_all(shell->cmd_list, cmd);
}
