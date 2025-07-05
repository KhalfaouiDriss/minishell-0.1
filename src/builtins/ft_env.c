#include "../../include/minishell.h"

void ft_env(t_env *env, int print_as_env)
{
    int i = 0;
    while (env)
    {
        if (print_as_env)
        {
            if (env->value)
                printf("[%d] : %s=%s\n", i, env->name, env->value);
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
        i++;
    }
}
