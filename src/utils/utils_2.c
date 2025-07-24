/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:43:35 by sel-bech          #+#    #+#             */
/*   Updated: 2025/07/23 20:01:00 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	redirect_input(char *file, t_cmd *cmd)
{
	int	fd;

	if (cmd->infile_fd != -1 && !cmd->flag_amb)
	{
		fd = open(file, O_RDONLY);
		cmd->infile_fd = fd;
		if (fd < 0)
			perror("open infile");
		}
}

void	redirect_output(t_cmd *cmd, int append)
{
	int	fd;

	if (!cmd->flag_amb && cmd->infile_fd != -1 && cmd->outfile_fd != -1)
	{
		if (append)
			fd = open(cmd->outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
		else
			fd = open(cmd->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		cmd->outfile_fd = fd;
		if (fd < 0)
			perror("open outfile");
	}
}

	int	check_delimiter(char *line, char *delimiter)
{
	if (!line)
		return (1);
	if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		return (1);
	return (0);
}

void	write_expanded_line(char *line, t_shell *shell, int tmp_fd)
{
	int		i;
	char	*var;

	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			var = get_variable(shell, line, &i);
			if (var)
				write(tmp_fd, var, ft_strlen(var));
		}
		else
		{
			write(tmp_fd, &line[i], 1);
			i++;
		}
	}
	write(tmp_fd, "\n", 1);
}

void	run_heredoc_loop(int tmp_fd, char *delimiter, t_shell *shell)
{
	char	*line;

	signal(SIGINT, handel_sig);
	while (1)
	{
		line = readline("> ");
		if (check_delimiter(line, delimiter))
		{
			shell->exp = 1;
			clean_shell(shell);
			close(tmp_fd);
			exit(0);
		}
		if (ft_strchr(line, '$') && shell->exp)
			write_expanded_line(line, shell, tmp_fd);
		else
		{
			write(tmp_fd, line, ft_strlen(line));
			write(tmp_fd, "\n", 1);
		}
		free(line);
	}
}
