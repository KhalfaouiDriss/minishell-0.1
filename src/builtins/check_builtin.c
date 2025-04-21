#include "../../include/minishell.h"

int check_if_is_builtin(char *s)
{
   if (ft_strncmp(s, "echo", 4) == 0)
        return 1;
    else if (ft_strncmp(s, "pwd", 3) == 0)
        return 1;
    else if (ft_strncmp(s, "cd", 2) == 0)
        return 1;
    else if (ft_strncmp(s, "env", 3) == 0)
        return 1;
    else if (ft_strncmp(s, "exit", 4) == 0)
        return 1;
    else if (ft_strncmp(s, "export", 6) == 0)
        return 1;
    else
        return 0;
}
