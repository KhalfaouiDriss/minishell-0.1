/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 22:10:08 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/18 18:30:14 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

# define ERROR 0
# define WORD 1
# define PIPE 2
# define REDIR_IN 3
# define REDIR_OUT 4
# define REDIR_APPEND 5
# define REDIR_HEREDOC 6
# define OPTION 7
# define VARIABLE 8
# define EXEC 10
# define NOT_FOUND 11
# define INVALID_DIR 12
# define QUETS_INVA 13
# define OPTION_INVA 14
# define INPUT_INVA 15
# define D_QUOTE 16
# define S_QUOTE 17
# define SYT_ERR "mshell: syntax error near unexpected token"

typedef struct s_error
{
	char			*invalaid_token;
	int				error_type;
}					t_error;

typedef struct s_token
{
	char			*value;
	int				type;
	int				quot_type;
	struct s_token	*next;
	struct s_token	*prev;
	int				error;
	int				ebag;
}					t_token;

typedef struct s_cmd
{
	char			**args;
	char			*infile;
	char			*outfile;
	int				outfile_fd;
	int				append;
	char			*heredoc;
	int				infile_fd;
	int				heredoc_fd;
	int				c_flag;
	int				fd_builtin;
	int				flag_amb;
	int				fod_flag;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_env
{
	char			*name;
	char			*value;
	void			*next;
}					t_env;

typedef struct s_shell
{
	char			**args;
	int				arg_count;
	char			*input;
	t_token			*token;
	t_env			*env;
	t_cmd			*cmd_list;
	int				exit_status;
	int				blocked;
	int				pip_count;
	int				ebag;
	int				ebag_final;
	int				is_heredoc_delimiter;
	char			**new_env;
	int				not_found;
	int				pos;
	int				in;
	int				out;
	int				exp;
	int				her_fd;
}					t_shell;

typedef struct s_lexer_state
{
	int				i;
	int				j;
	int				start;
	int				current_quote_type;
	int				token_type;
	char			*str;
	char			*current_word;
	t_token			*t_tmp;
	t_token			*head;
	char			*tmp;
	char			quote;
	int				opt_start;
	int				quoted_start;
	char			*val;
	char			*content;
	const char		*input;

}					t_lexer_state;

// galbege_collecter

// ptr -> list

typedef struct s_mlc
{
	void			*ptr;
	struct s_mlc	*next;
}					t_mlc;

typedef struct s_gc
{
	t_mlc			*head;
}					t_gc;

void				*ft_malloc(size_t size);
void				gc_free_all(void);
t_gc				*get_gc(void);

// tokenized
t_token				*lexer_split_to_tokens(t_shell *shell);
char				*handle_variable_token(char *str, int *i, t_shell *shell,
						char qte);
int					handle_option_token(t_shell *shell, const char *input,
						int *i, t_token **head);
void				handle_special_token(t_shell *shell, const char *input,
						int *i, t_token **head);

// tokenized Utils
void				print_tokens(t_token *head);
void				correct_lexer(t_shell *shell, t_token **token);
int					pips_coount(char *input);
void				set_quote_type_explicitly(t_lexer_state *state, char quote);
int					skip_spaces(char *str, int i);
int					check_embag(char *var_value);

// utils_1.c
void				init_shell(t_shell *shell);
char				*strjoin_free(char *s1, char *s2);
int					ft_nodelen(t_token *head);
int					is_space(char c);
int					is_special(char c);

//  utils_variable.c
int					is_embg_befor(t_shell *shell, int i);
int					last_is_redir(t_shell *shell, int i);
char				*handle_special_cases_utile(char *str, int *i,
						t_shell *shell);
int					is_opened_quote(char *str, int i);

// ft_cd.c
char				*get_env_value(t_env *env, const char *name);
void				add_new_env_var(t_env *env, const char *name,
						const char *value);
void				update_env_var(t_env *env, const char *name,
						const char *value);
void				handle_cd_args_error(t_shell *shell);
void				ft_cd(t_shell *shell, char **args);

// builtins_utils_1.c
const char			*get_target_dir(t_shell *shell, char **args);
void				handle_cd_oldpwd_failure(t_shell *shell);
int					check_cd_errors(t_shell *shell, const char *target_dir);
void				handle_cd_chdir_fail(t_shell *shell, const char *target_dir,
						char *oldpwd);
void				update_pwd_after_cd(t_shell *shell);

// builtins_utils_1.c
void				add_env_node(t_shell *shell, t_env **env, char *key,
						char *value);

// init_env.c
void				add_env(t_shell *shell, char *name, char *value);
void				process_env_variable(t_shell *shell, char *env_str);
void				init_env(t_shell *shell, char **envp);
void				init_new_env(t_shell *shell);

// signals_hand.c
int					global_state(int set);
void				get_sig(int sig);
void				handel_sig(int sig);
char				*get_variable(t_shell *shell, char *line, int *i);

// free_utils.c
void				free_split(char **lst);
void				free_tokens(t_token *token);
void				free_cmds(t_cmd *cmds);
void				free_env(t_env *env);
void				free_all(t_shell *shell, int enve);

// linkd_list_utils
void				add_token(t_token **head, t_token *new);
t_token				*new_token(int *ebag, char *val, int type, int error_type);
t_token				*new_node(char *value);
char				*find_env_node(t_env *env, char *key);

// parser
void				handle_quotes(t_shell *shell, t_lexer_state *state);
void				handle_special_token_case(t_shell *shell,
						t_lexer_state *state);
void				handle_normal_word(t_lexer_state *state);
void				finalize_current_word(t_shell *shell, t_lexer_state *state);
void				handle_dollar_sign(t_shell *shell, t_lexer_state *state);

t_cmd				*parse_tokens(t_shell *shell);
int					clean_exit(t_shell *shell, int fexit);
void				handle_ambiguous(t_shell *shell);
void				close_parent_fds(t_cmd *cmd, int prev_pipe);
char				*find_command_path(char *cmd, t_env *envp);
char				*full_paths(char *path, char *cmd);
void				sig_val(int sig, int *sigquit, int *sigint);
void				affiche_sig(int sigquit, int sigint);
void				execve_fail(t_cmd *cmd);
int					builtin_free_exit(t_shell *shell, t_cmd *cmd);
void				print_not_found_and_exit(t_cmd *cmd, t_shell *shell);
void				handle_exec_errors(char *path, t_cmd *cmd, t_shell *shell);
void				handle_signals_and_exit_cases(t_shell *shell, t_cmd *cmd);
void				dupping2(int fd, int a);
void				dupping(int in, int out);
void				handle_pipes_and_fds(t_cmd *cmd, int prev_pipe, int *fd);
void				init_str(t_cmd *cmd);
void				red_out(t_shell *shell, t_cmd *cmd, t_token *token);
int					count_args(t_token *token);
int					handle_heredoc(char *delimiter, t_shell *shell);
void				run_heredoc_loop(int tmp_fd, char *delimiter,
						t_shell *shell);
void				write_expanded_line(char *line, t_shell *shell, int tmp_fd);
int					handle_builtin_redirs(t_cmd *cmd, t_shell *shell);
char				*safe_strdup(char *s);
void				redirect_input(char *file, t_cmd *cmd);
void				redirect_output(t_cmd *cmd, int append);
int					handle_heredoc(char *delimiter, t_shell *shell);
void				execute_pipeline(t_shell *shell);
void				wait_all(int last_pid, t_shell *shell);
void				ft_echo(char **args);
void				ft_cd(t_shell *shell, char **args);
void				ft_pwd(t_shell *shell, char **args);
void				ft_export(t_shell *shell, char **args);
void				ft_unset(t_env **env, char **args);
int					ft_exit(t_shell *shell, char **args);
void				ft_env(t_env *env, int export);
int					is_builtin(char *cmd);
void				redirect_output_builtin(t_cmd *cmd, int append);
int					execute_builtin(t_shell *shell, char *cmd, char **args);

char				*ft_strdupv2(const char *str);
void				free_new_env(char **env);
void				clean_shell(t_shell *shell);
int					is_quote(char c);
t_shell				*get_shell(void);
int					check_syntax_errors(t_shell *shell, char *input, int pos);
int					*fake_glb(void);
int					her_red(t_cmd *cmd, t_token *token,t_shell *shell);
void				in_red(t_cmd *cmd, t_token *token);
#endif
