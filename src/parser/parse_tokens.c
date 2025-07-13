/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 15:51:13 by sel-bech          #+#    #+#             */
/*   Updated: 2025/07/13 10:11:13 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	parse_redirections(t_token **token, t_cmd *cmd, t_shell *shell)
{
	t_token	*next;

	next = (*token)->next;
	if (((*token)->type == REDIR_OUT || (*token)->type == REDIR_IN
			|| (*token)->type == REDIR_APPEND) && !next->ebag && !cmd->flag_amb)
		return (cmd->flag_amb = 1, *token = next, 0);
	if ((*token)->type == REDIR_IN)
	{
		cmd->infile = ft_strdup(next->value);
		redirect_input(cmd->infile, cmd);
		if(cmd->infile_fd == -1)
			return (shell->exit_status = 1, *token = next, 1);
	}
	else if ((*token)->type == REDIR_HEREDOC)
	{
		cmd->heredoc = ft_strdup(next->value);
		cmd->heredoc_fd = handle_heredoc(next->value, shell);
		if (cmd->heredoc_fd == -1)
			return (-1);
	}
	else if ((*token)->type == REDIR_OUT || (*token)->type == REDIR_APPEND)
		red_out(shell, cmd, (*token));
	*token = next;
	return (1);
}

static t_cmd	*parse_command(t_token **token, t_shell *shell)
{
	t_cmd	*cmd;
	int		arg_count;
	int		i;

	cmd = ft_malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	init_str(cmd);
	arg_count = count_args(*token);
	cmd->args = ft_malloc((arg_count + 1) * sizeof(char *));
	i = 0;
	while (*token && (*token)->type != PIPE)
	{
		shell->exit_status = 0;
		if ((*token)->type == WORD || (*token)->type == OPTION)
			cmd->args[i++] = ft_strdup((*token)->value);
		else
		{
			if (parse_redirections(token, cmd, shell) == -1)
				return (NULL);
		}
		if (*token)
			*token = (*token)->next;
	}
	return (cmd->args[i] = NULL, cmd);
}

int	check_syn(t_token *token)
{
	while (token)
	{
		if (!token->type)
		{
			ft_putstr_fd(token->value, 2);
			printf("\n");
			return (1);
		}
		token = token->next;
	}
	return (0);
}

t_cmd	*parse_tokens(t_shell *shell)
{
	t_cmd	*head;
	t_cmd	*last;
	t_cmd	*cmd;
	t_token	*token;

	head = NULL;
	last = NULL;
	token = shell->token;
	if (check_syn(token))
		return (NULL);
	token = shell->token;
	while (token)
	{
		cmd = parse_command(&token, shell);
		if (!cmd)
			return (NULL);
		if (!head)
			head = cmd;
		else
			last->next = cmd;
		last = cmd;
		if (token && token->type == PIPE)
			token = token->next;
	}
	return (head);
}
