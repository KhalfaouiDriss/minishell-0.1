#include "../../include/minishell.h"

int is_special(char c)
{
    return (c == '|' || c == '>' || c == '<');
}

t_token *new_token(char *val, int type, int error_type)
{
    t_token *t = malloc(sizeof(t_token));
    if (!t)
        return NULL;
    
    t->error = 0;
    if(!type)
        t->error = error_type;
    t->value = strdup(val);
    t->type = type;
    t->next = NULL;
    return t;
}

void add_token(t_token **head, t_token *new)
{
    if (!new || !new->value || new->value[0] == '\0')
    {
        if (new)
        {
            free(new->value);
            free(new);
        }
        return;
    }
    if (!*head)
        *head = new;
    else
    {
        t_token *tmp = *head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
}

void print_tokens(t_token *head)
{
    while (head)
    {
        printf("Token: %s | Type: %d ", head->value, head->type);
        if(head->type == 0)
            printf("| error type %d", head->error);
        printf("\n");
        head = head->next;
    }
}