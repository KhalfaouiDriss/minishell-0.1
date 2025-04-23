#include "../../include/minishell.h"

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
                printf("%s ", args[i]);
            else 
                printf("%s", args[i]);
            i++;
        }

        if (!no_newline)
            printf("\n");
}
}
