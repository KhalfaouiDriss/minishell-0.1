/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_util1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 09:40:33 by sel-bech          #+#    #+#             */
/*   Updated: 2025/07/17 16:25:31 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*full_paths(char *path, char *cmd)
{
	int		total_size;
	char	*full_path;

	total_size = ft_strlen(path) + ft_strlen(cmd) + 2;
	full_path = ft_malloc(total_size);
	ft_strlcpy(full_path, path, total_size);
	ft_strlcat(full_path, "/", total_size);
	ft_strlcat(full_path, cmd, total_size);
	return (full_path);
}

char	*find_command_path(char *cmd, t_env *envp)
{
	char		**paths;
	char		*full_path;
	char		*envp_value;
	struct stat	sb;
	int			i;

	paths = NULL;
	full_path = NULL;
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	envp_value = find_env_node(envp, "PATH");
	if (!envp_value)
		return (NULL);
	paths = ft_split(envp_value, ':');
	i = 0;
	while (paths[i])
	{
		full_path = full_paths(paths[i], cmd);
		if (access(full_path, X_OK) == 0 && stat(full_path, &sb) == 0
			&& !S_ISDIR(sb.st_mode))
			return (full_path);
		i++;
	}
	return (NULL);
}

void	close_parent_fds(t_cmd *cmd, int prev_pipe)
{
	if (cmd->outfile_fd > 2)
	{
		close(cmd->outfile_fd);
		cmd->outfile_fd = -1;
	}
	if (cmd->heredoc_fd > 2)
	{
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	if (cmd->infile_fd > 2)
	{
		close(cmd->infile_fd);
		cmd->infile_fd = -1;
	}
	if (prev_pipe != -1)
		close(prev_pipe);
}

void	handle_ambiguous(t_shell *shell)
{
	shell->exit_status = 1;
	write(2, "ambiguous redirect\n", 20);
}

int	clean_exit(t_shell *shell, int fexit)
{
	clean_shell(shell);
	return (fexit);
}
