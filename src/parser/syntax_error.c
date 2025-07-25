/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 17:18:37 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/25 18:52:54 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_operator(char *str, int i)
{
	if (str[i] == '<')
	{
		if (str[i + 1] == '<')
			return (2);
		else
			return (1);
	}
	else if (str[i] == '>')
	{
		if (str[i + 1] == '>')
			return (2);
		else
			return (1);
	}
	else if (str[i] == '|')
		return (1);
	return (0);
}

static int	handle_unmatched_quote(char quote, char *input, int *i,
		t_shell *shell)
{
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (!input[*i])
	{
		if (shell->pos == 1)
		{
			write(2, "syntax error: unmatched quote\n", 31);
			shell->exit_status = 2;
		}
		return (1);
	}
	(*i)++;
	return (0);
}

static int	skip_word(char *input, int *i, t_shell *shell)
{
	char	quote;

	while (input[*i] && !isspace(input[*i]) && !is_operator(input, *i))
	{
		if (input[*i] == '"' || input[*i] == '\'')
		{
			quote = input[(*i)++];
			if (handle_unmatched_quote(quote, input, i, shell))
				return (1);
		}
		else
			(*i)++;
	}
	return (0);
}

int	check_syntax_errors(t_shell *shell, char *input, int pos)
{
	int		i;
	char	quote;

	i = skip_spaces(shell->input, 0);
	shell->pos = pos;
	if (!input[i])
		return (0);
	while (input[i])
	{
		if (input[i] == '"' || input[i] == '\'')
		{
			quote = input[i++];
			if (handle_unmatched_quote(quote, input, &i, shell))
				return (1);
			continue ;
		}
		i = skip_spaces(input, i);
		if (handle_operator(input, &i, shell))
			return (1);
		if (skip_word(input, &i, shell))
			return (1);
	}
	shell->pos = 0;
	return (0);
}
