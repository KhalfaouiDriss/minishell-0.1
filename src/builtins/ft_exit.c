#include "../../include/minishell.h"

int ft_exit(t_shell *shell, char **args)
{
    int exit_code = 0;
    if(args[2] != NULL)
    {
        write(2, "too many arguments\n", 20);
        exit(1);
    }
    
    if (args[1])
        exit_code = ft_atoi(args[1]);
    printf("exit\n");
    if(exit_code == -1)
    {
        write(2, args[1], ft_strlen(args[1]));
        write(2, ": numeric argument required\n", 28);
        exit(2);
    }
    exit(exit_code);
    return 0;
}
