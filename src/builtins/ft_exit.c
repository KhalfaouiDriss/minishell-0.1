#include "../../include/minishell.h"

int ft_exit(t_shell *shell, char **args)
{
    int exit_code = 0;
    if (args[1])
        exit_code = ft_atoi(args[1]);
    if(!args[1]){
        printf("exit\n");
        gc_free_all();
        exit(shell->exit_status);
    }
    
    if(args[2]){
        write(2, "exit", 5);
        write(2, "too many arguments\n", 20);
    }
    printf("exit\n");
    if(exit_code == -1)
    {
        gc_free_all();
        write(2, args[1], ft_strlen(args[1]));
        write(2, ": numeric argument required\n", 28);
        exit(2);
    }
    gc_free_all();
    exit(exit_code);
    return 0;
}
