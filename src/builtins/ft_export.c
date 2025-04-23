#include "../../include/minishell.h"

void add_env_node(t_env **env, const char *key, const char *value)
{
    t_env *new_node = malloc(sizeof(t_env));
    t_env *tmp;

    if (!new_node)
        return;

    new_node->name = ft_strdup(key);
    new_node->value = ft_strdup(value);
    new_node->next = NULL;

    if (!*env)
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
    if (!args[1])
        return;

    char **kv = malloc(sizeof(char *) * 3);
    if (!kv)
        return;

    char *equal_sign = ft_strchr(args[1], '=');
    if (!equal_sign)
    {
        free(kv);
        return;
    }

    size_t key_len = equal_sign - args[1];
    kv[0] = ft_substr(args[1], 0, key_len);          
    kv[1] = ft_strdup(equal_sign + 1);               
    kv[2] = NULL;

    if (kv[0] && kv[1])
        add_env_node(env, kv[0], kv[1]);

    free_split(kv);
}



