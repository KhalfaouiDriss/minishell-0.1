/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenized.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 17:18:45 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/25 18:43:13 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	process_token_loop(t_shell *shell, t_lexer_state *state)
{
	while (state->str[state->i] && is_space(state->str[state->i]))
		state->i++;
	if (state->str[state->i] == '\0')
		return ;
	state->start = state->i;
	while (state->str[state->i] && !is_space(state->str[state->i]))
	{
		if (state->str[state->i] == '\'' || state->str[state->i] == '"')
			handle_quotes(shell, state);
		else if (is_special(state->str[state->i]))
			handle_special_token_case(shell, state);
		else if (state->str[state->i] == '$')
			handle_dollar_sign(shell, state);
		else
			handle_normal_word(state);
	}
	finalize_current_word(shell, state);
}

int	check_initial_dollar_error(t_shell *shell, t_lexer_state *state)
{
	if (state->str[0] == '$' && (!state->str[1] || state->str[1] == ' '))
	{
		add_token(&state->head, new_token(&(shell->ebag),
				"$", WORD, 0));
		shell->exit_status = 127;
		return (1);
	}
	return (0);
}

void	init_lexer_vars(t_shell *shell, t_lexer_state *state)
{
	state->i = 0;
	state->j = 0;
	state->t_tmp = NULL;
	state->head = NULL;
	state->current_word = NULL;
	state->current_quote_type = 0;
	state->token_type = 0;
	state->str = shell->input;
	state->start = 0;
}

t_token	*lexer_split_to_tokens(t_shell *shell)
{
	t_lexer_state	state;

	if (check_syntax_errors(shell, shell->input, 1))
		return (NULL);
	init_lexer_vars(shell, &state);
	while (state.str[state.i])
		process_token_loop(shell, &state);
	return (state.head);
}
