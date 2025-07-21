/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 21:58:32 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/13 22:09:00 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_strdupv2(const char *str)
{
	char	*dest;
	size_t	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	dest = malloc(len + 1);
	if (!dest)
		return (NULL);
	ft_strlcpy(dest, str, len + 1);
	return (dest);
}

void	free_new_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return ;
	while (env[i])
		free(env[i++]);
	free(env);
}

t_env	*init_new_node(char *value, char *key)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->name = ft_strdupv2(key);
	if (value)
		new_node->value = ft_strdupv2(value);
	else
		new_node->value = NULL;
	new_node->next = NULL;
	return (new_node);
}

void	add_env_node(t_shell *shell, t_env **env, char *key, char *value)
{
	t_env	*new_node;
	t_env	*tmp;
	int		count;

	new_node = init_new_node(value, key);
	count = 0;
	if (!new_node)
		return ;
	if (!*env)
		*env = new_node;
	else
	{
		tmp = *env;
		while (tmp->next)
		{
			tmp = tmp->next;
			count++;
		}
		tmp->next = new_node;
	}
	free_new_env(shell->new_env);
	shell->new_env = malloc(sizeof(char *) * (count + 3));
	init_new_env(shell);
}
