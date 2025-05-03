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
    int i = 1;

    if(args[i] == NULL)
    {
        ft_env(*env, 1);
        return;
    }
    else
    {
        while (args[i])
    {
        char *equal_sign = ft_strchr(args[i], '=');
        if (!equal_sign)
        {
            i++;
            continue;
        }

        size_t key_len = equal_sign - args[i];
        char *key = ft_substr(args[i], 0, key_len);
        char *value = ft_strdup(equal_sign + 1);

        if (key && value)
        {
            // تحقق إن كان المفتاح موجودًا مسبقًا
            t_env *tmp = *env;
            while (tmp)
            {
                if (ft_strncmp(tmp->name, key, ft_strlen(key) + 1) == 0)
                {
                    free(tmp->value);             // حذف القيمة القديمة
                    tmp->value = ft_strdup(value); // تعيين الجديدة
                    break;
                }
                tmp = tmp->next;
            }

            // إذا لم يوجد المفتاح، أضفه
            if (!tmp)
                add_env_node(env, key, value);
        }

        free(key);
        free(value);
        i++;
    }
    }
}





