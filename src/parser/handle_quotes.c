/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 17:18:14 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/20 10:58:05 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_invalid_quote(t_shell *shell, t_lexer_state *state)
{
	add_token(&state->head, new_token(&(shell->ebag), "Invalid quote", 0,
			QUETS_INVA));
	shell->exit_status = 258;
}

static void	handle_empty_quotes(t_shell *shell, t_lexer_state *state)
{
	(void)shell;
	if (state->start == state->i && state->str[state->i])
	{
		if (!state->current_word)
		{
			if ((state->i == 1 && ft_strlen(state->str) == 2)
				|| !state->str[state->i + 1])
				state->current_word = ft_strdup("");
			else if (state->current_word)
				state->current_word = ft_strdup("");
		}
	}
}

char	*expand_variables_in_string(char *str, t_shell *shell, char qt)
{
	int		i;
	char	*result;
	char	*tmp;
	int		start;

	i = 0;
	result = NULL;
	while (str[i])
	{
		if (str[i] == '$')
		{
			tmp = handle_variable_token(str, &i, shell, qt);
			result = strjoin_free(result, tmp);
		}
		else
		{
			start = i;
			while (str[i] && str[i] != '$')
				i++;
			tmp = ft_substr(str, start, i - start);
			result = strjoin_free(result, tmp);
		}
	}
	return (result);
}

static void	append_expanded_or_raw(t_shell *shell, t_lexer_state *state,
		char *tmp, char quote)
{
	char	*expanded;

	if (quote == '\'' && tmp[0] != '$')
	{
		state->current_word = strjoin_free(state->current_word, tmp);
	}
	else
	{
		expanded = expand_variables_in_string(tmp, shell, quote);
		state->current_word = strjoin_free(state->current_word, expanded);
	}
	state->quote = 0;
}

void	handle_quotes(t_shell *shell, t_lexer_state *state)
{
	if (shell->is_heredoc_delimiter)
		shell->exp = 0;
	state->quote = state->str[state->i];
	set_quote_type_explicitly(state, state->quote);
	state->i++;
	state->start = state->i;
	while (state->str[state->i] && state->str[state->i] != state->quote)
		state->i++;
	state->tmp = ft_substr(state->str, state->start, state->i - state->start);
	handle_empty_quotes(shell, state);
	append_expanded_or_raw(shell, state, state->tmp, state->quote);
	state->i++;
	state->current_quote_type = 0;
}
