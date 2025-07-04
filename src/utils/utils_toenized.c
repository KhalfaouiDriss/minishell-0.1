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
		return 0;
	return 1;
}



t_token *get_last_token(t_shell *shell)
{
	t_token *tmp;

	// tmp = shell->token;
	// while (tmp->next)
	// 	tmp = tmp->next;
	
	return NULL;
	// return tmp;
}

int is_redire(char *symbol)
{
	if(!ft_strncmp(symbol, ">>", 2) || !ft_strncmp(symbol, ">", 2) || !ft_strncmp(symbol, "<<", 2) ||
		!ft_strncmp(symbol, "<", 2))
	{
		return 1;
	}
	return 0;
}

char *handle_variable_token(char *str, int *i, t_shell *shell, char quote)
{
	int		start, len;
	char	*var_name = NULL;
	char	*var_value = NULL;
	t_env	*env = shell->env;

    if(ft_isdigit(str[*i + 1]))
    {
        *i += 2;
        return NULL;
    }
    if (str[*i] == '$' && str[*i + 1] == '?')
	{
		*i += 2;
		int status = shell->exit_status;
		// shell->exit_status = 0;
		return ft_itoa(status);
	}

    // if(!str[*i + 1] || str[*i + 1] == ' ')
    // {
    //     (*i)++;
    //     return ft_strdup("$");
    // }

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

    if(!ft_isalnum(str[*i + 1]))
        return (ft_strdup("$"));

	if (*i > 0 && (str[*i - 1] == '\\'))
	{
		(*i)++;
		start = *i;
		while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
			(*i)++;
		len = *i - start;
		var_name = ft_substr(str, start, len);
		char *result = ft_strjoin("$", var_name);
		free(var_name);
		return result;
	}

    // if(!ft_isalpha(str[*i+1]) && !ft_isdigit(str[*i+1]))
    // {
    //     while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
	// 		(*i)++;
	// 	len = *i - start;
	// 	var_name = ft_substr(str, start, len);
	// 	char *result = ft_strjoin("$", var_name);
	// 	free(var_name);
	// 	return result;
    // }

	(*i)++;
	start = *i;

	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;

	len = *i - start;
	var_name = ft_substr(str, start, len);

	if (quote == '\'')
	{
		char *result = ft_strjoin("$", var_name);
		free(var_name);
		return result;
	}
	while (env)
	{
		if (ft_strncmp(env->name, var_name, ft_strlen(env->name)) == 0
			&& ft_strlen(env->name) == len)
		{
			var_value = env->value;
			// var_value = ft_strdup(env->value);
			break;
		}
		env = env->next;
	}

	if (!var_value && ft_isalpha(var_name[0]))
	{
		char *unknown = NULL;
		if(shell->is_heredoc_delimiter)
		{
			shell->is_heredoc_delimiter = 0;
			return ft_strjoin("$", var_name);
		}
		return ft_strdup("");
			// free(var_name);
	}
    if (!var_value)
	{
		char *unknown = NULL;
		free(var_name);
		return "$";
	}
	shell->ebag = check_embag(var_value);
	// shell->ebag_final = check_embag(var_value);
	free(var_name);
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