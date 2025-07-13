/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:35:16 by sel-bech          #+#    #+#             */
/*   Updated: 2025/07/13 22:10:37 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_env(t_env *env, int print_as_env)
{
	while (env)
	{
		if (print_as_env)
		{
			if (env->value)
				printf("%s=%s\n", env->name, env->value);
		}
		else
		{
			if (env->value)
			{
				printf("declare -x %s=\"%s\"\n", env->name, env->value);
			}
			else
			{
				printf("declare -x %s\n", env->name);
			}
		}
		env = env->next;
	}
}
