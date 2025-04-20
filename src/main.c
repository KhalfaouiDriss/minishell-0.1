#include "../include/minishell.h"
#include "../include/minishell.h"

int main(int ac, char **av, char **env)
{
    t_shell shell;

    (void)ac;
    (void)av;
    (void)env;

    init_shell(&shell);

    while (1)
    {
        shell.input = readline("minishell$/~ ");
        if (!shell.input)
        {
            printf("exit\n");
            free_all(&shell);
            exit(0);
        }

        if (shell.input[0] == '\0' || is_all_space(shell.input))
        {
            free(shell.input);
            continue;
        }

        shell.token = lexer_split_to_tokens(shell.input);
        if (!shell.token)
        {
            // fprintf(stderr, "Lexer Error: Invalid input or quotes not closed.\n");
            free(shell.input);
            continue;
        }

        print_tokens(shell.token); // فقط للـ Debug حالياً
        add_history(shell.input);

        // هنا مستقبلاً: parse → exec → cleanup

        free_token(shell.token);
        shell.token = NULL;
        free(shell.input);
        shell.input = NULL;
    }
    return 0;
}
