/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:35:16 by sel-bech          #+#    #+#             */
/*   Updated: 2025/07/11 20:35:17 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void ft_env(t_env *env, int print_as_env)
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
            else{
                printf("declare -x %s\n", env->name);
            }
        }
        env = env->next;
    }
}
