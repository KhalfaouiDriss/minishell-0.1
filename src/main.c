#include "../include/minishell.h"


void main_loop(t_shell *shell, char **envp)
{
	while (1)
	{
		global_state(0);
		shell->input = readline("➜ Minishell $/~ ");
		if (global_state(-1) == 2)
			shell->exit_status = 130;
		if (!shell->input)
		{
			free(shell->input);
			gc_free_all();
			printf("exit\n");
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
	}
	return;
}

int	main(int ac, char **av, char **envp)
{
	t_shell shell;
	char *pwd;
	int i = 0;

	shell.not_found = 0;
	(void)ac;
	(void)av;
	init_shell(&shell);
	init_env(&shell, envp);
	signal(SIGINT, get_sig);
	signal(SIGTSTP, SIG_IGN); 
	signal(SIGQUIT, SIG_IGN);
	main_loop(&shell, envp);
	return 0;
}

