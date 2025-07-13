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

void	handle_dollar_variable_expansion(t_shell *shell, t_lexer_state *state,
		int j)
{
	char	*value;
	char	*tmp2;
	char	*tmp3;

	value = handle_variable_token(state->str, &state->i, shell, 0);
	if (value)
	{
		if (ft_strncmp(value, "$", 1) == 0)
		{
			state->current_word = strjoin_free(state->current_word, value);
			state->i++;
		}
		else if(value && ft_strlen(value) == 0)
		{
			state->current_word = strjoin_free(state->current_word, "");
			state->i++;
		}
		else
		{
			tmp2 = ft_substr(state->str, 0, j);
			tmp3 = ft_substr(state->str, state->i, ft_strlen(state->str)
			- state->i);
			shell->input = ft_strjoin(tmp2, value);
			tmp2 = shell->input;
			shell->input = ft_strjoin(shell->input, tmp3);
			state->str = shell->input;
			state->i = j;
		}
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