/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar_sign.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 17:18:04 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/13 17:24:35 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_dollar_in_single_quotes(t_lexer_state *state)
{
	char	*tmp;

	state->start = state->i++;
	while (state->str[state->i] && (ft_isalnum(state->str[state->i])
			|| state->str[state->i] == '_'))
		state->i++;
	tmp = ft_substr(state->str, state->start, state->i - state->start);
	state->current_word = strjoin_free(state->current_word, tmp);
}

void	handle_empty_or_literal_dollar(t_shell *shell, t_lexer_state *state,
		char *value)
{
	if (ft_strncmp(value, "$", 1) == 0)
	{
		state->current_word = strjoin_free(state->current_word, value);
		state->i++;
	}
	else if (ft_strlen(value) == 0)
	{
		state->current_word = strjoin_free(state->current_word, "");
		state->i++;
	}
}

void	handle_non_empty_variable(t_shell *shell, t_lexer_state *state,
		char *value, int j)
{
	char	*tmp2;
	char	*tmp3;

	tmp2 = ft_substr(state->str, 0, j);
	tmp3 = ft_substr(state->str, state->i, ft_strlen(state->str) - state->i);
	shell->input = ft_strjoin(tmp2, value);
	tmp2 = shell->input;
	shell->input = ft_strjoin(shell->input, tmp3);
	state->str = shell->input;
	state->i = j;
}

void	handle_dollar_variable_expansion(t_shell *shell, t_lexer_state *state,
		int j)
{
	char	*value;

	value = handle_variable_token(state->str, &state->i, shell, 0);
	if (value)
	{
		if (ft_strncmp(value, "$", 1) == 0 || ft_strlen(value) == 0)
			handle_empty_or_literal_dollar(shell, state, value);
		else
			handle_non_empty_variable(shell, state, value, j);
	}
}

void	handle_dollar_sign(t_shell *shell, t_lexer_state *state)
{
	int	j;

	j = state->i;
	if (state->current_quote_type == S_QUOTE)
		handle_dollar_in_single_quotes(state);
	else
		handle_dollar_variable_expansion(shell, state, j);
}
