/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_special_token.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khalfaoui47 <khalfaoui47@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 17:18:26 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/14 16:36:32 by khalfaoui47      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	get_type(char *val, int type)
{
	if (ft_strncmp(val, "|", 1) == 0)
		type = PIPE;
	else if (ft_strncmp(val, ">>", 2) == 0)
		type = REDIR_APPEND;
	else if (ft_strncmp(val, "<<", 2) == 0)
		type = REDIR_HEREDOC;
	else if (ft_strncmp(val, "<", 1) == 0)
		type = REDIR_IN;
	else if (ft_strncmp(val, ">", 1) == 0)
		type = REDIR_OUT;
	else
		type = ERROR;
	return (type);
}

void	handle_special_token(t_shell *shell, const char *input, int *i,
		t_token **head)
{
	int		start;
	int		type;
	char	*val;

	start = *i;
	if ((input[*i] == '<' || input[*i] == '>') && input[*i + 1] == input[*i])
		*i += 2;
	else if (input[*i] == '|' || input[*i] == '<' || input[*i] == '>')
		(*i)++;
	else
	{
		(*i)++;
		val = ft_substr(input, start, *i - start);
		add_token(head, new_token(&(shell->ebag), val, ERROR, 0));
		return ;
	}
	val = ft_substr(input, start, *i - start);
	type = get_type(val, type);
	if(shell->not_found == 2)
	{
		type = WORD;
		shell->not_found = 0;
	}
	if (type == REDIR_HEREDOC)
		shell->is_heredoc_delimiter = 1;
	add_token(head, new_token(&(shell->ebag), val, type, 0));
	return ;
}

void	handle_special_token_case(t_shell *shell, t_lexer_state *state)
{
	if (state->current_word)
	{
		state->t_tmp = new_token(&(shell->ebag), state->current_word, WORD, 0);
		state->t_tmp->quot_type = state->current_quote_type;
		add_token(&state->head, state->t_tmp);
		state->current_word = NULL;
	}
	handle_special_token(shell, state->str, &state->i, &state->head);
}
