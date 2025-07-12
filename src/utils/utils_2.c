/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:43:35 by sel-bech          #+#    #+#             */
/*   Updated: 2025/07/12 09:18:54 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	redirect_input(char *file, t_cmd *cmd)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror("open infile");
		return (1);
	}
	dup2(fd, 0);
	close(fd);
	return (0);
}

void	redirect_output(t_shell *shell, t_cmd *cmd, int append)
{
	int	fd;

	if (!cmd->flag_amb)
	{
		if (append)
			fd = open(cmd->outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
		else
			fd = open(cmd->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror("open outfile");
			cmd->outfile_fd = fd;
			return ;
		}
		cmd->outfile_fd = fd;
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
			if (ft_strchr(shell->input, '\''))
				var = handle_variable_token(line, &i, shell, '\'');
			else
				var = handle_variable_token(line, &i, shell, 0);
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

void	handel_sig(int sig)
{
	write(1, "\n", 1);
	gc_free_all();
	exit(2);
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
			clean_shell(shell);
			exit(0);
		}
		if (ft_strchr(line, '$'))
			write_expanded_line(line, shell, tmp_fd);
		else
		{
			write(tmp_fd, line, ft_strlen(line));
			write(tmp_fd, "\n", 1);
		}
		free(line);
	}
}
