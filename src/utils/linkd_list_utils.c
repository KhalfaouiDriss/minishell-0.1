/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linkd_list_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khalfaoui47 <khalfaoui47@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 20:36:44 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/25 06:11:47 by khalfaoui47      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_special(char c)
{
	return (c == '|' || c == '>' || c == '<');
}

// t_token	*new_node(char *value)
// {
// 	t_token	*new;

// 	new = ft_malloc(sizeof(*new));
// 	if (!new)
// 		return (NULL);
// 	new->value = value;
// 	new->next = NULL;
// 	return (new);
// }

void	append_to_token(t_shell *shell, t_lexer_state *state, int type)
{
	state->t_tmp = new_token(&(shell->ebag), state->current_word, type, 0);
	state->t_tmp->quot_type = state->current_quote_type;
	add_token(&state->head, state->t_tmp);
	state->current_word = NULL;
}

t_token	*new_token(int *ebag, char *val, int type, int error_type)
{
	t_token	*t;

	t = (t_token *)ft_malloc(sizeof(t_token));
	if (!t)
		return (NULL);
	if (val)
	{
		t->value = ft_strdup(val);
		if (!t->value)
			return (NULL);
	}
	else
	{
		t->value = NULL;
	}
	t->ebag = *ebag;
	t->type = type;
	t->error = error_type;
	t->next = NULL;
	*ebag = 1;
	return (t);
}

void	add_token(t_token **head, t_token *new)
{
	t_token	*tmp;

	if (!new)
		return ;
	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

char	*find_env_node(t_env *env, char *key)
{
	t_env	*tmp;
	size_t	key_len;

	tmp = env;
	key_len = ft_strlen(key);
	while (tmp)
	{
		if (ft_strncmp(tmp->name, key, key_len) == 0
			&& tmp->name[key_len] == '\0')
		{
			return (tmp->value);
		}
		tmp = tmp->next;
	}
	return (NULL);
}
