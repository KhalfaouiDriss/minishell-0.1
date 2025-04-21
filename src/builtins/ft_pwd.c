#include "../../include/minishell.h"
void ft_pwd(char **args)
{
    char *dir;
    if (strcmp(args[0], "pwd") == 0)
    {
        dir = getcwd(NULL, 0);
        printf("%s\n", dir);
        free(dir);
    }
}