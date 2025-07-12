/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenized.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:57:22 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/12 14:39:06 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// ================================================================

void	handle_invalid_option(t_shell *shell, t_token **head)
{
	char	*err_str;

	err_str = ft_strdup("Error: Invalid option");
	add_token(head, new_token(&(shell->ebag), err_str, ERROR, OPTION_INVA));
	shell->exit_status = 2;
}

void	detect_token_type(const char *input, char *final, int *type, int *error)
{
	if (final && final[0] == '-' && ft_strlen(final) >= 2)
		*type = OPTION;
	if (final && input[1] == ' ')
	{
		*type = ERROR;
		*error = OPTION_INVA;
	}
}

void	handle_unmatched_quote(t_shell *shell, char *segment, char *final)
{
	t_token	**head;
	char	*err_str;

	head = &(shell->token);
	err_str = ft_strdup("Error: Unmatched quote");
	add_token(head, new_token(&(shell->ebag), err_str, ERROR, QUETS_INVA));
	shell->exit_status = 2;
}

int	fill_segment(const char *input, int *i, char *segment, char quote)
{
	int	j;

	j = 0;
	while (input[*i])
	{
		if (input[*i] == quote)
			break ;
		if (quote == '"' && input[*i] == '\\' && input[*i + 1])
		{
			(*i)++;
			segment[j++] = input[(*i)++];
		}
		else
		{
			segment[j++] = input[(*i)++];
		}
	}
	return (j);
}

// int	collect_single_quote(const char *input, int *i, char **final,
// char quote,
// 		t_shell *shell)
// {
// 	char	*segment;
// 	int		j;
// 	char	*tmp;

// 	segment = ft_malloc(ft_strlen(input) + 1);
// 	if (!segment)
// 		return (0);
// 	j = fill_segment(input, i, segment, quote);
// 	if (input[*i] != quote)
// 	{
// 		handle_unmatched_quote(shell, segment, *final);
// 		return (0);
// 	}
// 	(*i)++;
// 	segment[j] = '\0';
// 	if (ft_strncmp(segment, " ", 2) != 0)
// 	{
// 		tmp = ft_strjoin((*final) ? *final : "", segment);
// 		*final = tmp;
// 	}
// 	return (1);
// }

// int	collect_all_quotes(const char *input, int *i, char **final,
// t_shell *shell)
// {
// 	char	quote;
// 	char	nested_quote;

// 	quote = input[(*i)++];
// 	if (!collect_single_quote(input, i, final, quote, shell))
// 		return (0);
// 	while (input[*i] == '"' || input[*i] == '\'')
// 	{
// 		nested_quote = input[(*i)++];
// 		if (!collect_single_quote(input, i, final, nested_quote, shell))
// 			return (0);
// 	}
// 	return (1);
// }

// int	handle_quoted_token(const char *input, int *i, t_shell *shell)
// {
// 	t_token	**head;
// 	char	*final;
// 	int		type;
// 	int		error;

// 	head = &(shell->token);
// 	final = NULL;
// 	type = WORD;
// 	error = 0;
// 	if (input[*i] == '"' || input[*i] == '\'')
// 	{
// 		if (!collect_all_quotes(input, i, &final, shell))
// 			return (0);
// 	}
// 	detect_token_type(input, final, &type, &error);
// 	if (final)
// 		add_token(head, new_token(&(shell->ebag), final, type, error));
// 	if (error)
// 	{
// 		handle_invalid_option(shell, head);
// 		return (0);
// 	}
// 	return (1);
// }

// ================================================================

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
	// if(input[*i] == '|' && input[*i + 1] == '|')
	// {
	// 	(*i) += 2;
	// 	type = 0;
	// }
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
	// if(!type)
	// {
	// 	val = ft_strdup(syntax_error);
	// 	add_token(head, new_token(&(shell->ebag), val, 0, INPUT_INVA));
	// 	return;
	// }
	val = ft_substr(input, start, *i - start);
	type = get_type(val, type);
	if (type == REDIR_APPEND || type == REDIR_HEREDOC || type == REDIR_IN
		|| type == REDIR_OUT)
		shell->is_heredoc_delimiter = 1;
	add_token(head, new_token(&(shell->ebag), val, type, 0));
	return ;
}

