#include "../../include/minishell.h"

int is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    
    if (ft_strncmp(cmd, "echo", 5) == 0 ||
        ft_strncmp(cmd, "pwd", 4) == 0 ||
        ft_strncmp(cmd, "cd", 3) == 0 ||
        ft_strncmp(cmd, "exit", 5) == 0 ||
        ft_strncmp(cmd, "export", 7) == 0 ||
        ft_strncmp(cmd, "env", 4) == 0 ||
        ft_strncmp(cmd, "unset", 6) == 0)
    {
        return (1);
    }
    
    return (0);
}

int execute_builtin(t_shell *shell, char *cmd, char **args)
{
    if (ft_strncmp(cmd, "echo", 5) == 0) {
        ft_echo(args);
        return (0);
    }
    else if (ft_strncmp(cmd, "cd", 3) == 0) {
        ft_cd(shell, args);
        return (0);
    }
    
    else if (ft_strncmp(cmd, "pwd", 4) == 0) {
        ft_pwd(shell ,args);
        return (0);
    }
    else if (ft_strncmp(cmd, "export", 6) == 0) {
        ft_export(&(shell->env), args);
        return (0);
    }
    else if (ft_strncmp(cmd, "unset", 5) == 0) {
        ft_unset(&(shell->env), args);
        return (0);
    }
    else if (ft_strncmp(cmd, "env", 3) == 0) {
        ft_env(shell->env, 0);
        return (0);
    }
    else if (ft_strncmp(cmd, "exit", 5) == 0) {
        return (ft_exit(shell, args));
    }
    return (127);
}
