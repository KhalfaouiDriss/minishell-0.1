#include "../include/minishell.h"

int		blocked = 0;

void	get_sig(int sig)
{
	if (blocked == 1)
		return ;
	if (sig == SIGINT)
	{
		blocked = 2;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	// if (sig == 12)
	// {
	// 	// printf("llll");
	// 	rl_replace_line("", 0);
	// 	rl_on_new_line();
	// 	rl_catch_signals = 0;
	// 	rl_replace_line("", 0);
	// 	rl_redisplay();
	// }
}

void	init_env(t_shell *shell, char **envp)
{
	int		i;
	char	*name;
	char	*value;
	char	*egl;
	t_env	*new_env;
	t_env	*last;

	i = 0;
	shell->env = NULL;
	while (envp[i])
	{
		egl = ft_strchr(envp[i], '=');
		if(egl)
		{
			name = ft_substr(envp[i], 0, ft_strlen(envp[i]) - ft_strlen(egl));
			value = egl + 1;
		}
		if (!name)
			return ;
		new_env = malloc(sizeof(t_env));
		if (!new_env)
			return ;
		new_env->name = ft_strdup(name);
		new_env->value = ft_strdup(value);
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
		free(name);
		i++;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;
	int		exit_status;
	char	*pwd;

	shell.exit_status = 0;
	(void)ac;
	(void)av;
	init_shell(&shell);
	signal(SIGINT, get_sig);
	signal(SIGQUIT, SIG_IGN);
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror("getcwd");
		exit(1);
	}
	free(pwd);
	init_env(&shell, envp);
	while (1)
	{
		if (shell.blocked == 12)
		{
			get_sig(12);
		}
		shell.input = readline("minishell-sendo-C47 $/~ ");
		if (!shell.input)
		{
			printf("exit\n");
			free_all(&shell);
			free_env(shell.env);
			break ;
		}
		if (shell.input[0] != '\0')
			add_history(shell.input);
		if (blocked == 2)
			shell.exit_status = 130;
		shell.token = lexer_split_to_tokens(&shell);
		shell.cmd_list = parse_tokens(&shell);
		if (!shell.cmd_list)
		{
			free_all(&shell);
			continue ;
		}
		blocked = 1;
		if (shell.cmd_list)
			execute_pipeline(&shell, envp);
		blocked = 0;
		free_all(&shell);
	}
	return (0);
}
