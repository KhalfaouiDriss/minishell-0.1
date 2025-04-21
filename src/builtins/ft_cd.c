#include "../../include/minishell.h"

void ft_cd(char **args)
{
    const char *dir;

    if (args[0] && strcmp(args[0], "cd") == 0)
    {
        if (args[1] == NULL)
        {
            dir = getenv("HOME");
            if (!dir)
            {
                printf("-bash: cd: HOME not set\n");
                return;
            }
        }
        else
        {
            dir = args[1];
        }

        if (chdir(dir) == -1)
            printf("-bash: cd: %s: No such file or directory\n", dir);
    }
}