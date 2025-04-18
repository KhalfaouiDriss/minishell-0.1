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

// Basic Token Types
# define ERROR         0   // Error
# define WORD          1   // Any word (command or argument)
# define PIPE          2   // |
# define REDIR_IN      3   // <
# define REDIR_OUT     4   // >
# define REDIR_APPEND  5   // >>
# define REDIR_HEREDOC 6   // <<
# define OPTION        7   // e.g. -l or --help (optional: classify separately)
# define VARIABLE      8   // $var

// Execution Result Types (not for parsing, but post-exec status)
# define EXEC          10
# define NOT_FOUND     11
# define INVALID_DIR   12
# define QUETS_INVA    13

typedef struct s_error
{
    char *invalaid_token;
    int error_type;
} t_error;

typedef struct s_token
{
    char *value;
    int type;
    struct s_token *next;
    int error;
} t_token;

typedef struct s_cmd {
    char **args;        // command & args
    char *infile;       // file for input redirection
    char *outfile;      // file for output redirection
    int append;         // 1 if >>, 0 if >
    int heredoc;        // 1 if << used
    struct s_cmd *next; // next in pipeline
} t_cmd;




typedef struct s_shell
{
    char **args;
    int arg_count;
    char *input;
    t_token *token;
} t_shell;


// tokenized 
t_token *lexer_split_to_tokens(const char *input);
void handle_variable_token(const char *input, int *i, t_token **head);

// tokenized Utils
void add_token(t_token **head, t_token *new);
void print_tokens(t_token *head);
t_token *new_token(char *val, int type, int error_type);
int is_special(char c);
void add_token(t_token **head, t_token *new);

// utils_1.c
void	free_split(char **lst);
void init_shell(t_shell *shell);
void free_token(t_token *token);
void free_all(t_shell *shell);
t_token	*new_node(char *value);

// parse_tokens.
t_cmd *parse_tokens(t_token *token);
int count_args(t_token *token);
char *safe_strdup(const char *s);
t_cmd *parse_tokens(t_token *token);


//utils_2.c
void error_exit(const char *msg);
void redirect_input(const char *file);
void redirect_output(const char *file, int append);

#endif