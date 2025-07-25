/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_normal_word.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 17:18:09 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/25 18:07:39 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	finalize_current_word(t_shell *shell, t_lexer_state *state)
{
	if (state->current_word)
		append_to_token(shell, state, WORD);
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
