/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_toenized.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 21:15:41 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/23 14:47:47 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	spaces_is(char *value)
{
	int	i;

	i = 0;
	while (value[i])
	{
		if (value[i] != ' ')
			return (1);
		i++;
	}
	return (0);
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
	state->quote = quote;
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
