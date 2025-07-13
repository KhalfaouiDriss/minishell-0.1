/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 20:36:39 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/13 20:40:47 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	add_env(t_shell *shell, char *name, char *value)
{
	t_env	*new_env;
	t_env	*last;

	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return ;
	new_env->name = ft_strdupv2(name);
	if (value)
		new_env->value = ft_strdupv2(value);
	else
		new_env->value = NULL;
	new_env->next = NULL;
	if (!shell->env)
		shell->env = new_env;
	else
	{
		last = shell->env;
		while (last->next)
			last = last->next;
		last->next = new_env;
	}
}

void	process_env_variable(t_shell *shell, char *env_str)
{
	char	*egl;
	char	*name;
	char	*value;

	egl = ft_strchr(env_str, '=');
	if (!egl)
		return ;
	name = ft_substr(env_str, 0, egl - env_str);
	value = egl + 1;
	if (!name)
		return ;
	add_env(shell, name, value);
}

char	*ft_strjoin2(const char *s1, const char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*res;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	res = malloc(len1 + len2 + 1);
	if (!res)
		return (NULL);
	ft_strlcpy(res, s1, len1 + 1);
	ft_strlcat(res, s2, len1 + len2 + 1);
	return (res);
}

void	init_new_env(t_shell *shell)
{
	int		i;
	t_env	*tmp;
	char	*temp1;
	char	*temp2;

	i = 0;
	tmp = shell->env;
	while (tmp)
	{
		temp1 = ft_strjoin2(tmp->name, "=");
		if (tmp->value)
		{
			temp2 = ft_strjoin2(temp1, tmp->value);
			shell->new_env[i] = ft_strdupv2(temp2);
			free(temp2);
		}
		else
			shell->new_env[i] = ft_strdupv2(temp1);
		free(temp1);
		i++;
		tmp = tmp->next;
	}
	shell->new_env[i] = NULL;
}

void	init_env(t_shell *shell, char **envp)
{
	int	i;

	i = 0;
	shell->env = NULL;
	while (envp[i])
	{
		process_env_variable(shell, envp[i]);
		i++;
	}
	shell->new_env = malloc(sizeof(char *) * (i + 1));
	init_new_env(shell);
}
