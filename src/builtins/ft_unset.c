/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:38:03 by sel-bech          #+#    #+#             */
/*   Updated: 2025/07/11 20:38:04 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_unset(t_env **env, char **args)
{
	int		i;
	t_env	*curr;
	t_env	*prev;

	i = 1;
	while (args[i])
	{
		curr = *env;
		prev = NULL;
		while (curr)
		{
			if (ft_strncmp(curr->name, args[i], ft_strlen(args[i])) == 0)
			{
				if (prev)
					prev->next = curr->next;
				else
					*env = curr->next;
				break ;
			}
			prev = curr;
			curr = curr->next;
		}
		i++;
	}
}
