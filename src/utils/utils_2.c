#include "../../include/minishell.h"

void error_exit(char *msg)
{
    perror(msg);
    exit(1);
}

void redirect_input(char *file, int heredoc)
{
    int fd;

    if (heredoc)
    {
        fd = handle_heredoc(file);
    }
    else
    {
        fd = open(file, O_RDONLY);
        if (fd < 0)
            error_exit("open infile");
    }

    if (dup2(fd, 0) < 0)
        error_exit("dup2 infile");

    close(fd);
}

void redirect_output(t_cmd *cmd, int append)
{
    int fd;

    if (append)
        fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd < 0)
        error_exit("open outfile");
    cmd->outfile_fd = fd;
}


void redirect_output_bu(t_cmd *cmd, int append)
{
    int fd;

    if (append)
        fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
        error_exit("open outfile");
    if (dup2(fd, 1) < 0)
        error_exit("dup2 outfile");
    close(fd);  
}

int handle_heredoc(char *delimiter)
{
    int pipe_fd[2];
    char *line;

    if (pipe(pipe_fd) == -1)
        error_exit("pipe");

    while (1)
    {
        line = readline("> ");
        if (!line)
            break;

        if (ft_strncmp(line, delimiter,ft_strlen(delimiter)) == 0)
        {
            free(line);
            break;
        }

        write(pipe_fd[1], line, ft_strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }

    close(pipe_fd[1]);
    return pipe_fd[0];
}

