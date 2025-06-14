#include "../../include/minishell.h"

void add_env_node(t_env **env, const char *key, const char *value)
{
    t_env *new_node = malloc(sizeof(t_env));
    t_env *tmp;
    
    if (!new_node)
        return;
    new_node->name = ft_strdup(key);
    new_node->value = NULL;
    if(value)
        new_node->value = ft_strdup(value);
    new_node->next = NULL;
    printf("+==============+\n");

    if (*env == NULL)
    {
        *env = new_node;
        return;
    }

    tmp = *env;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new_node;
}

void ft_export(t_env **env, char **args)
{
    int i = 1;
    char *key;
    t_env *tmp = *env;
    char *value;

    if(args[i] == NULL)
    {
        ft_env(*env, 0);
        return;
    }
    else
    {
        while (args[i])
        {
            if(!(args[i][0] >= 97 && args[i][0] <= 122) && !(args[i][0] >= 65 && args[i][0] <= 90) && args[i][0] != '_')
            {
                printf("minishell: export: %s not a valid identifier\n", args[i]);
                i++;
                continue;
            }
            char *equal_sign = ft_strchr(args[i], '=');
            if(equal_sign)
            {
                int key_len = equal_sign - args[i];
                key = ft_substr(args[i], 0, key_len);
                value = ft_strdup(equal_sign + 1);
            }
            else
            {
                key = ft_strdup(args[i]);
                value = NULL;
            }
            if (key)
            {
                while (tmp)
                {
                    if (ft_strncmp(tmp->name, key, ft_strlen(key) + 1) == 0)
                    {
                        if(value)
                        free(tmp->value);      
                        tmp->value = ft_strdup(value);
                        break;
                    }
                    tmp = tmp->next;
                }
                if (!tmp)
                    add_env_node(env, key, value);
            }
            free(key);
            if(value)
                free(value);
            i++;
        }
    }
}
