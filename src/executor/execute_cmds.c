/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:24:35 by sel-bech          #+#    #+#             */
/*   Updated: 2025/07/20 18:07:51 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_builtin_redirs(t_cmd *cmd, t_shell *shell)
{
	int	in;
	int	out;

	if (cmd->c_flag == 1 || cmd->flag_amb == 1 || cmd->infile_fd == -1
		|| cmd->outfile_fd == -1)
		return (shell->exit_status);
	in = -1;
	out = -1;
	in = dup(0);
	out = dup(1);
	if (cmd->infile_fd != -1)
		(dup2(cmd->infile_fd, 0), close(cmd->infile_fd));
	if (cmd->outfile_fd != -1)
		(dup2(cmd->outfile_fd, 1), close(cmd->outfile_fd));
	if (cmd->heredoc_fd != -1)
		close(cmd->heredoc_fd);
	shell->in = in;
	shell->out = out;
	shell->exit_status = execute_builtin(shell, cmd->args[0], cmd->args);
	dupping(in, out);
	return (shell->exit_status);
}

void	dupping2(int fd, int a)
{
	dup2(fd, a);
	close(fd);
	fd = -1;
}

static void	handle_child(t_cmd *cmd, t_shell *shell, int prev_pipe, int *fd)
{
	char	*path;

	handle_signals_and_exit_cases(shell, cmd, prev_pipe, fd);
	if (cmd->next)
		(dupping2(fd[1], 1), close(fd[0]));
	if (cmd->infile_fd == -1 || cmd->outfile_fd == -1)
		exit(clean_exit(cmd, shell, 1));
	if (prev_pipe != -1)
		dupping2(prev_pipe, 0);
	if (cmd->heredoc_fd != -1)
		dupping2(cmd->heredoc_fd, 0);
	if (is_builtin(cmd->args[0]))
		(close_all(cmd), exit(builtin_free_exit(shell, cmd)));
	if (cmd->outfile_fd > 2)
		dupping2(cmd->outfile_fd, 1);
	if (cmd->infile_fd > 2)
		dupping2(cmd->infile_fd, 0);
	if (!cmd->args[0])
		exit(clean_exit(cmd, shell, 0));
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
			return (perror("pipe error"), gc_free_all(), (void)0);
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
	int	sig;

	sigint = 0;
	sigquit = 0;
	waitpid(last_pid, &status, 0);
	while (wait(NULL) > 0)
		;
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		shell->exit_status = 128 + WTERMSIG(status);
		sig = WTERMSIG(status);
		sig_val(sig, &sigquit, &sigint);
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
}
