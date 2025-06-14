#include "../../include/minishell.h"

void error_exit(char *msg)
{
    perror(msg);
    return ;
}

void redirect_input(char *file, t_cmd *cmd)
{
    int fd;
    fd = open(file, O_RDONLY);
    if (fd < 0){
        perror("open infile");
        if(!is_builtin(cmd->args[0]))
            exit(1);
    }
    dup2(fd, 0);
    close(fd);
}

void redirect_output(t_cmd *cmd, int append)
{
    int fd;
    if (append)
        fd = open(cmd->outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
    else
        fd = open(cmd->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if(fd < 0){
        perror("open outfile");
        cmd->c_flag = 1;
    }
    cmd->outfile_fd = fd;
}


void redirect_output_builtin(t_cmd *cmd, int append)
{
    int fd;

    if (append)
        fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0){
        perror("open outfile");
        cmd->c_flag = 1;
    }
    dup2(fd, 1);
}
// int handle_heredoc(char *delimiter, t_shell *shell)
// {
//     int pipe_fd[2];
//     char *line;
//     char *tmp = NULL;
//     char *fin = NULL;
//     char *exp;
//     int i;
//     int j;
//     int k;

//     if (pipe(pipe_fd) == -1)
//         error_exit("pipe");

//     while (1)
//     {
//         // i = 0;
//         // j = 0;
//         // fin = NULL;  
//         line = readline("> ");
//         if (!line)
//             break;

//         if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
//         {
//             free(line);
//             break;
//         }

//         if (ft_strchr(line, '$'))
//         {
//             while (line[j])
//             {
//                 if (line[j] == '$')
//                 {
//                     k = j;
//                     j++;
//                     while (line[j] && (ft_isalnum(line[j]) || line[j] == '_'))
//                         j++;
//                     tmp = ft_substr(line, k, j - k);
//                     exp = tmp;
//                     tmp = handle_variable_token(tmp, &i, shell, 0);
//                     free(exp);
//                 }
//                 else
//                 {
//                     k = j;
//                     while (line[j] && line[j] != '$')
//                         j++;
//                     tmp = ft_substr(line, k, j - k);
//                 }
//                 exp = fin;
//                 fin = ft_strjoin(fin , tmp);
//                 free(exp);
//                 free(tmp);
// 				tmp = NULL;
//             }
//         }
//         else
//         {
//             fin = ft_strdup(line);
//         }

//         write(pipe_fd[1], fin, ft_strlen(fin));
//         write(pipe_fd[1], "\n", 1);

//         free(fin);
//         free(line);
//     }

//     close(pipe_fd[1]);
//     return pipe_fd[0];
// }

int	handle_heredoc(char *delimiter, t_shell *shell)
{
	const char	*tmp_name = "/tmp/.heredoc_tmp";
	int			tmp_fd, status;
	pid_t		pid;
    int brk = 0; 

	tmp_fd = open(tmp_name, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (tmp_fd == -1)
		return (perror("open"), -1);

	pid = fork();
	if (pid == -1)
		return (perror("fork"), close(tmp_fd), -1);

	if (pid == 0)
	{
		char	*line;
		char	*expanded;
		while (1)
		{
			line = readline("> ");
			if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
			{
				free(line);
				break;
			}
			if (ft_strchr(line, '$'))
			{
				int i = 0;
				expanded = NULL;
				while (line[i])
				{
					if (line[i] == '$')
					{
                        char *var;
						if(ft_strchr(shell->input, '\''))
                            var = handle_variable_token(line, &i, shell, '\'');
                        else
                            var = handle_variable_token(line, &i, shell, 0);
                        // printf("->> %s/n", var);
						if (var)
						{
                            if (ft_strncmp(var, delimiter, ft_strlen(delimiter) + 1) == 0 && !line[i])
                            {
                                free(var);
                                // free(line);
                                brk = 1;
                                break;
                            }
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
			else
			{
				write(tmp_fd, line, ft_strlen(line));
				write(tmp_fd, "\n", 1);
			}
            if(brk)
                break;
			free(line);
		}
		close(tmp_fd);
		exit(0);
	}

	close(tmp_fd);
	waitpid(pid, &status, 0);

	tmp_fd = open(tmp_name, O_RDONLY);
	unlink(tmp_name);
	if (tmp_fd == -1)
		perror("open heredoc read");
	return tmp_fd;
}

