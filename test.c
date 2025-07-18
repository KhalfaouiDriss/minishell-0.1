#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int ac, char **av)
{
    int i = 1;
    int fd[2];
    int prev_fd = -1;
    int pid;

    while (i < ac)
    {
        if (i < ac - 1)
            pipe(fd);
        pid = fork();
        if (pid == 0)
        {
            if (prev_fd != -1)
            {
                dup2(prev_fd, 0);
                close(prev_fd);
            }
            if (i < ac - 1)
            {
                close(fd[0]);
                dup2(fd[1], 1);
                close(fd[1]);
            }
            execvp(av[i], av + i);
            perror("execvp");
            exit(1);
        }
        if (prev_fd != -1)
            close(prev_fd);
        if (i < ac - 1)
        {
            close(fd[1]);
            prev_fd = fd[0];
        }
        i++;
    }
    while (wait(NULL) > 0);
    return 0;
}
