#include "../include/minishell.h"

void	clean_env(t_env *env)
{
	t_env *tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
}

t_shell *get_shell()
{
	static t_shell shell;
	return &shell;
}

void	clean_shell(t_shell *shell)
{
	if(shell->new_env)
		free_new_env(shell->new_env);
	if(shell->env)
		clean_env(shell->env);
	gc_free_all();
	shell->new_env = NULL;
	shell->env = NULL;
}


void	main_loop(t_shell *shell)
{
	while (1)
	{
		global_state(0);
		shell->input = readline("➜ Minishell $/~ ");
		if (global_state(-1) == 2)
			shell->exit_status = 130;
		if (!shell->input)
		{
			ft_putendl_fd("exit", 1);
			clean_shell(shell);
			exit(shell->exit_status);
		}
		if (shell->input[0])
			add_history(shell->input);
		shell->token = lexer_split_to_tokens(shell);
		shell->cmd_list = parse_tokens(shell);
		if (!shell->cmd_list)
			continue;
		global_state(1);
		execute_pipeline(shell);
		free(shell->input);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_shell *shell;

	shell = get_shell();
	(void)ac;
	(void)av;
	init_shell(shell);
	init_env(shell, envp);
	signal(SIGINT, get_sig);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	main_loop(shell);
	return 0;
}
