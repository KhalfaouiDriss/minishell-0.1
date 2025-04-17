#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include "../lib/libft/libft.h"
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
# define NOT_FOUND 12 
# define INVALAID_DIR 13 

typedef struct s_token
{
    char *value;
    int type;
    struct s_token *next;
    t_error *error;
} t_token;

typedef struct s_cmd
{
    char **args;
    char *infile;
    char *outfile;
    int append;
    struct s_cmd *next;
}t_cmd;

typedef struct s_error
{
    char *invalaid_token;
    int error_type
} t_error;


typedef struct s_shell
{
    char **args;
    int arg_count;
    char *input;
    t_token *token;
} t_shell;


// tokenized 
int check_type(char *token);
void get_the_token(t_shell *shell, char *token);
void ft_tokenized(t_shell *shell);

// utils_1.c 
void	free_split(char **lst);
void init_shell(t_shell *shell);
void free_token(t_token *token);
void free_all(t_shell *shell);
t_token	*new_node(char *value);


#endif