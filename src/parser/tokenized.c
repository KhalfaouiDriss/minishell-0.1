#include "../../include/minishell.h"

// --- Quoted Tokens Helper ---
int	handle_quoted_token(const char *input, int *i, t_token **head)
{
	char	quote;
	int		start;
	int		end_quote;
	char	*core;

	quote = input[(*i)++];
	start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (!input[*i])
	{
		core = ft_substr(input, start - 1, *i);
		add_token(head, new_token(core, ERROR, QUETS_INVA));
		free(core);
		return (0);
	}
	end_quote = *i;
	(*i)++; // skip the closing quote

	core = ft_substr(input, start, end_quote - start);

	// ✅ add token even if it's empty "" → used in echo
	add_token(head, new_token(core, WORD, 0));
	free(core);

	// ⚠️ Do NOT consume next word (like "") here
	return (1);
}


// --- Redirections and Pipes Helper ---
void	handle_special_token(const char *input, int *i, t_token **head)
{
	int		start;
	int		type;
	char	*val;

	start = *i;

	// Detect double redirections (<< or >>)
	if ((input[*i] == '<' || input[*i] == '>') && input[*i + 1] == input[*i])
		*i += 2;
	else if (input[*i] == '|' || input[*i] == '<' || input[*i] == '>')
		(*i)++;
	else
	{
		// Unknown special character
		(*i)++;
		val = ft_substr(input, start, *i - start);
		add_token(head, new_token(val, ERROR, 0));
		free(val);
		return;
	}

	val = ft_substr(input, start, *i - start);

	if (ft_strncmp(val, "|", 1) == 0)
		type = PIPE;
	else if (ft_strncmp(val, "<", 1) == 0)
		type = REDIR_IN;
	else if (ft_strncmp(val, ">", 1) == 0)
		type = REDIR_OUT;
	else if (ft_strncmp(val, ">>", 2) == 0)
		type = REDIR_APPEND;
	else if (ft_strncmp(val, "<<", 2) == 0)
		type = REDIR_HEREDOC;
	else
		type = ERROR;

	add_token(head, new_token(val, type, 0));
	free(val);
}


// --- Option Handling Helper ---
int	handle_option_token(const char *input, int *i, t_token **head)
{
	int		start;
	int		opt_start;
	int		quoted_start;
	char	quote;
	char	*val;
	char	*content;

	start = (*i)++;
	val = NULL;
	if (input[*i] == '\'' || input[*i] == '"')
	{
		quote = input[(*i)++];
		quoted_start = *i;
		while (input[*i] && input[*i] != quote)
			(*i)++;
		if (!input[*i])
		{
			val = ft_substr(input, start, *i);
			add_token(head, new_token(val, ERROR, 0));
			free(val);
			return (0);
		}
		content = ft_substr(input, quoted_start, *i);
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
		while (input[*i] && !(input[*i] == ' ') && !is_special(input[*i]))
			(*i)++;
		val = ft_substr(input, start, *i);
	}
	if (val)
	{
		add_token(head, new_token(val, OPTION, 0));
		free(val);
	}
	return (1);
}

// --- Normal Words ---
void	handle_word_token(const char *input, int *i, t_token **head)
{
	int		start;
	char	*val;
	int type;

	start = *i;
	type = WORD;
	while (input[*i] && input[*i] != ' ' && !is_special(input[*i])
		&& input[*i] != '"' && input[*i] != '\'') // نوقف عند quotes
		(*i)++;
	if (*i > start)
	{
		val = ft_substr(input, start, *i - start);
		add_token(head, new_token(val, WORD, 0));
		free(val);
	}
}


// --- Main Lexer Split Function ---
t_token	*lexer_split_to_tokens(const char *input)
{
	t_token	*head;
	int		i;

	head = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		if (!input[i])
			break ;
		if (input[i] == '\'' || input[i] == '"')
		{
			if (!handle_quoted_token(input, &i, &head))
				break ;
		}
		else if (is_special(input[i]))
			handle_special_token(input, &i, &head);
		else if (input[i] == '-' && input[i + 1])
		{
			if (!handle_option_token(input, &i, &head))
				break ;
		}
		else
			handle_word_token(input, &i, &head);
	}
	return (head);
}
