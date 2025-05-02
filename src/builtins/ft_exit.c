#include "../../include/minishell.h"

int ft_exit(t_shell *shell, char **args)
{
    int exit_code = 0;
    
    if (args[1])
        exit_code = ft_atoi(args[1]); 
    printf("exit\n");
    free_all(shell);
    free_env(shell->env);
    exit(exit_code);
    return (exit_code);
}
