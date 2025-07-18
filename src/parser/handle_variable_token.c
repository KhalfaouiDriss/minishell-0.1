/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_variable_token.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 20:55:57 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/18 17:02:00 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*handle_special_cases(char *str, int *i, t_shell *shell, char quote)
{
	char	*ret;

	ret = handle_special_cases_utile(str, i, shell);
	if (ret)
		return (ret);
	if (str[*i] == '$' && !ft_isalnum(str[*i + 1]))
	{
		if (!is_quote(str[*i + 1]) && !quote)
			(*i)++;
		return (ft_strdup("$"));
	}
	if (ft_isdigit(str[*i + 1]))
	{
		*i += 2;
		return (NULL);
	}
	if (is_opened_quote(str, *i))
	{
		(*i)++;
		return (NULL);
	}
	if (!ft_isalnum(str[*i + 1]))
		return (ft_strdup("$"));
	return (NULL);
}

static char	*extract_var_name(char *str, int *i)
{
	int	start;

	start = ++(*i);
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	return (ft_substr(str, start, *i - start));
}

static char	*handle_heredoc_case(t_shell *shell, char *var_name, int *i,
		char quote)
{
	shell->is_heredoc_delimiter = 0;
	if (quote == '\"' || quote == '\'')
		shell->exp = 0;
	return (ft_strjoin("$", var_name));
}

static char	*handle_no_var_value(t_shell *shell, char *var_name, int start,
		int *i)
{
	if (last_is_redir(shell, start))
	{
		if (is_embg_befor(shell, start - 1) && !is_quote(shell->input[*i]))
			shell->ebag = 0;
		(*i)--;
		return (ft_strdup(""));
	}
	if (shell->input[*i] == ' ')
		(*i)++;
	return (NULL);
}

char	*handle_variable_token(char *str, int *i, t_shell *shell, char quote)
{
	char	*var_name;
	int		start;
	char	*res;
	char	*var_value;
	char	*special;

	var_name = NULL;
	special = handle_special_cases(str, i, shell, quote);
	if (special)
		return (special);
	start = *i;
	var_name = extract_var_name(str, i);
	if (quote == '\'')
		return (ft_strjoin("$", var_name));
	if (shell->is_heredoc_delimiter)
		return (handle_heredoc_case(shell, var_name, i, quote));
	var_value = find_env_node(shell->env, var_name);
	if (!var_value)
	{
		res = handle_no_var_value(shell, var_name, start, i);
		if (res)
			return (res);
	}
	shell->ebag = check_embag(var_value);
	return (var_value);
}
