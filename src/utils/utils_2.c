#include "../../include/minishell.h"


void error_exit(char *msg)
{
    perror(msg);
    exit(1);
}

void redirect_input(char *file) 
{
    int fd = open(file, O_RDONLY);
    if (fd < 0)
        error_exit("open infile");
    dup2(fd, 0);
    close(fd);
}

void redirect_output(char *file, int append) 
{
    int fd;

    if (append)
        fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd < 0)
        error_exit("open outfile");

    dup2(fd, 1);
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

<<<<<<< HEAD
char *ft_strcat(char *dest, const char *src)
{
    char *ptr = dest;

    while (*ptr != '\0')
        ptr++;

    while (*src != '\0')
    {
        *ptr = *src;
        ptr++;
        src++;
    }

    *ptr = '\0';

    return dest;
}

char *ft_strcpy(char *dest, const char *src)
{
    char *ptr = dest;

    while (*src != '\0')
    {
        *ptr = *src;
        ptr++;
        src++;
    }

    *ptr = '\0';

    return dest;
}
=======
int is_all_space(const char *str)
{
    int i = 0;
    while (str[i])
    {
        if (str[i] != ' ' && str[i] != '\t')
            return 0;
        i++;
    }
    return 1;
}

>>>>>>> c551993ea866309c300f3181ee9fefb617b3e879
