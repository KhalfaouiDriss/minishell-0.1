/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 21:37:44 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/19 13:26:44 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

const char	*get_target_dir(t_shell *shell, char **args)
{
	const char	*target_dir;

	if (!args[1])
	{
		target_dir = get_env_value(shell->env, "HOME");
		if (!target_dir)
		{
			write(2, "cd: HOME not set\n", 17);
			shell->exit_status = 1;
			return (NULL);
		}
	}
	else
		target_dir = args[1];
	return (target_dir);
}

void	handle_cd_oldpwd_failure(t_shell *shell)
{
	char	*fallback;
	int		attempts;
	char	*tmp;

	fallback = get_env_value(shell->env, "PWD");
	attempts = 10;
	if (chdir(fallback) == -1 && attempts--)
	{
		tmp = fallback;
		fallback = ft_strjoin(tmp, "/..");
	}
	if (fallback)
		update_env_var(shell->env, "PWD", fallback);
}

int	check_cd_errors(t_shell *shell, const char *target_dir)
{
	struct stat	sb;

	if (stat(target_dir, &sb) == 0)
	{
		if (S_ISDIR(sb.st_mode))
		{
			if (!(sb.st_mode & S_IXUSR))
			{
				write(2, "Permission denied\n", 19);
				shell->exit_status = 1;
				return (1);
			}
		}
		else
		{
			write(2, "Not a directory\n", 17);
			shell->exit_status = 1;
			return (1);
		}
	}
	return (0);
}

void	handle_cd_chdir_fail(t_shell *shell, const char *target_dir)
{
	char	*buffer;

	buffer = ft_strjoin(target_dir, ": No such file or directory\n");
	write(2, buffer, ft_strlen(buffer));
	shell->exit_status = 1;
}

void	update_pwd_after_cd(t_shell *shell)
{
	char	*newpwd;

	newpwd = getcwd(NULL, 0);
	if (!newpwd)
	{
		perror("cd: getcwd after chdir");
		shell->exit_status = 1;
		return ;
	}
	update_env_var(shell->env, "PWD", newpwd);
	free(newpwd);
}
