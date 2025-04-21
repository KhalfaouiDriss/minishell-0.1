#include "include/minishell.h"

int main(int ac, char **av,char **envc)
{
	int fd[2];
	pipe(fd);
	int pid=fork();
	if(pid==0)
	{
		char *avv[]={"ls","-l",NULL};
		dup2(fd[1],1);
		close(fd[1]);
		close(fd[0]);	
		execve("/bin/ls",avv,NULL);
	}
	int pid2=fork();
	if(pid2==0)
	{
		char *avv[]={"cat",NULL};
		dup2(fd[0],0);
		close(fd[0]);
		close(fd[1]);
		execve("/bin/cat",avv,NULL);
	}
	close(fd[1]);
	wait(NULL);
	wait(NULL);
}
