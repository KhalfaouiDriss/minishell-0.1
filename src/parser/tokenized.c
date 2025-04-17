#include "../../include/minishell.h"

int check_type(t_shell *shell, char *token)
{
    if (ft_strncmp(token, "|", 1) == 0)
        return PIPE;

    if (token[0] == '"' && token[ft_strlen(token) - 1] == '"')
        return WORD;

    if (token[0] == '\'' && token[ft_strlen(token) - 1] == '\'')
    {
        shell->arg_count++;
        return WORD;
    }
    return WORD;
}

void get_the_token(t_shell *shell, char *token)
{
    t_token *new = new_node(token);
    t_token *tmp;

    new->type = check_type(shell, token);

    if (!shell->token)
        shell->token = new;
    else
    {
        tmp = shell->token;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
}

void fint_the_soc_cott(char *input,char c, int *i)
{
    while (input[*i])
    {
        if(input[*i] == c)
            return;
        i++;
    }
    *i = 999999;
}

void set_error(t_shell *shell)
{
    int i;

    i = 0;
    while (shell->input[i] != ' ')
    {
        i++;
    }
    ft_strlcpy(shell->token->error->invalaid_token, shell->input, i);
}

char **ft_input_split(t_shell *shell)
{
    int i;

    i = 0;
    while (shell->input[i])
    {
        if(shell->input[i] == '"' || shell->input[i] == '\'')
            fint_the_soc_cott(shell->input, shell->input[i], &i); // for find the 2 cott
        if(i == 999999)
        {
            set_error(shell);
            shell->token->error->error_type = NOT_FOUND;
            return NULL;
        }
    }
}

void ft_tokenized(t_shell *shell)
{
    int i = 0;

    shell->args = ft_input_split(shell);
    while (shell->args[i])
    {
        get_the_token(shell, shell->args[i]);
        i++;
    }
}
