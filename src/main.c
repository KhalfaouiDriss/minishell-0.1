#include "../include/minishell.h"

int main(int ac, char **av, char **envp)
{
    t_shell shell;
    int exit_status = 0;
    
    (void)ac;
    (void)av;
    
    init_shell(&shell);
    
    while (1)
    {
        shell.input = readline("minishell$ ");
        if (!shell.input)
            break;
        
        if (shell.input[0] != '\0')
            add_history(shell.input);
            
        shell.token = lexer_split_to_tokens(shell.input);
        t_cmd *cmd_list = parse_tokens(shell.token);
        if (cmd_list)
            exit_status = execute_pipeline(cmd_list, envp);
        
        free_tokens(shell.token);
        free_cmds(cmd_list);
        free(shell.input);
    }
    
    free_all(&shell);
    return exit_status;
}