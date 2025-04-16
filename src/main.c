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
<<<<<<< HEAD
       add_history(shell.input);
        //shell.args = simple_cmd(&shell, env);
=======
        // add_history(shell.input);
>>>>>>> b07be71aa0414b1acebefd773b0bc6291fa5b99b
    }
    return 0;
}