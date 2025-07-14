/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khalfaoui47 <khalfaoui47@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 17:18:37 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/14 16:49:45 by khalfaoui47      ###   ########.fr       */
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
	{
		return (1);
	}
	return (0);
}

static int	handle_unmatched_quote(char quote, char *input, int *i,
		t_shell *shell)
{
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (!input[*i])
	{
		if(shell->pos = 1)
		{
			printf("syntax error: unmatched quote\n");
			shell->exit_status = 130;
		}
		return (1);
	}
	(*i)++;
	return (0);
}

static int	handle_operator(char *input, int *i, t_shell *shell)
{
	int	op_len;

	op_len = is_operator(input, *i);
	if (op_len)
	{
		*i += op_len;
		*i = skip_spaces(input, *i);
		if (!input[*i] || input[0] == '|' || (is_operator(input, *i)
				&& !((input[*i] == '<') || (input[*i] == '>'))))
		{
			if(shell->pos = 1)
			{
				printf("syntax error\n");
				shell->exit_status = 2;
			}
			return (1);
		}
	}
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
