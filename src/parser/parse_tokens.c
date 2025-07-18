/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 15:51:13 by sel-bech          #+#    #+#             */
/*   Updated: 2025/07/18 18:03:10 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	*fake_glb(void)
{
	static int	n = -1;

	return (&n);
}

static int	parse_redirections(t_token **token, t_cmd *cmd, t_shell *shell)
{
	t_token	*next;

	next = (*token)->next;
	if (((*token)->type == REDIR_OUT || (*token)->type == REDIR_IN
			|| (*token)->type == REDIR_APPEND) && !next->ebag && !cmd->flag_amb)
		return (cmd->flag_amb = 1, *token = next, 0);
	if ((*token)->type == REDIR_IN)
	{
		if (*fake_glb() != -1)
			close(*fake_glb());
		cmd->infile = ft_strdup(next->value);
		redirect_input(cmd->infile, cmd);
		*fake_glb() = cmd->infile_fd;
		if (cmd->infile_fd == -1)
			return (shell->exit_status = 1, *token = next, 1);
	}
	else if ((*token)->type == REDIR_HEREDOC)
	{
		if (*fake_glb() != -1)
			close(*fake_glb());
		cmd->heredoc = ft_strdup(next->value);
		cmd->heredoc_fd = handle_heredoc(next->value, shell);
		*fake_glb() = cmd->heredoc_fd;
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
