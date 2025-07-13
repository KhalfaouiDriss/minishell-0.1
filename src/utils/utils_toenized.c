/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_toenized.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 21:15:41 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/13 21:15:42 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_embag(char *var_value)
{
	char	**value;
	int		i;

	if (!var_value)
		return (0);
	value = ft_split(var_value, ' ');
	i = 0;
	while (value[i])
		i++;
	if (i > 1)
	{
		return (0);
	}
	return (1);
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

int	skip_spaces(char *str, int i)
{
	while (str[i] && isspace(str[i]))
		i++;
	return (i);
}
