#include "../include/minishell.h"

int		o = 0;
void	get_sig(int sig)
{
	if (sig == SIGINT)
	{
		if (o == 0)
			return ;
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;
	int		exit_status;
	char	*pwd;
		const char *prefix = "\033[0;32mminishell-1337-rabat $ \033[0m";
	int		size;
	char	*mini;
	t_cmd	*cmd_list;

	exit_status = 0;
	(void)ac;
	(void)av;
	if (ac == 1)
	{
		init_shell(&shell);
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
		ft_strlcpy(mini, prefix, size);
		while (1)
		{
			shell.input = readline(mini);
			if (!shell.input)
				break ;
			if (shell.input == NULL)
			{
				free_all(&shell);
				exit(1);
			}
			if (shell.input[0] != '\0')
				add_history(shell.input);
			shell.token = lexer_split_to_tokens(shell.input);
			cmd_list = parse_tokens(shell.token);
			o = 0;
			if (cmd_list)
				exit_status = execute_pipeline(cmd_list, envp);
			o = 1;
			free_token(shell.token);
			free_cmds(cmd_list);
			free(shell.input);
		}
	}
    else
    {
        printf("Invalid args\n");
    }
	// free_all(&shell);
	return (0);
}
