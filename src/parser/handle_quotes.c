#include "../../include/minishell.h"

void	set_quote_type_explicitly(t_lexer_state *state, char quote)
{
	if (quote == '"')
		state->current_quote_type = D_QUOTE;
	else if (quote == '\'')
		state->current_quote_type = S_QUOTE;
}

void	handle_invalid_quote(t_shell *shell, t_lexer_state *state)
{
	add_token(&state->head, new_token(&(shell->ebag), "Invalid quote", 0,
			QUETS_INVA));
	shell->exit_status = 258;
}

static void	handle_empty_quotes(t_shell *shell, t_lexer_state *state)
{
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
		i++;
	}
	return (result);
}

static void	append_expanded_or_raw(t_shell *shell, t_lexer_state *state,
		char *tmp, char quote)
{
	char	*expanded;

	if (quote == '\'' && tmp[0] != '$')
		state->current_word = strjoin_free(state->current_word, tmp);
	else
	{
		expanded = expand_variables_in_string(tmp, shell, quote);
		state->current_word = strjoin_free(state->current_word, expanded);
	}
}

void	handle_quotes(t_shell *shell, t_lexer_state *state)
{
	state->quote = state->str[state->i];
	set_quote_type_explicitly(state, state->quote);
	state->i++;
	state->start = state->i;
	while (state->str[state->i] && state->str[state->i] != state->quote)
		state->i++;
	if (state->str[state->i] == '\0')
		return (handle_invalid_quote(shell, state));
	state->tmp = ft_substr(state->str, state->start, state->i - state->start);
	handle_empty_quotes(shell, state);
	append_expanded_or_raw(shell, state, state->tmp, state->quote);
	state->i++;
	state->current_quote_type = 0;
}