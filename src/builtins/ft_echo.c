#include "../../include/minishell.h"

void ft_strchar(char *s, int f)
{
    int len = ft_strlen(s);
    write(1, s, len);
    if (f == 1)
        write(1, " ", 1);
}

void ft_echo(char **args)
{
    int i = 1;
    int no_newline = 0;

    while (args[i] && args[i][0] == '-')
    {
        int j = 1;
        while (args[i][j] == 'n')
            j++;
        if (args[i][j] == '\0')
        {
            no_newline = 1;
            i++;
        }
        else
            break;
    }

    while (args[i])
    {
        ft_strchar(args[i], args[i + 1] != NULL);
        i++;
    }

    if (i == 1 || !no_newline)
        write(1, "\n", 1);
}
