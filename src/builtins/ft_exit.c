#include "../../include/minishell.h"

int ft_exit(char **args)
{
    int exit_code = 0;
    
    if (args[1])
        exit_code = ft_atoi(args[1]); 
    printf("exit");
    exit(exit_code);
    return (exit_code);
}