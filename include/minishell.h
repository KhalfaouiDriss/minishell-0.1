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
#include <stdbool.h>

# define ERROR         0
# define WORD          1   
# define PIPE          2   
# define REDIR_IN      3   
# define REDIR_OUT     4   
# define REDIR_APPEND  5
# define REDIR_HEREDOC 6
# define OPTION        7
# define VARIABLE      8
# define EXEC          10
# define NOT_FOUND     11
# define INVALID_DIR   12
# define QUETS_INVA    13
# define OPTION_INVA   14
# define INPUT_INVA    15
# define D_QUOTE       16
# define S_QUOTE       17

typedef struct s_error
{
    char *invalaid_token;
    int error_type;
} t_error;

typedef struct s_token
{
    char *value;
    int type;
    int quot_type;
    struct s_token *next;
    struct s_token *prev;
    int error;
} t_token;

typedef struct s_cmd
{
    char **args;
    char *infile;       
    char *outfile;      
    int outfile_fd;
    int append;
    int heredoc;
    int heredoc_fd;
    int c_flag;
    int fd_builtin;
    struct s_cmd *next;
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
    t_cmd *cmd_list;
    int exit_status;
    int blocked;
    int pip_count;
} t_shell;


// tokenized 
t_token	*lexer_split_to_tokens(t_shell *shell);
char *handle_variable_token(char *str, int *i, t_shell *shell, char qte);
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
void free_tokens(t_token *token);
void free_all(t_shell *shell);
t_token	*new_node(char *value);
void	get_sig(int sig);

t_cmd *parse_tokens(t_shell *shell);
int count_args(t_token *token);
char *safe_strdup(char *s);
void free_cmds(t_cmd *cmds);
void free_env(t_env *env);
int redirect_input(char *file, t_cmd *cmd);
void redirect_output(t_cmd *cmd, int append);
int handle_heredoc(char *delimiter, t_shell *shell);
void execute_pipeline(t_shell *shell, char **envp);
void	wait_all(int last_pid, t_shell *shell);
void ft_echo(char **args);
void ft_cd(t_shell *shell, char **args);
void ft_pwd(t_shell *shell, char **args);
void ft_export(t_shell *shell, char **args);
void ft_unset(t_env **env, char **args);
int ft_exit(t_shell *shell, char **args);
void ft_env(t_env *env, int export);
int is_builtin(char *cmd);
void redirect_output_builtin(t_cmd *cmd, int append);
int execute_builtin(t_shell *shell, char *cmd, char **args);
char *find_env_node(t_env *env, char *key);

#endif
