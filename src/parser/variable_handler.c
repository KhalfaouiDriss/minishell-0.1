#include "../../include/minishell.h"


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
        add_token(head, new_token("$", WORD, 0));
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