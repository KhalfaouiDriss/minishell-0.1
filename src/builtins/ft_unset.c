#include "../../include/minishell.h"

void ft_unset(t_env **env, char **args)
{
    int i = 1;

    while (args[i])
    {
        t_env *curr = *env;
        t_env *prev = NULL;

        while (curr)
        {
            if (ft_strncmp(curr->name, args[i], ft_strlen(args[i])) == 0)
            {
                if (prev)
                    prev->next = curr->next;
                else
                    *env = curr->next;
                break;
            }
            prev = curr;
            curr = curr->next;
        }
        i++;
    }
}
