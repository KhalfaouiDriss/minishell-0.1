#include "../../include/minishell.h"

void ft_pwd(t_shell *shell, char **args)
{
    char *dir;

    (void)args;

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
