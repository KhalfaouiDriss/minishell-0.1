#include "../../include/minishell.h"

void	free_split(char **lst)
{
	int	i;

	i = 0;
	if (!lst)
		return ;
	while (lst[i])
	{
		free(lst[i]);
		i++;
	}
	free(lst);
}


void init_shell(t_shell *shell)
{
    shell->arg_count = 0;
    shell->args = NULL;
    shell->input = NULL;
    shell->token = NULL;
    shell->cmd_list = NULL;
    shell->exit_status = 0;
    shell->env = NULL;
    shell->input = NULL;
}

void free_tokens(t_token *tokens)
{
    t_token *tmp;

    while (tokens)
    {
        tmp = tokens->next;
        if (tokens->value)
            free(tokens->value);
        free(tokens);
        tokens = tmp;
    }
}

void free_env(t_env *env)
{
    t_env *tmp;

    while (env)
    {
        tmp = env->next;
        if (env->name)
            free(env->name);
        if (env->value)
            free(env->value);
        free(env);
        env = tmp;
    }
}


void free_all(t_shell *shell)
{
    if (!shell)
        return;

    if (shell->input)
    {
        free(shell->input);
        shell->input = NULL;
    }

    if (shell->token)
    {
        free_tokens(shell->token);
        shell->token = NULL;
    }

    if (shell->args)
    {
        free_split(shell->args);
        shell->args = NULL;
    }
    if (shell->cmd_list)
    {
        free_cmds(shell->cmd_list);
        shell->cmd_list = NULL;
    }

}


t_token	*new_node(char *value)
{
	t_token	*new;

	new = malloc(sizeof(*new));
	if (!new)
		return (NULL);
    new->value = value;
	new->next = NULL;
	return (new);
}
