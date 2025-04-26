#include "../include/minishell.h"

int		blocked = 0;

void	get_sig(int sig)
{
	if (blocked)
		return ;
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		write(1, "\n", 1);
		rl_redisplay();
	}
}

void	init_env(t_shell *shell, char **envp)
{
	int		i;
	char	**variable;
	t_env	*new_env;
	t_env	*last;

	i = 0;
	shell->env = NULL;
	while (envp[i])
	{
		variable = ft_split(envp[i], '=');
		if (!variable)
			return ;
		new_env = malloc(sizeof(t_env));
		if (!new_env)
			return ;
		new_env->name = ft_strdup(variable[0]);
		new_env->value = ft_strdup(variable[1]);
		new_env->next = NULL;
		if (!shell->env)
			shell->env = new_env;
		else
		{
			last = shell->env;
			while (last->next)
				last = last->next;
			last->next = new_env;
		}
		free_split(variable);
		i++;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_shell		shell;
	int			exit_status;
	char		*pwd;
	const char	*prefix = "\033[0;32mminishell-sendo-C47 $/~ \033[0m";
	int			size;
	char		*mini;

	exit_status = 0;
	(void)ac;
	(void)av;
	init_shell(&shell);
	rl_catch_signals = 0;
	signal(SIGINT, get_sig);
	signal(SIGQUIT, SIG_IGN);
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror("getcwd");
		exit(1);
	}
	size = ft_strlen(prefix) + 1;
	mini = malloc(size);
	while (1)
	{
		init_env(&shell, envp);
		shell.input = readline(prefix);
		if (!shell.input)
		{
			printf("exit\n");
			break ;
		}
		if (shell.input[0] != '\0')
			add_history(shell.input);
		shell.token = lexer_split_to_tokens(&shell);
		shell.cmd_list = parse_tokens(&shell);
		blocked = 1;
		
		if (shell.cmd_list)
			exit_status = execute_pipeline(&shell, envp);
		blocked = 0;
		free_token(shell.token);
		free_cmds(shell.cmd_list);
		free(shell.input);
	}
	// free_all(&shell);
	return (0);
}
