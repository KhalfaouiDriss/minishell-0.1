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