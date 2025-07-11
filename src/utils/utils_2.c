#include "../../include/minishell.h"

int	redirect_input(char *file, t_cmd *cmd)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror("open infile");
		return (1);
	}
	dup2(fd, 0);
	close(fd);
	return (0);
}

void	redirect_output(t_shell *shell,t_cmd *cmd, int append)
{
	int	fd;

	if(!cmd->flag_amb)
	{
		if (append)
			fd = open(cmd->outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
		else
			fd = open(cmd->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror("open outfile");
			cmd->outfile_fd = fd;
			return ;
	
		}
		cmd->outfile_fd = fd;

	}
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
				// free(var);
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

void handel_sig(int sig)
{
	write(1,"\n",1);
	exit(2);
}

static void	run_heredoc_loop(int tmp_fd, char *delimiter, t_shell *shell)
{
	char	*line;
	signal(SIGINT,handel_sig);
	while (1)
	{
		line = readline("> ");
		if (check_delimiter(line, delimiter))
		{
			clean_shell(shell);
			exit(0);
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
	tmp = ft_malloc(6);
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

static int	open_heredoc_file(char **path)
{
	char	*base;
	char	*rand;

	base = "/tmp/.heredoc_tmp";
	rand = gen_random();
	*path = ft_strjoin(base, rand);
	return (open(*path, O_CREAT | O_WRONLY | O_TRUNC, 0600));
}

static void	handle_fork_child(int fd, char *delimiter, t_shell *shell)
{
	run_heredoc_loop(fd, delimiter, shell);
	close(fd);
	clean_shell(shell);
	exit(shell->exit_status);
}

static int	check_exit_status(int status, t_shell *shell)
{
	if (WIFEXITED(status) && WEXITSTATUS(status) == 2)
	{
		shell->exit_status = 130;
		signal(SIGINT, get_sig);
		return (-1);
	}
	return (0);
}

int	handle_heredoc(char *delimiter, t_shell *shell)
{
	char	*path;
	int		tmp_fd;
	pid_t	pid;
	int		status;

	tmp_fd = open_heredoc_file(&path);
	if (tmp_fd == -1)
		return (perror("open"), -1);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		signal(SIGINT, get_sig);
		close(tmp_fd);
		return (perror("fork"), -1);
	}
	if (pid == 0)
		handle_fork_child(tmp_fd, delimiter, shell);
	close(tmp_fd);
	waitpid(pid, &status, 0);
	if (check_exit_status(status, shell) == -1)
		return (-1);
	signal(SIGINT, get_sig);
	tmp_fd = open(path, O_RDONLY);
	if (tmp_fd == -1)
		perror("open heredoc read");
	return (tmp_fd);
}
