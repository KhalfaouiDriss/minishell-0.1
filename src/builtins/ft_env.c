#include "../../include/minishell.h"

void ft_env(t_env *env, int is_export)
{
    while (env)
    {
        if (is_export)
        {
            if (env->value)
                printf("declare -x %s=\"%s\"\n", env->name, env->value);
            else
                printf("declare -x %s\n", env->name);
        }
        else
        {
            if (env->value)
                printf("%s=%s\n", env->name, env->value);
            else
                printf("%s\n", env->name);
        }
        // if (env->value)
        //     printf("%s=%s\n", env->name, env->value);
        // else
        //     printf("%s\n", env->name);
        env = env->next;
    }
}