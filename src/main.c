#include "minishell.h"

nt main(int ac, char **av, char **env)
{
    t_shell shell;

    shell.env = NULL;
    
    init_env(&shell, env);
    
    signal(SIGINT, get_sig);
    signal(SIGQUIT, SIG_IGN);
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
        shell.args = simple_cmd(&shell, env);
    }
    return 0;
}