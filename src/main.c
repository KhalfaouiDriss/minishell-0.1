#include "../include/minishell.h"

int check_type(char *token)
{
    if(token == "|")
        return PIPE;
    if(token[0] == '"')
    {
        if(token[ft_strlen(token)] == '"')
            return WORD;
        return 0;
    }
    if(token[0] == '\'')
    {
        if(token[ft_strlen(token)] == '\n')
            return WORD;
        return 0;
    }
}

void get_the_token(t_shell *shell, char *token)
{
    t_token *new;

    new = new_node(token);
    if(!shell->token)
        shell->token = new;
    else
        shell->token->next = new;
    shell->token->type = check_type(shell->token->value);
}

void ft_tokenized(t_shell *shell)
{
    int i;

    i = 0;
    shell->args = ft_split(shell->input, " ");
    while (shell->args[i])
    {
        get_the_token(shell, shell->args[i]);
        
        i++;
    }
    

}

int main(int ac, char **av, char **env)
{
    t_shell shell;
    
    init_shell(&shell);
    while (1)
    {
        shell.input = readline("minishell$ ");
        if (shell.input == NULL)
        {
            free_all(&shell);
            printf("exit\n");
            exit(1);
        }
        ft_tokenized(&shell);
        add_history(shell.input);
    }
    return 0;
}