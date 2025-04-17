#include "../include/minishell.h"

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
        shell.token = lexer_split_to_tokens(shell.input);
        print_tokens(shell.token);
        add_history(shell.input);
    }
    return 0;
}