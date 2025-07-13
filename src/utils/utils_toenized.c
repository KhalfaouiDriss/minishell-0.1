#include "../../include/minishell.h"

int check_embag(char *var_value)
{
	char **value;
	int i;

	if(!var_value)
		return 0;
	value = ft_split(var_value, ' ');
	i = 0;
	while (value[i])
		i++;
	if(i > 1)
	{
		return 0;
	}
	return 1;
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

void	set_quote_type_explicitly(t_lexer_state *state, char quote)
{
	if (quote == '"')
		state->current_quote_type = D_QUOTE;
	else if (quote == '\'')
		state->current_quote_type = S_QUOTE;
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

int is_embg_befor(t_shell *shell, int i)
{
	i--;
	if(is_quote(shell->input[i]))
		return 0;
	return 1; 
}

int last_is_redir(t_shell *shell, int i)
{
	while (i > 0 && shell->input[i] != '<' && shell->input[i] != '>')
		i--;

	if (i < 0)
		return 0;

	if ((shell->input[i] == '<' && shell->input[i + 1] != '<') ||
		(shell->input[i] == '>' && shell->input[i + 1] != '>'))
		return 1;

	return 0;
}


char *handle_variable_token(char *str, int *i, t_shell *shell, char quote)
{
	int		start, len;
	char	*var_name = NULL;
	char	*var_value = NULL;
	t_env	*env = shell->env;
	int status = 0;
	int is = 0;

	start = *i;
	if(*i > 1 && is_quote(str[*i - 1]) && is_quote(str[*i - 2]))
		is = 1;
    if(ft_isdigit(str[*i + 1]))
    {
        *i += 2;
        return NULL;
    }
    if (str[*i] == '$' && str[*i + 1] == '?')
	{
		*i += 2;
		status = shell->exit_status;
		return ft_itoa(status);
	}
    if (str[*i] == '$' && (str[*i + 1] == '\0' || str[*i + 1] == ' '))
	{
		(*i)++;
		return ft_strdup("$");
	}
	if (str[*i] == '$' && str[*i + 1] == '$')
	{
		*i += 2;
		return ft_strdup("$$");
	}

	if((str[*i + 1] == '\'' && str[*i + 2] == '\'') || (str[*i + 1] == '"' && str[*i + 2] == '"'))
    {
		(*i) += 3;
	    return (ft_strdup(""));
	}
	if((str[*i + 1] == '\'' && str[*i + 2] != '\'') || (str[*i + 1] == '"' && str[*i + 2] != '"'))
    {
		(*i)++;
	    return NULL;
	}
	if(!ft_isalnum(str[*i + 1]))
    {
		return (ft_strdup("$"));
	}
	(*i)++;
	start = *i;

	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	len = *i - start;
	var_name = ft_substr(str, start, len);


	if (quote == '\'')
		return ft_strjoin("$", var_name);
	else if(quote == '\"')
		return ft_strdup("");
	
	var_value = find_env_node(env, var_name);

	if(shell->is_heredoc_delimiter)
	{
		shell->is_heredoc_delimiter = 0;
		(*i)--;
		return ft_strjoin("$", var_name);
	}

	if (!var_value)
	{
		if (last_is_redir(shell, start))
		{
			if (is_embg_befor(shell, start - 1) && !is_quote(shell->input[*i]))
			{
				shell->ebag = 0;
			}
			(*i)--;
			return ft_strdup("");
		}
	}


	shell->ebag = check_embag(var_value);
	return var_value;
}





const char *token_type_to_str(int type)
{
    switch (type)
    {
    case WORD:
        return "WORD";
    case OPTION:
        return "OPTION";
    case PIPE:
        return "PIPE";
    case REDIR_IN:
        return "REDIR_IN";
    case REDIR_OUT:
        return "REDIR_OUT";
    case REDIR_APPEND:
        return "REDIR_APPEND";
    case REDIR_HEREDOC:
        return "REDIR_HEREDOC";
    case ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

void print_tokens(t_token *head)
{
    while (head)
    {
        printf("Token: %s | Type: %s | Error type: %d | quote type : %d | ebag : %d", head->value, token_type_to_str(head->type), head->error, head->quot_type, head->ebag);
        printf("\n");
        head = head->next;
    }
}