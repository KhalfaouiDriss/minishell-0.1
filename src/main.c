#include "../include/minishell.h"

void get_sig(int sig)
{
    if (sig == SIGINT)
    {
        write(1, "\n", 1); 
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

int main(int ac, char **av, char **envp)
{
    t_shell shell;
    int exit_status = 0;

    (void)ac;
    (void)av;

    init_shell(&shell);
    signal(SIGINT, get_sig);
    signal(SIGQUIT, SIG_IGN);
    char *pwd = getcwd(NULL, 0);
    if (!pwd)
    {
        perror("getcwd");
        exit(1);
    }
    
    const char *prefix = "\033[0;32mminishell-1337-rabat \033[0m";
    int size = ft_strlen(prefix) + 3;

    char *mini = malloc(size);

    ft_strlcpy(mini, prefix, size);
    ft_strlcat(mini, "\n ", size);
    while (1)
    {
        shell.input = readline(mini);
        if (!shell.input)
            break;
        if (shell.input == NULL)
        {
            free_all(&shell);
            printf("exit\n");
            exit(1);
        }

        if (shell.input[0] != '\0')
            add_history(shell.input);
        
        shell.token = lexer_split_to_tokens(shell.input);
        t_cmd *cmd_list = parse_tokens(shell.token);
        if (cmd_list)
            exit_status = execute_pipeline(cmd_list, envp);

        free_token(shell.token);
        free_cmds(cmd_list);
        free(shell.input);
    }

    // free_all(&shell);
    return 0;
}
