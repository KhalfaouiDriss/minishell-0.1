#include "../../include/minishell.h"

void ft_env(t_env *env)
{
    while (env)
    {
        if (env->value)
            printf("%s=%s\n", env->name, env->value);
        else
            printf("%s\n", env->name);
        env = env->next;
    }
}