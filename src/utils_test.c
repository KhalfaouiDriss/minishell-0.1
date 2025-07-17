#include "../include/minishell.h"


const char	*token_type_to_str(int type)
{
	switch (type)
	{
	case WORD:
		return ("WORD");
	case OPTION:
		return ("OPTION");
	case PIPE:
		return ("PIPE");
	case REDIR_IN:
		return ("REDIR_IN");
	case REDIR_OUT:
		return ("REDIR_OUT");
	case REDIR_APPEND:
		return ("REDIR_APPEND");
	case REDIR_HEREDOC:
		return ("REDIR_HEREDOC");
	case ERROR:
		return ("ERROR");
	default:
		return ("UNKNOWN");
	}
}

void	print_tokens(t_token *head)
{
	while (head)
	{
		printf("Token: %s | Type: %s | Error type: %d | quote type : %d | ebag :%d", head->value, token_type_to_str(head->type), head->error,head->quot_type, head->ebag);
		printf("\n");
		head = head->next;
	}
}