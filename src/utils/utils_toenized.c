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
    if(type)
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

void handle_variable_token(const char *input, int *i, t_token **head)
{
    int     start;
    int     var_len;
    char    *var_name;
    char    *var_value;
    (*i)++;
    start = *i;

    if (input[*i] == '?')
    {
        (*i)++;
        add_token(head, new_token("0", WORD, 0));
        return;
    }
    while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
        (*i)++;
    
    if (start == *i)
    {
        // add_token(head, new_token("$", WORD, 0));
        return;
    }
    var_len = *i - start;
    var_name = ft_substr(input, start, var_len);
    var_value = getenv(var_name);
    
    if (var_value)
        add_token(head, new_token(var_value, WORD, 0));
    else
        add_token(head, new_token("", WORD, 0));
    
    free(var_name);
}

void print_tokens(t_token *head)
{
    while (head)
    {
        printf("Token: %s | Type: %d ", head->value, head->type);
        if(!head->type)
            printf("| error type %d", head->error);
        printf("\n");
        head = head->next;
    }
}