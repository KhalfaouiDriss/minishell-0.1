#include "../../include/minishell.h"

int	redirect_input(char *file, t_cmd *cmd)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror("open infile");
		if (!is_builtin(cmd->args[0]))
			exit(1);
		return (1);
	}
	dup2(fd, 0);
	close(fd);
	return (0);
}

void	redirect_output(t_cmd *cmd, int append)
{
	int	fd;

	if (append)
		fd = open(cmd->outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(cmd->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open outfile");
		cmd->c_flag = 1;
	}
	cmd->outfile_fd = fd;
}

void	redirect_output_builtin(t_cmd *cmd, int append)
{
	int	fd;

	if (append)
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open outfile");
		cmd->c_flag = 1;
	}
	dup2(fd, 1);
}

void	hand(int sig)
{
	if (sig == SIGINT)
		exit(130);
}

static int	check_delimiter(char *line, char *delimiter)
{
	if (!line)
		return (1);
	if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		return (1);
	return (0);
}

static void	write_expanded_line(char *line, t_shell *shell, int tmp_fd)
{
	int		i;
	char	*var;

	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			if (ft_strchr(shell->input, '\''))
				var = handle_variable_token(line, &i, shell, '\'');
			else
				var = handle_variable_token(line, &i, shell, 0);
			if (var)
			{
				write(tmp_fd, var, ft_strlen(var));
				free(var);
			}
		}
		else
		{
			write(tmp_fd, &line[i], 1);
			i++;
		}
	}
	write(tmp_fd, "\n", 1);
}

static void	run_heredoc_loop(int tmp_fd, char *delimiter, t_shell *shell)
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	while (1)
	{
		line = readline("> ");
		if (check_delimiter(line, delimiter))
		{
			free(line);
			break ;
		}
		if (ft_strchr(line, '$'))
			write_expanded_line(line, shell, tmp_fd);
		else
		{
			write(tmp_fd, line, ft_strlen(line));
			write(tmp_fd, "\n", 1);
		}
		free(line);
	}
}

char *gen_random(){
	char *tmp;
	tmp = malloc(6);
	int i = 0;
	int fd = open("/dev/random", O_RDONLY);
	while(i < 5){
		read(fd, &tmp[i],1);
		if (tmp[i] >= 32 && tmp[i] <= 126 && tmp[i] != '/')
			i++;
	}
	tmp[i] = '\0';
	return tmp;
	
}

int	handle_heredoc(char *delimiter, t_shell *shell)
{
	char	*tmp = "/tmp/.heredoc_tmp";
	char *tmp2 = gen_random();
	tmp = ft_strjoin(tmp, tmp2);
	int			tmp_fd;
	pid_t		pid;
	int			status;

	tmp_fd = open(tmp, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (tmp_fd == -1)
		return (perror("open"), -1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), close(tmp_fd), -1);
	if (pid == 0)
	{
		run_heredoc_loop(tmp_fd, delimiter, shell);
		close(tmp_fd);
		exit(0);
	}
	close(tmp_fd);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		shell->exit_status = 130;
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	tmp_fd = open(tmp, O_RDONLY);
	// unlink(tmp);
	if (tmp_fd == -1)
		perror("open heredoc read");
	return (tmp_fd);
}

