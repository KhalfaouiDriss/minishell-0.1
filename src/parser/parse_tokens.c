#include "../../include/minishell.h"

int	count_args(t_token *token)
{
	int	i = 0;

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

char	*safe_strdup(char *s)
{
	if (!s)
		return (NULL);
	return (ft_strdup(s));
}

void	init_str(t_cmd *cmd)
{
	cmd->args = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->next = NULL;
	cmd->heredoc = 0;
	cmd->append = 0;
	cmd->outfile_fd = 0;
	cmd->c_flag = 0;
	cmd->flag_amb = 0;
	cmd->heredoc_fd = -1;
	cmd->fod_flag = 0;
}

static void	handle_ambiguous(t_token **token, t_cmd *cmd, t_shell *shell)
{
	shell->ebag_final = 0;
	cmd->flag_amb = 1;
	shell->exit_status = 1;
	write(2,"ambiguous redirect\n", 20);
	*token = (*token)->next;
}


static int	parse_redirections(t_token **token, t_cmd *cmd, t_shell *shell)
{
	t_token *next = (*token)->next;
	if (!next)
		return 0;

	if (((*token)->type == REDIR_OUT || (*token)->type == REDIR_IN || (*token)->type == REDIR_APPEND) &&
		ft_strncmp(next->value, "''", 3) == 0)
	{
		write(2, "minishell: : No such file or directory\n", 39);
		shell->exit_status = 1;
		cmd->fod_flag = 1;
		cmd->args[0] = NULL;
		return 0;
	}

	if (((*token)->type == REDIR_OUT || (*token)->type == REDIR_IN || (*token)->type == REDIR_APPEND)
		&& !next->ebag)
	{
		handle_ambiguous(token, cmd, shell);
		cmd->flag_amb = 1;
		return 0;
	}

	if ((*token)->type == REDIR_IN){
		cmd->infile = safe_strdup(next->value);
		return 1;
	}
	else if ((*token)->type == REDIR_HEREDOC)
	{
		cmd->heredoc = safe_strdup(next->value);
		cmd->heredoc_fd = handle_heredoc(next->value, shell);
		if (cmd->heredoc_fd == -1)
			return -1;
	}
	else if ((*token)->type == REDIR_OUT)
	{
		cmd->outfile = safe_strdup(next->value);
		redirect_output(shell, cmd, 0);
		cmd->append = 0;
	}
	else if ((*token)->type == REDIR_APPEND)
	{
		cmd->outfile = safe_strdup(next->value);
		redirect_output(shell, cmd, 1);
		cmd->append = 1;
	}
	*token = next;
	return 1;
}

static t_cmd	*parse_command(t_token **token, t_shell *shell)
{
	t_cmd	*cmd;
	int		arg_count, i;

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
		{
			if(ft_strncmp((*token)->value, "''", 3) == 0)
			{
				if(cmd->args[0] == NULL)
					cmd->args[i++] = safe_strdup((*token)->value);
			}
			else
				cmd->args[i++] = safe_strdup((*token)->value);
		}
		else
		{
			if (parse_redirections(token, cmd, shell) == -1){
				return NULL;
			}
		}
		if (*token)
			*token = (*token)->next;
	}
	cmd->args[i] = NULL;
	return (cmd);
}

t_cmd	*parse_tokens(t_shell *shell)
{
	t_cmd	*head = NULL;
	t_cmd	*last = NULL;
	t_cmd	*cmd;
	t_token	*token = shell->token;

	while (token)
	{
		if (!token->type)
		{
			ft_putstr_fd(token->value, 2);
			printf("\n");
			return (NULL);
		}
		token = token->next;
	}
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
