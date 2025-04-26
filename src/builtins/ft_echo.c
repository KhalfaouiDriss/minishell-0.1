#include "../../include/minishell.h"

void ft_strchar(char *s, char f)
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

{
        if (args[1] && args[1][0] == '-' && args[1][1] == 'n')
        {
            int j = 2;  
            while (args[1][j] == 'n') {
                j++;
            }

            if (args[1][j] == '\0') {
                no_newline = 1;  
                i++; 
            }
        }

        while (args[i])
        {
            if (args[i + 1])
                ft_strchar(args[i], 1);
            else 
                ft_strchar(args[i], 0);
            i++;
        }

        if (!no_newline)
            write(1, "\n", 1);
}
}
