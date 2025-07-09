#include "../../include/minishell.h"

char	*ft_strdup2(const char *str)
{
	size_t	size;
	char	*dest;

	size = ft_strlen(str);
	dest = malloc((sizeof(char) * size) + 1);
	if (!dest)
		return (NULL);
	ft_strlcpy(dest, str, size + 1);
	return (dest);
}

void add_env_node(t_shell *shell, t_env **env, char *key, char *value)
{
    t_env *new_node = malloc(sizeof(t_env));
    t_env *tmp;
    int i;

    if (!new_node)
        return;
    new_node->name = ft_strdup2(key);
    new_node->value = NULL;
    if (value)
        new_node->value = ft_strdup2(value);
    new_node->next = NULL;

    if (*env == NULL)
    {
        *env = new_node;
        return;
    }
    i = 0;
    tmp = *env;
    while (tmp->next)
    {
        tmp = tmp->next;
        i++;
    }
    tmp->next = new_node;
    free(key);
    free(value);
    shell->new_env = ft_malloc(sizeof(char *) * (i + 2));
    init_new_env(shell);
}

void ft_export(t_shell *shell, char **args)
{
    int i = 1;
    char *key;
    t_env *tmp;
    char *value;

    if (args[i] == NULL)
    {
        ft_env(shell->env, 0);
        return;
    }
    else
    {
        while (args[i])
        {
            if (!(args[i][0] >= 'a' && args[i][0] <= 'z') && !(args[i][0] >= 'A' && args[i][0] <= 'Z') && args[i][0] != '_')
            {
                ft_putstr_fd("minishell: export: ", 2);
                ft_putstr_fd(args[i], 2);
                ft_putstr_fd(" not a valid identifier\n", 2);
                shell->exit_status = 1;
                i++;
                continue;
            }
            char *equal_sign = ft_strchr(args[i], '=');
            if (equal_sign)
            {
                int key_len = equal_sign - args[i];
                key = ft_substr(args[i], 0, key_len);
                value = ft_strdup2(equal_sign + 1);
            }
            else
            {
                key = ft_strdup2(args[i]);
                value = NULL;
            }

            if (key)
            {
                if (ft_strchr(key, '-'))
                {
                    ft_putstr_fd("minishell: export: ", 2);
                    ft_putstr_fd(args[i], 2);
                    ft_putstr_fd(" not a valid identifier\n", 2);
                    shell->exit_status = 1;
                    i++;
                    continue;
                }

                tmp = shell->env;
                while (tmp)
                {
                    if (ft_strncmp(tmp->name, key, ft_strlen(key) + 1) == 0)
                    {
                        if (value)
                        {
                            tmp->value = ft_strdup(value);
                        }
                        break;
                    }
                    tmp = tmp->next;
                }

                if (!tmp)
                    add_env_node(shell, &(shell->env), key, value);
            }
            i++;
        }
    }
}