// int	handle_option_token(t_shell *shell, const char *input, int *i,
// 		t_token **head)
// {
// 	t_lexer_state	*tmp;

// 	tmp->start = (*i)++;
// 	tmp->val = NULL;
// 	if (input[*i] == '\'' || input[*i] == '"')
// 	{
// 		tmp->quote = input[(*i)++];
// 		tmp->quoted_start = *i;
// 		while (input[*i] && input[*i] != tmp->quote && input[*i] != ' ')
// 			(*i)++;
// 		if (!input[*i])
// 		{
// 			tmp->val = ft_substr(input, tmp->start, *i);
// 			add_token(head, new_token(&(shell->ebag), tmp->val, ERROR,
// 					QUETS_INVA));
// 			return (0);
// 		}
// 		if (input[*i] == ' ')
// 		{
// 			tmp->val = ft_substr(input, tmp->start, *i);
// 			add_token(head, new_token(&(shell->ebag), tmp->val, ERROR,
// 					OPTION_INVA));
// 			return (0);
// 		}
// 		tmp->content = ft_substr(input, tmp->quoted_start, *i
// 				- tmp->quoted_start);
// 		tmp->val = ft_malloc(ft_strlen(tmp->content) + 2);
// 		if (tmp->val)
// 		{
// 			tmp->val[0] = '-';
// 			ft_strlcpy(&tmp->val[1], tmp->content, ft_strlen(tmp->content) + 1);
// 		}
// 		(*i)++;
// 	}
// 	else
// 	{
// 		tmp->opt_start = *i;
// 		if (input[*i + 1] == ' ')
// 		{
// 			add_token(head, new_token(&(shell->ebag), tmp->val, 0,
// 					OPTION_INVA));
// 			return (0);
// 		}
// 		while (input[*i] && !(input[*i] == ' ') && !is_special(input[*i]))
// 			(*i)++;
// 		tmp->val = ft_substr(input, tmp->start, *i - tmp->start);
// 	}
// 	if (tmp->val)
// 	{
// 		add_token(head, new_token(&(shell->ebag), tmp->val, OPTION, 0));
// 	}
// 	return (1);
// }

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

static int	handle_quoted_error(t_shell *shell, t_token **head,
		t_lexer_state *tmp, int *i)
{
	tmp->val = ft_substr(tmp->input, tmp->start, *i);
	if (!tmp->input[*i])
	{
		add_token(head, new_token(&(shell->ebag), tmp->val, ERROR, QUETS_INVA));
		return (0);
	}
	if (tmp->input[*i] == ' ')
	{
		add_token(head, new_token(&(shell->ebag), tmp->val, ERROR,
				OPTION_INVA));
		return (0);
	}
	return (1);
}

static void	build_quoted_value(t_lexer_state *tmp)
{
	tmp->val = ft_malloc(ft_strlen(tmp->content) + 2);
	if (tmp->val)
	{
		tmp->val[0] = '-';
		ft_strlcpy(&tmp->val[1], tmp->content, ft_strlen(tmp->content) + 1);
	}
}

static int	process_quoted(t_shell *shell, int *i, t_token **head,
		t_lexer_state *tmp)
{
	tmp->quote = tmp->input[(*i)++];
	tmp->quoted_start = *i;
	while (tmp->input[*i] && tmp->input[*i] != tmp->quote
		&& tmp->input[*i] != ' ')
		(*i)++;
	if (!handle_quoted_error(shell, head, tmp, i))
		return (0);
	tmp->content = ft_substr(tmp->input, tmp->quoted_start, *i
			- tmp->quoted_start);
	build_quoted_value(tmp);
	(*i)++;
	return (1);
}

