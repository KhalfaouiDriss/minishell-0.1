#include "../../include/minishell.h"

// ================================================================

// void	handle_invalid_option(t_shell *shell, t_token **head)
// {
// 	char	*err_str;

// 	err_str = ft_strdup("Error: Invalid option");
// 	add_token(head, new_token(&(shell->ebag), err_str, ERROR, OPTION_INVA));
// 	shell->exit_status = 2;
// }

// void	detect_token_type(const char *input, char *final, int *type, int *error)
// {
// 	if (final && final[0] == '-' && ft_strlen(final) >= 2)
// 		*type = OPTION;
// 	if (final && input[1] == ' ')
// 	{
// 		*type = ERROR;
// 		*error = OPTION_INVA;
// 	}
// }

// void	handle_unmatched_quote(t_shell *shell, char *segment, char *final)
// {
// 	t_token	**head;
// 	char	*err_str;

// 	head = &(shell->token);
// 	err_str = ft_strdup("Error: Unmatched quote");
// 	add_token(head, new_token(&(shell->ebag), err_str, ERROR, QUETS_INVA));
// 	shell->exit_status = 2;
// }

// int	fill_segment(const char *input, int *i, char *segment, char quote)
// {
// 	int	j;

// 	j = 0;
// 	while (input[*i])
// 	{
// 		if (input[*i] == quote)
// 			break ;
// 		if (quote == '"' && input[*i] == '\\' && input[*i + 1])
// 		{
// 			(*i)++;
// 			segment[j++] = input[(*i)++];
// 		}
// 		else
// 		{
// 			segment[j++] = input[(*i)++];
// 		}
// 	}
// 	return (j);
// }

// ================================================================




// static int	handle_quoted_error(t_shell *shell, t_token **head,
// 		t_lexer_state *tmp, int *i)
// {
// 	tmp->val = ft_substr(tmp->input, tmp->start, *i);
// 	if (!tmp->input[*i])
// 	{
// 		add_token(head, new_token(&(shell->ebag), tmp->val, ERROR, QUETS_INVA));
// 		return (0);
// 	}
// 	if (tmp->input[*i] == ' ')
// 	{
// 		add_token(head, new_token(&(shell->ebag), tmp->val, ERROR,
// 				OPTION_INVA));
// 		return (0);
// 	}
// 	return (1);
// }

// static void	build_quoted_value(t_lexer_state *tmp)
// {
// 	tmp->val = ft_malloc(ft_strlen(tmp->content) + 2);
// 	if (tmp->val)
// 	{
// 		tmp->val[0] = '-';
// 		ft_strlcpy(&tmp->val[1], tmp->content, ft_strlen(tmp->content) + 1);
// 	}
// }

// static int	process_quoted(t_shell *shell, int *i, t_token **head,
// 		t_lexer_state *tmp)
// {
// 	tmp->quote = tmp->input[(*i)++];
// 	tmp->quoted_start = *i;
// 	while (tmp->input[*i] && tmp->input[*i] != tmp->quote
// 		&& tmp->input[*i] != ' ')
// 		(*i)++;
// 	if (!handle_quoted_error(shell, head, tmp, i))
// 		return (0);
// 	tmp->content = ft_substr(tmp->input, tmp->quoted_start, *i
// 			- tmp->quoted_start);
// 	build_quoted_value(tmp);
// 	(*i)++;
// 	return (1);
// }

// static int	process_unquoted(t_shell *shell, int *i, t_token **head,
// 		t_lexer_state *tmp)
// {
// 	tmp->opt_start = *i;
// 	if (tmp->input[*i + 1] == ' ')
// 	{
// 		add_token(head, new_token(&(shell->ebag), tmp->val, 0, OPTION_INVA));
// 		return (0);
// 	}
// 	while (tmp->input[*i] && !(tmp->input[*i] == ' ')
// 		&& !is_special(tmp->input[*i]))
// 		(*i)++;
// 	tmp->val = ft_substr(tmp->input, tmp->start, *i - tmp->start);
// 	return (1);
// }

// int	handle_option_token(t_shell *shell, const char *input, int *i,
// 		t_token **head)
// {
// 	t_lexer_state	*tmp;

// 	tmp->input = input;
// 	tmp = ft_malloc(sizeof(t_lexer_state));
// 	tmp->start = (*i)++;
// 	tmp->val = NULL;
// 	if (is_quote(input[*i]))
// 	{
// 		if (!process_quoted(shell, i, head, tmp))
// 			return (0);
// 	}
// 	else
// 	{
// 		if (!process_unquoted(shell, i, head, tmp))
// 			return (0);
// 	}
// 	if (tmp->val)
// 		add_token(head, new_token(&(shell->ebag), tmp->val, OPTION, 0));
// 	return (1);
// }

// =========================================================================

// void	handle_word_token(t_shell *shell, const char *input, int *i,
// 		t_token **head)
// {
// 	int		start;
// 	char	*buffer;
// 	int		j;

// 	start = *i;
// 	buffer = ft_malloc(ft_strlen(input) + 1);
// 	j = 0;
// 	if (!buffer)
// 		return ;
// 	while (input[*i] && input[*i] != ' ' && !is_special(input[*i])
// 		&& input[*i] != '"' && input[*i] != '\'')
// 		buffer[j++] = input[(*i)++];
// 	buffer[j] = '\0';
// 	if (j > 0)
// 		add_token(head, new_token(&(shell->ebag), buffer, WORD, 0));
// }

// =================================================================

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
