/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:38:03 by sel-bech          #+#    #+#             */
/*   Updated: 2025/07/13 21:49:30 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	remove_env_node(t_env **env, t_env *node, t_env *prev)
{
	if (prev)
		prev->next = node->next;
	else
		*env = node->next;
	free(node->name);
	free(node->value);
	free(node);
}

void	unset_single_var(t_env **env, char *key)
{
	t_env	*curr;
	t_env	*prev;

	curr = *env;
	prev = NULL;
	while (curr)
	{
		if (ft_strncmp(curr->name, key, ft_strlen(key) + 1) == 0)
		{
			remove_env_node(env, curr, prev);
			break ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	ft_unset(t_env **env, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		unset_single_var(env, args[i]);
		i++;
	}
}
