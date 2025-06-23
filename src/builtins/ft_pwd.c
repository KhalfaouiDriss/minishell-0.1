#include "../../include/minishell.h"

void ft_pwd(t_shell *shell, char **args)
{
    char *dir;

    (void)args;

    if(args[1] && args[1][0] == '-' && args[1][1] != '\0'){
        write(2, &args[1][0], 1);
        write(2, &args[1][1], 1);
        write(2, " : invalid option\n", 18);
        shell->exit_status = 2;
        return ;
    }
    dir = getcwd(NULL, 0);
    if (!dir)
    {
        char *pwd = find_env_node(shell->env, "PWD");
        if (pwd)
            printf("%s\n", pwd);
        else
            printf("pwd: error retrieving working directory\n");
        return;
    }

    printf("%s\n", dir);
    free(dir);
}
