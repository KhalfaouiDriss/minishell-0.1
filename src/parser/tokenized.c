#include "../../include/minishell.h"

int h = 0;

char *strjoin_free(char *s1, char *s2)
{
	char *new_str;

	if (!s1 && !s2)
		return NULL;
	if (!s1)
		return strdup(s2);
	if (!s2)
		return strdup(s1);
	new_str = ft_strjoin(s1, s2);
	free(s1);
	return new_str;
}

int handle_quoted_token(const char *input, int *i, t_shell *shell)
{
	char quote;
	char *final = NULL;
	int type = WORD;
	int error = 0;
	t_token **head = &(shell->token);

	if (input[*i] == '"' || input[*i] == '\'')
	{
		quote = input[(*i)++];
		int j = 0;
		char *segment = malloc(ft_strlen(input) + 1);
		if (!segment)
			return (0);

		while (input[*i])
		{
			if (input[*i] == quote)
				break;

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

		if (input[*i] != quote)
		{
			segment[j] = '\0';
			char *err_str = ft_strdup("Error: Unmatched quote");
			add_token(head, new_token(err_str, ERROR, QUETS_INVA));
			free(err_str);
			free(segment);
			free(final);
			shell->exit_status = 258;
			return (0);
		}

		(*i)++;
		segment[j] = '\0';

		if (ft_strncmp(segment, " ", 2) == 0)
		{
			free(segment);
		}
		else
		{
			char *tmp;

			if (final)
				tmp = ft_strjoin(final, segment);
			else
				tmp = ft_strjoin("", segment);

			free(final);
			free(segment);
			final = tmp;
		}

		while (input[*i] == '"' || input[*i] == '\'')
		{
			char nested_quote = input[(*i)++];
			j = 0;
			segment = malloc(ft_strlen(input) + 1);
			if (!segment)
			{
				free(final);
				return (0);
			}
			while (input[*i])
			{
				if (input[*i] == nested_quote)
					break;
				if (nested_quote == '"' && input[*i] == '\\' && input[*i + 1])
				{
					(*i)++;
					segment[j++] = input[(*i)++];
				}
				else
				{
					segment[j++] = input[(*i)++];
				}
			}
			if (input[*i] != nested_quote)
			{
				segment[j] = '\0';
				char *err_str = ft_strdup("Error: Unmatched quote");
				add_token(head, new_token(err_str, ERROR, QUETS_INVA));
				free(err_str);
				free(segment);
				free(final);
				shell->exit_status = 258;
				return (0);
			}
			(*i)++;
			segment[j] = '\0';

			char *tmp = ft_strjoin(final ? final : "", segment);
			free(final);
			free(segment);
			final = tmp;
		}
	}

	if (final && final[0] == '-' && ft_strlen(final) >= 2)
		type = OPTION;

	if (final && input[*i + 1] == ' ')
	{
		type = ERROR;
		error = OPTION_INVA;
	}

	if (final)
		add_token(head, new_token(final, type, error));

	free(final);
	if (error)
	{
		char *err_str = ft_strdup("Error: Invalid option");
		add_token(head, new_token(err_str, ERROR, error));
		free(err_str);
		shell->exit_status = 258;
		return (0);
	}
	return (1);
}

void handle_special_token(const char *input, int *i, t_token **head)
{
	int start;
	int type;
	char *val;

	start = *i;
	if ((input[*i] == '<' || input[*i] == '>') && input[*i + 1] == input[*i])
		*i += 2;
	else if (input[*i] == '|' || input[*i] == '<' || input[*i] == '>')
		(*i)++;
	else
	{
		(*i)++;
		val = ft_substr(input, start, *i - start);
		add_token(head, new_token(val, ERROR, 0));
		free(val);
		return;
	}
	val = ft_substr(input, start, *i - start);
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
	add_token(head, new_token(val, type, 0));
	free(val);
}

int handle_option_token(const char *input, int *i, t_token **head)
{
	int start;
	int opt_start;
	int quoted_start;
	char quote;
	char *val;
	char *content;

	start = (*i)++;
	val = NULL;
	if (input[*i] == '\'' || input[*i] == '"')
	{
		quote = input[(*i)++];
		quoted_start = *i;
		while (input[*i] && input[*i] != quote && input[*i] != ' ')
			(*i)++;
		if (!input[*i])
		{
			val = ft_substr(input, start, *i);
			add_token(head, new_token(val, ERROR, QUETS_INVA));
			free(val);
			return (0);
		}
		if (input[*i] == ' ')
		{
			val = ft_substr(input, start, *i);
			add_token(head, new_token(val, ERROR, OPTION_INVA));
			free(val);
			return (0);
		}
		content = ft_substr(input, quoted_start, *i - quoted_start);
		val = malloc(ft_strlen(content) + 2);
		if (val)
		{
			val[0] = '-';
			ft_strlcpy(&val[1], content, ft_strlen(content) + 1);
		}
		free(content);
		(*i)++;
	}
	else
	{
		opt_start = *i;
		if (input[*i + 1] == ' ')
		{
			add_token(head, new_token(val, 0, OPTION_INVA));
			return 0;
		}
		while (input[*i] && !(input[*i] == ' ') && !is_special(input[*i]))
			(*i)++;
		val = ft_substr(input, start, *i - start);
	}
	if (val)
	{
		add_token(head, new_token(val, OPTION, 0));
		free(val);
	}
	return (1);
}

void handle_word_token(const char *input, int *i, t_token **head)
{
	int start = *i;
	char *buffer = malloc(ft_strlen(input) + 1);
	int j = 0;

	if (!buffer)
		return;

	while (input[*i] && input[*i] != ' ' && !is_special(input[*i]) && input[*i] != '"' && input[*i] != '\'')
		buffer[j++] = input[(*i)++];

	buffer[j] = '\0';

	if (j > 0)
		add_token(head, new_token(buffer, WORD, 0));
	free(buffer);
}

int ft_nodelen(t_token *head)
{
	int i;
	t_token *tmp;

	tmp = head;
	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return i;
}
void correct_lexer(t_shell *shell, t_token **token)
{
	t_token *tmp;
	char *tok = NULL;
	t_token *head = *token;

	if (!head)
	{
		return;
	}
	tmp = *token;

	if(ft_strncmp(tmp->value, "|", 1) == 0)
	{
		free(tmp->value);
		tmp->value = ft_strdup("syntax error");
		tmp->type = ERROR;
		tmp->error = INPUT_INVA;
		return;
	}

	while (tmp)
	{
		if (ft_strlen(tmp->value) == 1 && tmp->value[0] == '-')
		{
			tmp->type = ERROR;
			tmp->error = OPTION_INVA;
			free(tmp->value);
			tmp->value = ft_strdup("syntax error");
			shell->exit_status = 258;
			free(tok);
			return;
		}
		if (ft_strncmp(tmp->value, ">>", 2) == 0 && ft_strlen(tmp->value) == 2)
		{
			if (tok && ft_strncmp(tmp->value, tok, 2) == 0)
			{
				free(tmp->value);
				tmp->value = ft_strdup("syntax error");
				tmp->type = ERROR;
				tmp->error = INPUT_INVA;
				free(tok);
				return;
			}
			tmp->type = REDIR_APPEND;
		}
		else if (ft_strncmp(tmp->value, "<<", 2) == 0 && ft_strlen(tmp->value) == 2)
		{
			if (tok && ft_strncmp(tmp->value, tok, 2) == 0)
			{
				free(tmp->value);
				tmp->value = ft_strdup("syntax error");
				tmp->type = ERROR;
				tmp->error = INPUT_INVA;
				free(tok);
				return;
			}
			tmp->type = REDIR_HEREDOC;
		}
		else if (ft_strncmp(tmp->value, ">", 1) == 0 && ft_strlen(tmp->value) == 1)
		{
			if (tok && ft_strncmp(tmp->value, tok, 1) == 0)
			{
				free(tmp->value);
				tmp->value = ft_strdup("syntax error");
				tmp->type = ERROR;
				tmp->error = INPUT_INVA;
				free(tok);
				return;
			}
			tmp->type = REDIR_OUT;
		}
		else if (ft_strncmp(tmp->value, "<", 1) == 0 && ft_strlen(tmp->value) == 1)
		{
			if (tok && ft_strncmp(tmp->value, tok, 1) == 0)
			{
				free(tmp->value);
				tmp->value = ft_strdup("syntax error");
				tmp->type = ERROR;
				tmp->error = INPUT_INVA;
				free(tok);
				return;
			}
			tmp->type = REDIR_IN;
		}
		else if (ft_strncmp(tmp->value, "|", 1) == 0 && ft_strlen(tmp->value) == 1)
		{
			if (tok && ft_strncmp(tmp->value, tok, 1) == 0)
			{
				free(tmp->value);
				tmp->value = ft_strdup("syntax error");
				tmp->type = ERROR;
				tmp->error = INPUT_INVA;
				free(tok);
				return;
			}
			tmp->type = PIPE;
		}
		else
		{
			tmp->type = WORD;
			free(tok);
			tok = NULL;
		}
		if (!tmp->next)
		{
			if (!ft_strncmp(tmp->value, "|", 1) || !ft_strncmp(tmp->value, ">", 1) ||
				!ft_strncmp(tmp->value, "<<", 2) || !ft_strncmp(tmp->value, "<", 1) ||
				!ft_strncmp(tmp->value, ">>", 2))
			{
				free(tmp->value);
				tmp->value = ft_strdup("syntax error");
				tmp->type = 0;
				tmp->error = INPUT_INVA;
			}
		}
		if (tmp->type != WORD)
		{
			free(tok);
			tok = ft_strdup(tmp->value);
		}

		tmp = tmp->next;
	}
	free(tok);
}

int is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

char *expand_variables_in_string(char *str, t_shell *shell, char qt)
{
	int i = 0;
	char *result = NULL;
	char *tmp;

	while (str[i])
	{
		if (str[i] == '$')
		{
			tmp = handle_variable_token(str, &i, shell, qt);
			result = strjoin_free(result, tmp);
			free(tmp);
		}
		else
		{
			int start = i;
			while (str[i] && str[i] != '$')
				i++;
			tmp = ft_substr(str, start, i - start);
			result = strjoin_free(result, tmp);
			free(tmp);
		}
	}
	return result;
}

int isAllSpace(char *str)
{
	int i = 0;
	printf("check 2");
	while (str[i])
	{
		if (str[i] && str[i] != ' ')
			return 0;
		i++;
	}
	return 1;
}

t_token *lexer_split_to_tokens(t_shell *shell)
{
	int i = 0;
	t_token *head = NULL;
	t_token *t_tmp = NULL;
	char quote;
	char *isQuote = NULL;
	int current_quote_type = 0;
	int token_type = 0;
	char *str = shell->input;
	char *tmp;
	char *tmp_2;
	char *tmp_3;
	int start = 0;
	char *current_word = NULL;
	int j;

	if (str[0] == '$' && (!str[1] || str[1] == ' '))
	{
		add_token(&head, new_token("minishell: '$' command not found", 0, NOT_FOUND));
		shell->exit_status = 127;
		return head;
	}

	while (str[i])
	{
		while (str[i] && is_space(str[i]))
			i++;
		if (str[i] == '\0')
			break;
		start = i;
		while (str[i] && !is_space(str[i]))
		{
			if (str[i] == '\'' || str[i] == '"')
			{
				quote = str[i];
				if (quote == '"')
					current_quote_type = D_QUOTE;
				else
					current_quote_type = S_QUOTE;
				i++;
				start = i;

				while (str[i] && str[i] != quote)
					i++;
				if (str[i] == '\0')
				{
					add_token(&head, new_token("Invalid quote", 0, QUETS_INVA));
					shell->exit_status = 258;
					free(current_word);
					return head;
				}
				tmp = ft_substr(str, start, i - start);
				if (quote == '\'' && tmp[0] != '$')
					current_word = strjoin_free(current_word, tmp);
				else
				{
					char *expanded = expand_variables_in_string(tmp, shell, quote);
					current_word = strjoin_free(current_word, expanded);
					free(expanded);
				}
				free(tmp);
				i++;
				current_quote_type = 0;
			}

			else if (is_special(str[i]))
			{
				if (current_word)
				{
					t_tmp = new_token(current_word, WORD, 0);
					t_tmp->quot_type = current_quote_type;
					add_token(&head, t_tmp);
					free(current_word);
					current_word = NULL;
				}
				if (str[i] == '|' && str[i + 1] == '|')
				{
					correct_lexer(shell, &head);
					return head;
				}
				handle_special_token(str, &i, &head);
			}
			else if (str[i] == '$')
			{
				if (current_quote_type == S_QUOTE)
				{
					start = i;
					i++;
					while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
						i++;
					tmp = ft_substr(str, start, i - start);
					current_word = strjoin_free(current_word, tmp);
					free(tmp);
				}
				else
				{
					j = i;
					char *value = handle_variable_token(str, &i, shell, 0);
					if(value)
					{
						tmp_2 = ft_substr(str, 0, j);
						tmp_3 = ft_substr(str, i, ft_strlen(str) - i);
						free(shell->input);
						shell->input = ft_strjoin(tmp_2, value);
						free(tmp_2);
						tmp_2 = shell->input;
						shell->input = ft_strjoin(shell->input, tmp_3);
						i = j;
						// current_word = strjoin_free(current_word, value);
						free(tmp_2);
						free(value);
						str = shell->input;
						break;
					}
				}
			}
			else
			{
				start = i;
				while (str[i] && !is_space(str[i]) && str[i] != '\'' && str[i] != '"' && !is_special(str[i]) && str[i] != '$')
					i++;
				tmp = ft_substr(str, start, i - start);
				current_word = strjoin_free(current_word, tmp);
				free(tmp);
			}
		}
		if (current_word)
		{
			if (current_word[0] == '$')
				token_type = VARIABLE;
			else if (current_word[0] == '-' && ft_strlen(current_word) >= 2)
				token_type = OPTION;
			else
				token_type = WORD;

			t_tmp = new_token(current_word, token_type, 0);
			t_tmp->quot_type = current_quote_type;
			add_token(&head, t_tmp);
			free(current_word);
			current_word = NULL;
		}
	}
	correct_lexer(shell, &head);
	print_tokens(head);
	return head;
}
