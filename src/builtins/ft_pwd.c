#include "../../include/minishell.h"
void ft_pwd(t_shell *shell, char **args)
{
    char *dir;

    (void)shell; // إذا لم تستخدمه

    // تحقق من أن args موجود وأن أول عنصر ليس NULL
    if (!args || !args[0])
        return;

    if (strcmp(args[0], "pwd") != 0)
        return;

    dir = getcwd(NULL, 0);
    if (dir == NULL)
    {
        printf("%s", find_env_node(shell->env, "PWD"));
        printf("\n");
        // perror("minishell: pwd");
        return;
    }

    printf("%s\n", dir);
    free(dir);
}