static int	process_unquoted(t_shell *shell, int *i, t_token **head,
		t_lexer_state *tmp)
{
	tmp->opt_start = *i;
	if (tmp->input[*i + 1] == ' ')
	{
		add_token(head, new_token(&(shell->ebag), tmp->val, 0, OPTION_INVA));
		return (0);
	}
	while (tmp->input[*i] && !(tmp->input[*i] == ' ')
		&& !is_special(tmp->input[*i]))
		(*i)++;
	tmp->val = ft_substr(tmp->input, tmp->start, *i - tmp->start);
	return (1);
}

int	handle_option_token(t_shell *shell, const char *input, int *i,
		t_token **head)
{
	t_lexer_state	*tmp;

	tmp->input = input;
	tmp = ft_malloc(sizeof(t_lexer_state));
	tmp->start = (*i)++;
	tmp->val = NULL;
	if (is_quote(input[*i]))
	{
		if (!process_quoted(shell, i, head, tmp))
			return (0);
	}
	else
	{
		if (!process_unquoted(shell, i, head, tmp))
			return (0);
	}
	if (tmp->val)
		add_token(head, new_token(&(shell->ebag), tmp->val, OPTION, 0));
	return (1);
}

// =========================================================================

void	handle_word_token(t_shell *shell, const char *input, int *i,
		t_token **head)
{
	int		start;
	char	*buffer;
	int		j;

	start = *i;
	buffer = ft_malloc(ft_strlen(input) + 1);
	j = 0;
	if (!buffer)
		return ;
	while (input[*i] && input[*i] != ' ' && !is_special(input[*i])
		&& input[*i] != '"' && input[*i] != '\'')
		buffer[j++] = input[(*i)++];
	buffer[j] = '\0';
	if (j > 0)
		add_token(head, new_token(&(shell->ebag), buffer, WORD, 0));
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

int	pips_coount(char *input)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (input[i])
	{
		if (input[i] == '|')
			j++;
		i++;
	}
	return (j);
}
// =================================================================

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
		state->t_tmp = new_token(&(shell->ebag), state->current_word, WORD, 0);
		state->t_tmp->quot_type = state->current_quote_type;
		add_token(&state->head, state->t_tmp);
		state->current_word = NULL;
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

// =====================================

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

void	handle_dollar_sign(t_shell *shell, t_lexer_state *state)
{
	int	j;

	j = state->i;
	if (state->current_quote_type == S_QUOTE)
		handle_dollar_in_single_quotes(state);
	else
		handle_dollar_variable_expansion(shell, state, j);
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

// ======================================================

void	handle_invalid_quote(t_shell *shell, t_lexer_state *state)
{
	add_token(&state->head, new_token(&(shell->ebag), "Invalid quote", 0,
			QUETS_INVA));
	shell->exit_status = 258;
}

void	set_quote_type_explicitly(t_lexer_state *state, char quote)
{
	if (quote == '"')
		state->current_quote_type = D_QUOTE;
	else if (quote == '\'')
		state->current_quote_type = S_QUOTE;
}

static void	handle_empty_quotes(t_shell *shell, t_lexer_state *state)
{
	if (state->start == state->i && state->str[state->i])
	{
		if (!state->current_word)
		{
			if ((state->i == 1 && ft_strlen(state->str) == 2)
				|| !state->str[state->i + 1])
			{
				shell->not_found = 1;
				state->current_word = ft_strdup("");
			}
			else if (state->current_word)
			{
				shell->not_found = 1;
				state->current_word = ft_strdup("");
			}
		}
	}
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

// ======================================================

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

	init_lexer_vars(shell, &state);
	if (check_initial_dollar_error(shell, &state))
		return (state.head);
	while (state.str[state.i])
		process_token_loop(shell, &state);
	correct_lexer(shell, &state.head);
	// print_tokens(state.head);
	return (state.head);
}
