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

// void init_env(t_shell *shell, char **env)
// {

// }

void init_shell(t_shell *shell)
{
    shell->arg_count = 0;
    shell->args = NULL;
    shell->input = NULL;
    shell->token = NULL;
    // init_env(&(shell->env), env);
}

void free_token(t_token *token)
{
    t_token *head;
    t_token *next;

    head = token;
    while (head)
    {
        next = head->next;
        free(head);
        if (head->value)
            free(head->value);
        head = next;
    }
    
}

void free_all(t_shell *shell)
{
    if(shell->input)
        free(shell->input);
    if(shell->token)
        free_token(shell->token);
    if(shell->args)
        free_split(shell->args);
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