#include "../../include/minishell.h"

// دوال بسيطة لتقليل الأخطاء
void error_exit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void redirect_input(const char *file) 
{
    int fd = open(file, O_RDONLY);
    if (fd < 0)
        error_exit("open infile");
    dup2(fd, STDIN_FILENO);
    close(fd);
}

void redirect_output(const char *file, int append) 
{
    int fd = open(file, O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0644);
    if (fd < 0)
        error_exit("open outfile");
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

void exec_command(t_cmd *cmd, char **envp) 
{
    if (cmd->infile)
        redirect_input(cmd->infile);
    if (cmd->outfile)
        redirect_output(cmd->outfile, cmd->append);

    execvp(cmd->args[0], cmd->args);
    perror("exec failed");
    exit(EXIT_FAILURE);
}
