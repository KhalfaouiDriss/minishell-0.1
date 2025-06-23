#include "../../include/minishell.h"

char *find_command_path(char *cmd, t_env *envp)
{
	char **paths = NULL;
	char *full_path = NULL;
	char *envp_value;
	int i;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return ft_strdup(cmd);
		return NULL;
	}

	envp_value = find_env_node(envp, "PATH");
	if (!envp_value)
		return NULL;

	paths = ft_split(envp_value, ':');
	i = 0;
	while (paths[i])
	{
		int total_size = ft_strlen(paths[i]) + ft_strlen(cmd) + 2;
		full_path = malloc(total_size);
        ft_strlcpy(full_path, paths[i], total_size);
        ft_strlcat(full_path, "/", total_size);
        ft_strlcat(full_path, cmd, total_size);
        if(access(full_path , X_OK) == 0){
            free_split(paths);
            return full_path;
        }
		i++;
	}
	free_split(paths);
	return NULL;
}

void execute_pipeline(t_shell *shell, char **envp)
{
    int fd[2];
    int prev_pipe = -1;
    int pid;
    t_cmd *current = shell->cmd_list;
    if(current->c_flag)
        shell->exit_status = 1;
    while (current && !current->c_flag)
    {
        if (is_builtin(current->args[0]) && current->next == NULL)
        {
            int in = dup(0);
            int out = dup(1);
            if (current->infile){
                if(redirect_input(current->infile, current)){
                    shell->exit_status = 1;
                    return ;
                }
            }
            if (current->outfile)
                redirect_output_builtin(current, current->append);
            shell->exit_status = execute_builtin(shell, current->args[0], current->args);
            dup2(in, 0);
            close(in);
            dup2(out, 1);
            close(out);            
            return ;
        }
        
        if (current->next && pipe(fd) < 0)
        {

            perror("pipe error");
            exit(1);
        }
        pid = fork();
        if (pid == 0)
        {
            if (current->next)
                dup2(fd[1], 1);
            if (prev_pipe != -1)
                dup2(prev_pipe, 0);
            if (prev_pipe != -1)
                close(prev_pipe);
            if (current->heredoc_fd > 2)
            {
                dup2(current->heredoc_fd, 0);
                close(current->heredoc_fd);
            }
            if (current->infile)
                redirect_input(current->infile, current);
            if (!current->args[0] || current->args[0][0] == '\0')
                exit(0);
            if(is_builtin(current->args[0]))
            {
                execute_builtin(shell, current->args[0], current->args);
                exit(0);
            } 
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
            write(2, path, ft_strlen(path));
            write(2, ": Is a directory \n", 19);
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
    while (waitpid(pid, &status, 0) > 0)
    {
        if (WIFSIGNALED(status))
            write(1, "\n", 1);
    }
    if (WIFEXITED(status))
        shell->exit_status = WEXITSTATUS(status);
}