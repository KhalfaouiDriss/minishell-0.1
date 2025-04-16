#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include <sys/stat.h>
# include <dirent.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <limits.h>
# include <signal.h>
# include <sys/wait.h>
# include <errno.h>

# define CMD 1
# define PIPE 2
# define AND 3
# define OR 4
# define REDIR 5
# define ACCOL 6
# define APPEND 7
# define HERE_DOC 8
# define OPTION 9
# define WORD 10
# define EXEV 11 


typedef struct s_token
{
    char *token;
    int type;
    struct s_token *next;
} t_token;


// echo hello world
// echo 
typedef struct s_shell
{
    char **args;
    int arg_count;
    char *input;
    t_token *token;
} t_shell;

typedef struct s_cmd
{
    char **args;
    char *infile;
    char *outfile;
    int append;
    struct s_cmd *next;
}t_cmd;

#endif