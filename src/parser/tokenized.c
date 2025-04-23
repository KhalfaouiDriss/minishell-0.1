#include "../../include/minishell.h"

int h = 0;

// --- Quoted Tokens Helper ---
int handle_quoted_token(const char *input, int *i, t_token **head)
{
	char	quote;
	char	*final = ft_strdup("");
	int		type = WORD;
	int		error = 0;

	while (input[*i] == '"' || input[*i] == '\'')
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
			// Escape handling داخل double quotes
			if (quote == '"' && input[*i] == '\\' && input[*i + 1])
			{
				(*i)++;
				segment[j++] = input[(*i)++];
				continue;
			}

			segment[j++] = input[(*i)++];
		}

		h++;
		if (input[*i] != quote)
		{
			segment[j] = '\0';
			// char *err_str = ft_strjoin(final, segment);
			char *err_str = ft_strdup("Error: Unmatched quote");
			add_token(head, new_token(err_str, ERROR, QUETS_INVA));
			free(err_str);
			free(segment);
			free(final);
			return (0);
		}

		(*i)++; // Skip closing quote
		segment[j] = '\0';

		// 🔍 نحيد المسافة إذا كانت فقط ' '
		if (ft_strncmp(segment, " ", 2) == 0)
		{
			free(segment);
			continue;
		}

		char *tmp = ft_strjoin(final, segment);
		free(final);
		free(segment);
		final = tmp;
	}

	// 🧠 هل هو OPTION؟
	if (final[0] == '-' && ft_strlen(final) >= 2)
		type = OPTION;
	if(input[*i + 1] == ' ')
	{
		type = ERROR;
		error = OPTION_INVA;
	}
	if(final[0] == '$')
		handle_variable_token(input, i, head);
	else
		add_token(head, new_token(final, type, error));
	free(final);
	return (1);
}





// --- Redirections and Pipes Helper ---
void	handle_special_token(const char *input, int *i, t_token **head)
{
	int		start;
	int		type;
	char	*val;

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

// --- Normal Words ---
void	handle_word_token(const char *input, int *i, t_token **head)
{
	int		start = *i;
	char	*buffer = malloc(ft_strlen(input) + 1);
	int		j = 0;

	if (!buffer)
		return;

	while (input[*i] && input[*i] != ' ' && !is_special(input[*i])
		&& input[*i] != '"' && input[*i] != '\'')
	{
		if (input[*i] == '\\')
		{
			(*i)++;
			if (input[*i])
				buffer[j++] = input[(*i)++];
			continue;
		}
		buffer[j++] = input[(*i)++];
	}

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
	while(tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return i;
} 

void correct_lexer(t_token *head)
{
    t_token *tmp;
    t_token *to_delete;
    char *joined;
	int token_count;

	token_count = ft_nodelen(head);

	if((token_count == 1 && head->value[0] == '<') || (token_count == 1 && head->value[0] == '>'))
	{
		head->error = INPUT_INVA;
		head->value = ft_strdup("Invalid input");
		return;
	}
    tmp = head;
    while (tmp)
    {
        // Handle joined option like -a
        if (ft_strlen(tmp->value) == 1 && tmp->value[0] == '-')
        {
            joined = ft_strjoin(tmp->value, tmp->next->value);
            if (!joined)
                return;
            free(tmp->value);
            tmp->value = joined;
            tmp->type = OPTION;

            to_delete = tmp->next;
            tmp->next = tmp->next->next;
            if (to_delete->value)
                free(to_delete->value);
            free(to_delete);
            continue;
        }

        if (ft_strncmp(tmp->value, ">>", 2) == 0 && ft_strlen(tmp->value) == 2)
            tmp->type = REDIR_APPEND;
        else if (ft_strncmp(tmp->value, "<<", 2) == 0 && ft_strlen(tmp->value) == 2)
            tmp->type = REDIR_HEREDOC;
        else if (ft_strncmp(tmp->value, ">", 1) == 0 && ft_strlen(tmp->value) == 1)
            tmp->type = REDIR_OUT;
        else if (ft_strncmp(tmp->value, "<", 1) == 0 && ft_strlen(tmp->value) == 1)
            tmp->type = REDIR_IN;
        else if (ft_strncmp(tmp->value, "|", 1) == 0 && ft_strlen(tmp->value) == 1)
            tmp->type = PIPE;

        tmp = tmp->next;
    }
}


int is_quots_correct(const char *s)
{
	int i;
	int quots;
	int d_quots;

	i = 0;
	quots = 0;
	d_quots = 0;
	if(ft_strnstr(s, "echo", 4))
		return 1;
	while (s[i])
	{
		if(s[i] == '"')
			d_quots++;
		else if(s[i] == '\'')
			quots++;
		i++;
	}

	if(d_quots % 2 != 0 || quots % 2 != 0)
		return 0;
	return 1;
}

// --- Main Lexer Split Function ---
t_token	*lexer_split_to_tokens(const char *input)
{
	t_token	*head;
	int		i;

	head = NULL;
	i = 0;
	if(!is_quots_correct(input))
		add_token(&head, new_token("Quots not valid", 0, QUETS_INVA));
		if ((ft_strlen(input) <= 2 &&
		(ft_strncmp(input, ">", 2) == 0 || ft_strncmp(input, ">>", 2) == 0 ||
		 ft_strncmp(input, "<", 2) == 0 || ft_strncmp(input, "<<", 2) == 0)))
	   add_token(&head, new_token("Invalid Input", 0, INPUT_INVA));
   
	else
	{
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
			else if (input[i] == '$')
				handle_variable_token(input, &i, &head);
			else
				handle_word_token(input, &i, &head);
		}
	}
	correct_lexer(head);
	return (head);
}
