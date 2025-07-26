/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 20:37:04 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/25 17:42:24 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_shell(t_shell *shell)
{
	shell->arg_count = 0;
	shell->args = NULL;
	shell->input = NULL;
	shell->token = NULL;
	shell->cmd_list = NULL;
	shell->exit_status = 0;
	shell->env = NULL;
	shell->input = NULL;
	shell->is_heredoc_delimiter = 0;
	shell->ebag = -1;
	shell->ebag_final = -1;
	shell->exp = 1;
	shell->is_dir = 0;
}

void	gc_remove(void *ptr)
{
	t_gc	*gc;
	t_mlc	*curr;
	t_mlc	*prev;

	gc = get_gc();
	curr = gc->head;
	prev = NULL;
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
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

char	*strjoin_free(char *s1, char *s2)
{
	char	*new_str;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (s2);
	if (!s2)
		return (s1);
	new_str = ft_strjoin(s1, s2);
	return (new_str);
}

int	ft_nodelen(t_token *head)
{
	int		i;
	t_token	*tmp;

	tmp = head;
	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}
