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

void ft_tokenized(t_shell *shell)
{
    int i = 0;

    shell->args = ft_split(shell->input, ' ');
    while (shell->args[i])
    {
        get_the_token(shell, shell->args[i]);

        i++;
    }
}
