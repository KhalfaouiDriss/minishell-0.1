#include "../../include/minishell.h"

int ft_exit(t_shell *shell, char **args)
{
    int exit_code = 0;
    
    if (args[1])
        exit_code = ft_atoi(args[1]);
    printf("exit\n");
    if(exit_code == -1)
    {
        printf("%s :numeric argument required\n",args[1]);
        exit(2);
    }
    exit(exit_code);
    return (exit_code);
}
