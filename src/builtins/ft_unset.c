/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:38:03 by sel-bech          #+#    #+#             */
/*   Updated: 2025/07/12 22:04:30 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_unset(t_env **env, char **args)
{
	int		i;
	t_env	*curr;
	t_env	*prev;
	t_env	*tmp;

	i = 1;
	while (args[i])
	{
		curr = *env;
		prev = NULL;
		while (curr)
		{
			if (ft_strncmp(curr->name, args[i], ft_strlen(args[i]) + 1) == 0)
			{
				tmp = curr;
				if (prev)
					prev->next = curr->next;
				else
					*env = curr->next;
				free(tmp->name);
				free(tmp->value);
				free(tmp);
				break ;
			}
			prev = curr;
			curr = curr->next;
		}
		i++;
	}
}

