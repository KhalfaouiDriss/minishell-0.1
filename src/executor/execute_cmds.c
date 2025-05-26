#include "../../include/minishell.h"

char *find_command_path(char *cmd, t_env *envp)
{
	char **paths = NULL;
	char *full_path = NULL;
	char *envp_value;
	int i;

	if (!cmd || ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return ft_strdup(cmd);
		return NULL;
	}

	envp_value = find_env_node(envp, "PATH");
	if (!envp_value)
		return NULL;

	paths = ft_split(envp_value, ':');
	if (!paths)
		return NULL;

	i = 0;
	while (paths[i])
	{
		int total_size = ft_strlen(paths[i]) + ft_strlen(cmd) + 2;
		full_path = malloc(total_size);
		if (!full_path)
		{
			free_split(paths);
			return NULL;
		}

		ft_strlcpy(full_path, paths[i], total_size);
		ft_strlcat(full_path, "/", total_size);
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

int execute_pipeline(t_shell *shell, char **envp)
{
    int exit_status = 0;
    int fd[2];
    int prev_pipe = -1;
    int pid;
    t_cmd *current = shell->cmd_list;

    if (current && !current->next && is_builtin(current->args[0]))
    {
        int stdin_backup = dup(0);
        int stdout_backup = dup(1);

        if (current->infile)
            redirect_input(current->infile, current->heredoc);
        if (current->outfile)
            redirect_output_bu(current, current->append);

        exit_status = execute_builtin(shell, current->args[0], current->args);

        dup2(stdin_backup, 0);
        dup2(stdout_backup, 1);
        close(stdin_backup);
        close(stdout_backup);

        return exit_status;
    }
    
    // if(current && !current->next && current->infile && current->heredoc == 1)
    //      return 0;

    while (current)
    {
        if (current->next && pipe(fd) < 0)
            error_exit("pipe error");

        pid = fork();
        if (pid < 0)
            error_exit("fork error");

        if (pid == 0)
        {
            if (current->next)
                dup2(fd[1], 1);
            if (prev_pipe != -1)
                dup2(prev_pipe, 0);

            if (prev_pipe != -1)
                close(prev_pipe);
            if (current->next)
                close(fd[0]);
            if (current->infile)
                redirect_input(current->infile, current->heredoc);
            if (current->outfile)
                redirect_output(current, current->append);

            if (!current->args[0] || current->args[0][0] == '\0')
                exit(0);

            if (is_builtin(current->args[0]) && current->next == NULL)
                exit(execute_builtin(shell, current->args[0], current->args));

            char *path = find_command_path(current->args[0], shell->env);
            if (!path)
            {
                write(2, current->args[0], ft_strlen(current->args[0]));
                write(2, ": command not found\n", 21);
                exit(127);
            }

            if (current->outfile_fd)
            {
                dup2(current->outfile_fd, 1);
                close(current->outfile_fd);
            }

            execve(path, current->args, envp);
            write(2, "minishell: execve failed\n", 26);
            free(path);
            exit(126);
        }

        if (prev_pipe != -1)
            close(prev_pipe);
        if (current->next)
        {
            close(fd[1]);
            prev_pipe = fd[0];
        }

        current = current->next;
    }

    int status;
    while (wait(&status) > 0)
    {
        if (WIFSIGNALED(status))
                write(1, "\n", 1);
    }

    if (WIFEXITED(status))
        shell->exit_status = WEXITSTATUS(status);

    return exit_status;
}
