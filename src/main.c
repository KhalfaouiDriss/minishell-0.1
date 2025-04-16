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
        
        add_history(shell.input);
    }
    return 0;
}