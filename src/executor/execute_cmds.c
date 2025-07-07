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
		full_path = ft_malloc(total_size);
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

static int	handle_builtin_redirs(t_cmd *cmd, t_shell *shell)
{
	if(cmd->c_flag  == 1 || cmd->flag_amb == 1 ||cmd->fod_flag ==1)
		return shell->exit_status;
	
	int	in = -1;
	int	out = -1;

	in = dup(0);
	out = dup(1);
	if (cmd->infile)
	{
		if (redirect_input(cmd->infile, cmd))
		{
			shell->exit_status = 1;
			if(in != -1)
				close(in);
			if(out != -1)
				close(out);
			return shell->exit_status;
		}
	}
	if (cmd->outfile_fd)
	{
		dup2(cmd->outfile_fd, 1);
		close(cmd->outfile_fd);
	}
	shell->exit_status = execute_builtin(shell, cmd->args[0], cmd->args);
	dup2(in, 0);
	close(in);
	dup2(out, 1);
	close(out);
	return shell->exit_status;
}

static void	print_not_found_and_exit(t_cmd *cmd, t_shell *shell)
{
	write(2, cmd->args[0], ft_strlen(cmd->args[0]));
	write(2, ": command not found\n", 21);
	if(shell->not_found)
		shell->not_found = 0;
	gc_free_all();
	exit(127);
}

void close_parent_fds(t_cmd *cmd)
{
	if (cmd->outfile_fd > 2)
	{
		close(cmd->outfile_fd);
		cmd->outfile_fd = -1;
	}

	if (cmd->heredoc_fd > 2)
	{
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
}

static void	handle_child(t_cmd *cmd, t_shell *shell, int prev_pipe, int *fd)
{
    signal(SIGQUIT, SIG_DFL);
	char	*path;

	if(cmd->args[0] == NULL)
		exit(0);
	if (cmd->c_flag == 1 || cmd->flag_amb == 1 || cmd->fod_flag == 1)
		exit(1);

	if (cmd->next){
		dup2(fd[1], 1);
		close(fd[1]);
		close(fd[0]);
	}
	if (prev_pipe != -1)
	{
		dup2(prev_pipe, 0);
		close(prev_pipe);
	}
	if (cmd->heredoc_fd != -1)
	{
		dup2(cmd->heredoc_fd, 0);
		close(cmd->heredoc_fd);
	}
	if (is_builtin(cmd->args[0])){
		exit(handle_builtin_redirs(cmd, shell));
	}
	if (cmd->infile)
		redirect_input(cmd->infile, cmd);
	if (!cmd->args[0] || cmd->args[0][0] == '\0'){
		gc_free_all();
		exit(0);
	}
	path = find_command_path(cmd->args[0], shell->env);
	if (!path)
	{
		print_not_found_and_exit(cmd, shell);
	}
	if (cmd->outfile_fd)
	{
		dup2(cmd->outfile_fd, 1);
		close(cmd->outfile_fd);
	}
	// execve(path, cmd->args, envp);
	execve(path, cmd->args, shell->new_env);
	perror(cmd->args[0]);
	gc_free_all();
	exit(126);
}

static void	exec_loop(t_shell *shell)
{
	int		fd[2];
	int		prev_pipe;
	int		pid;
	t_cmd	*cmd;

	cmd = shell->cmd_list;
	prev_pipe = -1;
	if (is_builtin(cmd->args[0]) && !cmd->next)
			return (handle_builtin_redirs(cmd, shell), (void)0);
	while (cmd)
	{
		if (cmd->next && pipe(fd) == -1)
			return (perror("pipe error"), gc_free_all(), (void)0);
		pid = fork();
		if (pid == 0)
			handle_child(cmd, shell, prev_pipe, fd);
		else
			close_parent_fds(cmd);
		if (prev_pipe != -1)
			close(prev_pipe);
		if (cmd->next){
			close(fd[1]);
	        prev_pipe = fd[0];
        }
        cmd = cmd->next;
	}
	wait_all(pid, shell);
}

void	wait_all(int last_pid, t_shell *shell)
{
	int	status;
	int	pid;

	while ((pid = waitpid(-1, &status, 0)) > 0)
	{
	if (pid == last_pid)
	{
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->exit_status = 128 + WTERMSIG(status);
	}

	if (WIFSIGNALED(status))
	{
		int sig = WTERMSIG(status);
		if (sig == SIGQUIT && pid == last_pid)
			ft_putstr_fd("Quit (core dumped)\n", 2);
		else if (sig == SIGINT && pid == last_pid)
			ft_putchar_fd('\n', 2);
	}
	}
}


void	execute_pipeline(t_shell *shell)
{
	exec_loop(shell);
}
