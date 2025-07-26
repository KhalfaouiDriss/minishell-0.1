/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_syntax_err.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 13:32:43 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/26 16:10:38 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	check_and_report_error(t_shell *shell)
{
	if (shell->pos == 1)
	{
		write(2, "syntax error\n", 14);
		shell->exit_status = 2;
	}
	return (1);
}

static int	check_invalid_operators(char *input, int i, t_shell *shell)
{
	if ((input[i] == '<' && input[i + 1] == '<' && input[i + 2] == '<')
		|| (input[i] == '>' && input[i + 1] == '>' && input[i + 2] == '>')
		|| (input[i] == '<' && input[i + 1] == '<' && input[i + 2] == '>')
		|| (input[i] == '>' && input[i + 1] == '>' && input[i + 2] == '<')
		|| (input[i] == '<' && input[i + 1] == '>') || (input[i] == '>'
			&& input[i + 1] == '<'))
		return (check_and_report_error(shell));
	return (0);
}

int	handle_operator(char *input, int *i, t_shell *shell)
{
	int	op_len;

	op_len = is_operator(input, *i);
	if (!op_len)
		return (0);
	if (check_invalid_operators(input, *i, shell))
		return (1);
	*i += op_len;
	*i = skip_spaces(input, *i);
	if (!input[*i] || input[0] == '|' || (is_operator(input, *i)
			&& !(input[*i] == '<' || input[*i] == '>')))
		return (check_and_report_error(shell));
	return (0);
}

int	check_embag(char *var_value)
{
	char	**value;
	int		i;

	if (!var_value)
		return (0);
	if (!spaces_is(var_value))
		return (0);
	value = ft_split(var_value, ' ');
	i = 0;
	while (value[i])
		i++;
	if (i > 1)
		return (0);
	return (1);
}
