#include "../../include/minishell.h"

int ft_exit(t_shell *shell, char **args)
{
    int exit_code = 0;
    if (args[1])
        exit_code = ft_atoi(args[1]);
    if(exit_code == -1)
        {
            write(2, args[1], ft_strlen(args[1]));
            write(2, ": numeric argument required\n", 28);
            gc_free_all();
            exit(2);
        }
    if(!args[1]){
        printf("exit\n");
        gc_free_all();
        exit(shell->exit_status);
    }
    
    if(args[2]){
        write(2, "exit\n", 6);
        write(2, "too many arguments\n", 20);
        return 1;
    }
    printf("exit\n");
    gc_free_all();
    exit(exit_code);
    return 0;
}
