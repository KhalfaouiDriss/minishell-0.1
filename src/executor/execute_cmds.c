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
    while (envp[i]) {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0) {
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
        full_path = malloc(strlen(paths[i]) + ft_strlen(cmd) + 2);
        if (!full_path)
            return NULL;
            int total_size = strlen(paths[i]) + strlen(cmd) + 2;
            full_path = malloc(total_size);
            if (!full_path)
                return NULL;
            
            ft_strlcpy(full_path, paths[i], total_size);
            ft_strlcat(full_path, "/", total_size);
            ft_strlcat(full_path, cmd, total_size);
            
        if (access(full_path, X_OK) == 0) {
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
    pid_t pid;
    t_cmd *current = cmd_list;

    while (current)
    {
        if (current->next && pipe(fd) < 0)
            error_exit2("pipe error");

        pid = fork();
        if (pid < 0)
            error_exit2("fork error");

        if (pid == 0)
        {
            if (prev_pipe != -1)
            {
                dup2(prev_pipe, 0);
                close(prev_pipe);
            }

            if (current->next)
                dup2(fd[1], 1);

            if (current->infile)
                redirect_input(current->infile);
            if (current->outfile)
                redirect_output(current->outfile, current->append);

            char *path = find_command_path(current->args[0], envp);
            if (!path) {
                write(2, "minishell: command not found \n", 31);
                write(2, current->args[0], strlen(current->args[0]));
                write(2, "\n", 1);
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

while (wait(&status) > 0)
    exit_status = status >> 8;

return exit_status;

}
