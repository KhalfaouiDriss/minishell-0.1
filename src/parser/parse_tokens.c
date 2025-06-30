#include "../../include/minishell.h"

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

char	*safe_strdup(char *s)
{
	if (!s)
		return (NULL);
	return (ft_strdup(s));
}

void	init_str(t_cmd *cmd)
{
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->next = NULL;
	cmd->heredoc = 0;
	cmd->append = 0;
	cmd->outfile_fd = 0;
	cmd->c_flag = 0;
}

static void	parse_redirections(t_token **token, t_cmd *cmd, t_shell *shell)
{
	if ((*token)->type == REDIR_IN && (*token)->next)
	{
		cmd->infile = safe_strdup((*token)->next->value);
		(*token) = (*token)->next;
	}
	else if ((*token)->type == REDIR_HEREDOC && (*token)->next)
	{
		cmd->heredoc_fd = handle_heredoc((*token)->next->value, shell);
		
		(*token) = (*token)->next;
	}
	else if ((*token)->type == REDIR_OUT && (*token)->next)
	{
		cmd->outfile = safe_strdup((*token)->next->value);
		redirect_output(cmd, 0);
		cmd->append = 0;
		(*token) = (*token)->next;
	}
	else if ((*token)->type == REDIR_APPEND && (*token)->next)
	{
		cmd->outfile = safe_strdup((*token)->next->value);
		redirect_output(cmd, 1);
		cmd->append = 1;
		(*token) = (*token)->next;
	}
}

static t_cmd	*parse_command(t_token **token, t_shell *shell)
{
	t_cmd	*cmd;
	int		arg_count;
	int		i;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	init_str(cmd);
	arg_count = count_args(*token);
	cmd->args = malloc((arg_count + 1) * sizeof(char *));
	i = 0;
	while (*token && (*token)->type != PIPE)
	{
		if ((*token)->type == WORD || (*token)->type == OPTION)
			cmd->args[i++] = safe_strdup((*token)->value);
		else
			parse_redirections(token, cmd, shell);
		(*token) = (*token)->next;
	}
	cmd->args[i] = NULL;
	return (cmd);
}

t_cmd	*parse_tokens(t_shell *shell)
{
	t_cmd	*head;
	t_cmd	*last;
	t_cmd	*cmd;
	t_token	*token;

	token = shell->token;
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
	head = NULL;
	last = NULL;
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
