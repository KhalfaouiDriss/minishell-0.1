/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 21:41:04 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/15 14:33:45 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_env_value(t_env *env, const char *name)
{
	while (env)
	{
		if (ft_strncmp(env->name, name, ft_strlen(name)) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	add_new_env_var(t_env *env, const char *name, const char *value)
{
	t_env	*new_var;
	t_env	*tmp;

	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return ;
	new_var->name = ft_strdupv2(name);
	if (value)
		new_var->value = ft_strdupv2(value);
	else
		new_var->value = NULL;
	new_var->next = NULL;
	tmp = env;
	if (!tmp)
		return ;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_var;
}

void	update_env_var(t_env *env, const char *name, const char *value)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strncmp(tmp->name, name, ft_strlen(name)) == 0)
		{
			if (tmp->value)
				free(tmp->value);
			if (value)
				tmp->value = ft_strdupv2(value);
			else
				tmp->value = NULL;
			return ;
		}
		tmp = tmp->next;
	}
	add_new_env_var(env, name, value);
}

void	handle_cd_args_error(t_shell *shell)
{
	write(2, "cd: too many arguments\n", 24);
	shell->exit_status = 1;
}

void	ft_cd(t_shell *shell, char **args)
{
	const char	*target_dir;
	char		*oldpwd;

	if (args[1] && args[2])
		return (handle_cd_args_error(shell));
	target_dir = get_target_dir(shell, args);
	if (!target_dir)
		return ;
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		handle_cd_oldpwd_failure(shell);
	if (check_cd_errors(shell, target_dir))
	{
		free(oldpwd);
		return ;
	}
	if (chdir(target_dir) == -1)
		return (handle_cd_chdir_fail(shell, target_dir, oldpwd));
	update_env_var(shell->env, "OLDPWD", oldpwd);
	free(oldpwd);
	update_pwd_after_cd(shell);
	shell->exit_status = 0;
}
