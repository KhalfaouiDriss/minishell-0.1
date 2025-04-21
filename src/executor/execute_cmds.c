#include "../../include/minishell.h"

char *find_command_path(char *cmd, char **envp)
{
    char **paths;
    char *path = NULL;
    char *full_path = NULL;
    int i;

    if (!cmd || ft_strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return ft_strdup(cmd);
        return NULL;
    }

    i = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0)
        {
            paths = ft_split(envp[i] + 5, ':');
            break;
        }
        i++;
    }

    if (!paths)
        return NULL;

    i = 0;
    while (paths[i])
    {
        int total_size = ft_strlen(paths[i]) + ft_strlen(cmd) + 2;
        full_path = malloc(total_size);
        if (!full_path)
            return NULL;
            
        ft_strlcpy(full_path, paths[i], ft_strlen(paths[i]) + 1);
        ft_strlcat(full_path, "/", ft_strlen(paths[i]) + 2);
        ft_strlcat(full_path, cmd, total_size);
            
        if (access(full_path, X_OK) == 0)
        {
            free_split(paths);
            return full_path;
        }
        free(full_path);
        i++;
    }

    free_split(paths);
    return NULL;
}

static void error_exit2(const char *msg)
{
    write(2, "minishell: ", 11);
    write(2, msg, ft_strlen(msg));
    write(2, "\n", 1);
    exit(1);
}

int execute_pipeline(t_cmd *cmd_list, char **envp)
{
    int exit_status = 0;
    int fd[2];
    int prev_pipe = -1;
    int pid;
    t_cmd *current = cmd_list;

    while (current)
    {
<<<<<<< HEAD
        if (current->next == NULL && is_builtin(current->args[0]))
            return execute_builtin(current->args[0], current->args);
=======
        if(check_if_is_builtin(current->args[0]))
                
>>>>>>> 3e8dbbbd99369a756716d2475b518ba167acfe9a

        if (current->next && pipe(fd) < 0)
            error_exit2("pipe error");

        pid = fork();
        if (pid < 0)
            error_exit2("fork error");

        if (pid == 0)
        {
            if (current->next)
                dup2(fd[1], 1);

            if (prev_pipe != -1)
            {
                dup2(prev_pipe, 0);
                close(prev_pipe);
            }
            if (current->infile)
                redirect_input(current->infile);
            if (current->outfile)
                redirect_output(current->outfile, current->append);

            if (is_builtin(current->args[0]))
            {
                exit(execute_builtin(current->args[0], current->args));
            }

            char *path = find_command_path(current->args[0], envp);
            if (!path)
            {
                write(2, current->args[0], ft_strlen(current->args[0]));
                write(2, ": ", 2);
                write(2, "command not found \n", 20);
                exit(1);
            }

            execve(path, current->args, envp);
            write(2, "minishell: execve failed\n", 26);
            free(path);
            exit(1);
        }

        if (prev_pipe != -1)
            close(prev_pipe);
        if (current->next) {
            close(fd[1]);
            prev_pipe = fd[0];
        }

        current = current->next;
    }

    int status;

    while ((wait(&status)) > 0)
    {
        if (WIFEXITED(status))
            exit_status = WEXITSTATUS(status);
    }
    return exit_status;

}
