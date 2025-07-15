/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 20:36:27 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/15 15:34:30 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*gen_random(void)
{
	char	*tmp;
	int		i;
	int		fd;

	tmp = ft_malloc(6);
	i = 0;
	fd = open("/dev/random", O_RDONLY);
	while (i < 5)
	{
		read(fd, &tmp[i], 1);
		if (tmp[i] >= 32 && tmp[i] <= 126 && tmp[i] != '/')
			i++;
	}
	close(fd);
	tmp[i] = '\0';
	return (tmp);
}

static int	open_heredoc_file(char **path)
{
	char	*base;
	char	*rand;

	base = "/tmp/.heredoc_tmp";
	rand = gen_random();
	*path = ft_strjoin(base, rand);
	return (open(*path, O_CREAT | O_WRONLY | O_TRUNC, 0600));
}

static void	handle_fork_child(int fd, char *delimiter, t_shell *shell)
{
	run_heredoc_loop(fd, delimiter, shell);
	close(fd);
	clean_shell(shell);
	exit(shell->exit_status);
}

static int	check_exit_status(int status, t_shell *shell)
{
	if (WIFEXITED(status) && WEXITSTATUS(status) == 2)
	{
		shell->exit_status = 130;
		signal(SIGINT, get_sig);
		return (-1);
	}
	return (0);
}

int	handle_heredoc(char *delimiter, t_shell *shell)
{
	char	*path;
	int		tmp_fd;
	pid_t	pid;
	int		status;

	tmp_fd = open_heredoc_file(&path);
	shell->her_fd = tmp_fd;
	if (tmp_fd == -1)
		return (perror("open"), -1);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
		return (signal(SIGINT, get_sig), close(tmp_fd), perror("fork"), -1);
	if (pid == 0)
		handle_fork_child(tmp_fd, delimiter, shell);
	close(tmp_fd);
	waitpid(pid, &status, 0);
	if (check_exit_status(status, shell) == -1)
		return (-1);
	signal(SIGINT, get_sig);
	tmp_fd = open(path, O_RDONLY);
	if (tmp_fd == -1)
		perror("open heredoc read");
	return (tmp_fd);
}
