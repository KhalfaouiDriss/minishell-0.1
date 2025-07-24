#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "lib/libft/libft.h"

char	*get_cmd_path(char *cmd, char **envp)
{
	char	*path_env = NULL;
	char	**paths;
	char	full_path[1024];
	int		i = 0;

	while (envp[i])
	{
		if (strcmp(envp[i], "PATH=") == 0)
		{
			path_env = envp[i] + 5;
			break;
		}
		i++;
	}
	if (!path_env)
		return (NULL);

	char **token = ft_split(path_env, ':');
    i = 0;
	while (token[i])
	{
		if (access(full_path, X_OK) == 0)
			return strdup(full_path);
        i++;
	}
	return NULL;
}

void	execute_cmd(char *cmd_str, char **envp)
{
	char	*args[64];
	int		i = 0;

	char **token = ft_split(cmd_str, ' ');
	while (token[i])
    {
		args[i] = token[i];
        i++;
    }
	args[i] = NULL;

	char *cmd_path = get_cmd_path(args[0], envp);
	if (!cmd_path)
		error_exit("Command not found");

	execve(cmd_path, args, envp);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd[2];
	pid_t	pid1, pid2;

	pipe(fd);

	pid1 = fork();
	if (pid1 == 0)
	{
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
		execute_cmd(argv[1], envp);
	}

	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(fd[0], 0);
		close(fd[1]);
		close(fd[0]);
		execute_cmd(argv[2], envp);
	}

	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);

	return (0);
}
