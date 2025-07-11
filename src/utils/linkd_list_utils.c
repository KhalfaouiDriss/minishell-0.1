#include "../../include/minishell.h"

t_token	*new_node(char *value)
{
	t_token	*new;

	new = ft_malloc(sizeof(*new));
	if (!new)
		return (NULL);
    new->value = value;
	new->next = NULL;
	return (new);
}

t_token *new_token(int *ebag ,char *val, int type, int error_type)
{
    t_token *t = (t_token *)ft_malloc(sizeof(t_token));
    if (!t)
        return NULL;
    
    if (val)
    {
        t->value = ft_strdup(val);
        if (!t->value)
        return NULL;
    }
    else
    {
        t->value = ft_strdup("");
    }
    t->ebag = *ebag;    
    t->type = type;
    t->error = error_type;
    t->next = NULL;
    *ebag = 1;
    return t;
}

void add_token(t_token **head, t_token *new)
{
    if (!new)
        return;

    if (!*head)
    {
        *head = new;
        return;
    }

    t_token *tmp = *head;
    while (tmp->next)
        tmp = tmp->next;

    tmp->next = new;
}

char *find_env_node(t_env *env, char *key)
{
    t_env *tmp = env;
    size_t key_len;

    key_len = ft_strlen(key);
    while (tmp)
    {
        if (ft_strncmp(tmp->name, key, key_len) == 0 && tmp->name[key_len] == '\0')
        {
            return tmp->value;
        }
        tmp = tmp->next;
    }
    return NULL;
}
