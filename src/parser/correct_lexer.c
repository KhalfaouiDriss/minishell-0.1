/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   correct_lexer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:49:06 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/12 12:08:19 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_operator(t_token *token)
{
	return (token->type == REDIR_OUT || token->type == REDIR_IN
		|| token->type == REDIR_APPEND || token->type == REDIR_HEREDOC
		|| token->type == PIPE);
}

static int	handle_pipe_at_start(t_shell *shell, t_token *token)
{
	if (token->type == PIPE)
	{
		token->value = ft_strdup(syntax_error);
		token->type = ERROR;
		token->error = INPUT_INVA;
		shell->exit_status = 2;
		return (1);
	}
	return (0);
}

int	handle_double_pipe(t_shell *shell, t_token *tmp, char **last_operator)
{
	if ((tmp->type == PIPE && ft_strncmp(*last_operator, "|", 1) == 0)
		|| (tmp->type != PIPE && strncmp(*last_operator, "|", 1) != 0))
	{
		tmp->value = ft_strdup(syntax_error);
		tmp->type = ERROR;
		tmp->error = INPUT_INVA;
		shell->exit_status = 2;
		return (1);
	}
	return (0);
}

static int	handle_operator_errors(t_shell *shell, t_token *tmp,
		char **last_operator)
{
	if (*last_operator)
	{
		if (handle_double_pipe(shell, tmp, last_operator))
			return (1);
	}
	*last_operator = ft_strdup(tmp->value);
	if (tmp->next == NULL)
	{
		tmp->value = ft_strdup(syntax_error);
		tmp->type = ERROR;
		tmp->error = INPUT_INVA;
		shell->exit_status = 2;
		return (1);
	}
	if (tmp->type != PIPE && is_operator(tmp->next))
	{
		tmp->next->value = ft_strdup(syntax_error);
		tmp->next->type = ERROR;
		tmp->next->error = INPUT_INVA;
		shell->exit_status = 2;
		return (1);
	}
	return (0);
}

void	correct_lexer(t_shell *shell, t_token **token)
{
	t_token	*tmp;
	char	*last_operator;

	last_operator = NULL;
	tmp = *token;
	if (!tmp || handle_pipe_at_start(shell, tmp))
		return ;
	while (tmp)
	{
		if (tmp->type == WORD)
			last_operator = NULL;
		else if (is_operator(tmp))
		{
			if (handle_operator_errors(shell, tmp, &last_operator))
				return ;
		}
		else
		{
			tmp->type = ERROR;
			tmp->error = INPUT_INVA;
			shell->exit_status = 2;
			return ;
		}
		tmp = tmp->next;
	}
}
