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
# define OPTION_INVA   14
# define INPUT_INVA    15

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
    int flag;
    struct s_cmd *next; // next in pipeline
} t_cmd;

typedef struct s_env
{
    char *name;
    char *value;
    void *next;
} t_env;

typedef struct s_shell
{
    char **args;
    int arg_count;
    char *input;
    t_token *token;
    t_env *env;
} t_shell;


// tokenized 
t_token *lexer_split_to_tokens(const char *input);
void handle_variable_token(const char *input, int *i, t_token **head);
int	handle_option_token(const char *input, int *i, t_token **head);
void	handle_special_token(const char *input, int *i, t_token **head);

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
void free_cmds(t_cmd *cmds);


//utils_2.c
void error_exit(char *msg);
void redirect_input(char *file,int heredoc);
void redirect_output(char *file, int append);
int is_all_space(const char *str);
int handle_heredoc(char *delimiter);

// 
int execute_pipeline(t_cmd *cmd_list, char **envp);

// Built-in command functions
void ft_echo(char **args);
void ft_cd(char **args);
void ft_pwd(char **args);
void ft_export(char **args);
void ft_unset(char **args);
void ft_env(char **args);
int ft_exit(char **args);

// Helper function to identify built-ins
int is_builtin(char *cmd);
int execute_builtin(char *cmd, char **args);


int check_if_is_builtin(char *s);
#endif