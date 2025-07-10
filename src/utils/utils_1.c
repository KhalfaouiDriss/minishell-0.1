#include "../../include/minishell.h"

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
    shell->pip_count = 0;
	shell->is_heredoc_delimiter = 0;
	shell->ebag = -1;
	shell->ebag_final = -1;
}

void gc_remove(void *ptr)
{
	t_gc *gc = get_gc();
	t_mlc *curr = gc->head;
	t_mlc *prev = NULL;

	while (curr)
	{
		if (curr->ptr == ptr)
		{
			if (prev)
				prev->next = curr->next;
			else
				gc->head = curr->next;
			free(curr->ptr);
			free(curr);
			return;
		}
		prev = curr;
		curr = curr->next;
	}
}


char *strjoin_free(char *s1, char *s2)
{
	char *new_str;

	if (!s1 && !s2)
		return NULL;
	if (!s1)
		return s2;
	if (!s2)
		return s1;

	new_str = ft_strjoin(s1, s2);
	gc_remove(s1);
	return new_str;
}


int ft_nodelen(t_token *head)
{
	int i;
	t_token *tmp;

	tmp = head;
	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return i;
}

int is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}


int is_special(char c)
{
    return (c == '|' || c == '>' || c == '<');
}
