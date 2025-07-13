/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 17:18:37 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/13 17:18:38 by dkhalfao         ###   ########.fr       */
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

int	skip_spaces(char *str, int i)
{
	while (str[i] && isspace(str[i]))
		i++;
	return (i);
}

int	check_syntax_errors(t_shell *shell)
{
	int		i;
	int		op_len;
	char	*input;
	char	quote;

	i = 0;
	input = shell->input;
	i = skip_spaces(input, 0);
	if (!input[i])
		return (0);
	while (input[i])
	{
		if (input[i] == '"' || input[i] == '\'')
		{
			quote = input[i++];
			while (input[i] && input[i] != quote)
				i++;
			if (!input[i])
			{
				printf("syntax error: unmatched quote\n");
				shell->exit_status = 130;
				return (1);
			}
			i++;
			continue ;
		}
		i = skip_spaces(input, i);
		op_len = is_operator(input, i);
		if (op_len)
		{
			i += op_len;
			i = skip_spaces(input, i);
			if (!input[i] || input[0] == '|' || (is_operator(input, i)
					&& !((input[i] == '<') || (input[i] == '>'))))
			{
				printf("syntax error\n");
				shell->exit_status = 2;
				return (1);
			}
		}
		else
		{
			while (input[i] && !isspace(input[i]) && !is_operator(input, i))
			{
				if (input[i] == '"' || input[i] == '\'')
				{
					quote = input[i++];
					while (input[i] && input[i] != quote)
						i++;
					if (!input[i])
					{
						printf("syntax error: unmatched quote\n");
						shell->exit_status = 130;
						return (1);
					}
				}
				i++;
			}
		}
	}
	return (0);
}
