/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_normal_word.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khalfaoui47 <khalfaoui47@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 17:18:09 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/25 06:09:11 by khalfaoui47      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	finalize_current_word(t_shell *shell, t_lexer_state *state)
{
	if (state->current_word)
	{
		if (state->current_word[0] == '$')
			state->token_type = VARIABLE;
		else if (state->current_word[0] == '-'
			&& ft_strlen(state->current_word) >= 2)
			state->token_type = OPTION;
		else
			state->token_type = WORD;
		append_to_token(shell, state, WORD);
	}
}

void	handle_normal_word(t_lexer_state *state)
{
	char	*tmp;

	state->start = state->i;
	while (state->str[state->i] && !is_space(state->str[state->i])
		&& state->str[state->i] != '\'' && state->str[state->i] != '"'
		&& !is_special(state->str[state->i]) && state->str[state->i] != '$')
		state->i++;
	tmp = ft_substr(state->str, state->start, state->i - state->start);
	state->current_word = strjoin_free(state->current_word, tmp);
}
