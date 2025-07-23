/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parce_red.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:14:12 by sel-bech          #+#    #+#             */
/*   Updated: 2025/07/23 14:42:08 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	red_out(t_shell *shell, t_cmd *cmd, t_token *token)
{
	(void)shell;
	cmd->outfile = ft_strdup(token->next->value);
	if (token->type == REDIR_OUT)
		cmd->append = 0;
	else if (token->type == REDIR_APPEND)
		cmd->append = 1;
	redirect_output(cmd, cmd->append);
}

int	count_args(t_token *token)
{
	int	i;

	i = 0;
	while (token && token->type != PIPE)
	{
		if (token->type == OPTION || token->type == WORD)
			i++;
		if (token->type >= REDIR_IN && token->type <= REDIR_HEREDOC)
			token = token->next;
		token = token->next;
	}
	return (i);
}

void	init_str(t_cmd *cmd)
{
	cmd->args = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->next = NULL;
	cmd->heredoc = 0;
	cmd->infile = 0;
	cmd->infile_fd = 0;
	cmd->append = 0;
	cmd->outfile_fd = 0;
	cmd->c_flag = 0;
	cmd->flag_amb = 0;
	cmd->heredoc_fd = 0;
	cmd->fod_flag = 0;
}

int	her_red(t_cmd *cmd, t_token *token, t_shell *shell)
{
	cmd->heredoc = ft_strdup(token->next->value);
	cmd->heredoc_fd = handle_heredoc(token->next->value, shell);
	*fake_gl() = cmd->heredoc_fd;
	if (cmd->heredoc_fd == -1)
		return (1);
	return (0);
}

void	in_red(t_cmd *cmd, t_token *token)
{
	cmd->infile = ft_strdup(token->next->value);
	redirect_input(cmd->infile, cmd);
}
