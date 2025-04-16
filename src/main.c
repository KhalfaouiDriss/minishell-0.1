#include "../include/minishell.h"


void init_shell(t_shell *shell)
{
    shell->arg_count = 0;
    shell->args = NULL;
    shell->input = NULL;
    shell->token = NULL;
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
            // free_all(&shell);
            printf("exit\n");
            exit(1);
        }
       add_history(shell.input);
        //shell.args = simple_cmd(&shell, env);
    }
    return 0;
}