/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_variable.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 20:57:20 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/23 14:48:26 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_embg_befor(t_shell *shell, int i)
{
	if (i <= 0)
		return (1);
	i--;
	if (is_quote(shell->input[i]))
		return (0);
	return (1);
}

int	last_is_redir(t_shell *shell, int i)
{
	while (i > 0 && shell->input[i] != '<' && shell->input[i] != '>')
		i--;
	if (i < 0)
		return (0);
	if ((shell->input[i] == '<' && shell->input[i + 1] != '<')
		|| (shell->input[i] == '>' && shell->input[i + 1] != '>'))
		return (1);
	return (0);
}

static int	is_double_empty_quote(char *str, int i)
{
	return ((str[i + 1] == '\'' && str[i + 2] == '\'') || (str[i + 1] == '"'
			&& str[i + 2] == '"'));
}

int	is_opened_quote(char *str, int i)
{
	return ((str[i + 1] == '\'' && str[i + 2] != '\'') || (str[i + 1] == '"'
			&& str[i + 2] != '"'));
}

char	*handle_special_cases_utile(char *str, int *i, t_shell *shell)
{
	if (str[*i] == '$' && str[*i + 1] == '?')
	{
		*i += 2;
		return (ft_itoa(shell->exit_status));
	}
	if (str[*i] == '$' && (str[*i + 1] == '\0' || str[*i + 1] == ' '))
	{
		(*i)++;
		return (ft_strdup("$"));
	}
	if (str[*i] == '$' && str[*i + 1] == '$')
	{
		*i += 2;
		return (ft_strdup("$$"));
	}
	if (is_double_empty_quote(str, *i))
	{
		(*i) += 3;
		return (ft_strdup(""));
	}
	return (NULL);
}
