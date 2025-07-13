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
				"minishell: '$' command not found", 0, NOT_FOUND));
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
	shell->pip_count = pips_coount(shell->input);
}

t_token	*lexer_split_to_tokens(t_shell *shell)
{
	t_lexer_state	state;
	
	if (check_syntax_errors(shell))
		return NULL;
	init_lexer_vars(shell, &state);
	if (check_initial_dollar_error(shell, &state))
		return (state.head);
	while (state.str[state.i])
		process_token_loop(shell, &state);
	// correct_lexer(shell, &state.head);
	// print_tokens(state.head);
	return (state.head);
}
