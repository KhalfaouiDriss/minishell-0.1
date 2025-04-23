#include "../../include/minishell.h"

int count_args(t_token *token)
{
    int count = 0;
    while (token && token->type != PIPE)
    {
        if (token->type == WORD || token->type == OPTION)
            count++;
        else if (token->type >= REDIR_IN && token->type <= REDIR_HEREDOC)
            token = token->next;
        token = token->next;
    }
    return count;
}

char *safe_strdup(const char *s)
{
    if (!s) 
        return NULL;
    return ft_strdup(s);
}

void free_tokens(t_token *tokens)
{
    t_token *current;
    t_token *next;
    
    current = tokens;
    while (current)
    {
        next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
}

void free_cmds(t_cmd *cmds)
{
    t_cmd *current;
    t_cmd *next;
    
    current = cmds;
    while (current)
    {
        next = current->next;
        int i = 0;
        while (current->args[i])
            free(current->args[i++]);
        free(current->args);
        free(current->infile);
        free(current->outfile);
        free(current);
        current = next;
    }
}

t_cmd *parse_tokens(t_token *token)
{
    t_cmd *head = NULL;
    t_cmd *last = NULL;

    while (token) {
        t_cmd *cmd = malloc(sizeof(t_cmd));
        if (!cmd)
            return NULL;
            
        cmd->infile = NULL;
        cmd->outfile = NULL;
        cmd->next = NULL;
        cmd->heredoc = 0;
        cmd->append = 0;
        
        int c = count_args(token);
        cmd->args = malloc((c + 1) * sizeof(char*));

        int i = 0;
        while (token && token->type != PIPE)
        {
            if (!token->type)
            {
                printf("%s\n", token->value);
                return NULL;
            }
            if (token->type == WORD || token->type == OPTION)
                cmd->args[i++] = safe_strdup(token->value);
            else if (token->type == REDIR_IN && token->next)
            {
                cmd->infile = safe_strdup(token->next->value);
                cmd->heredoc = 0;
                token = token->next;
            }
            else if (token->type == REDIR_HEREDOC && token->next)
            {
                cmd->infile = safe_strdup(token->next->value);
                cmd->heredoc = 1;
                token = token->next;
            }
            else if (token->type == REDIR_OUT && token->next)
            {
                cmd->outfile = safe_strdup(token->next->value);
                cmd->append = 0;
                token = token->next;
            }
            else if (token->type == REDIR_APPEND && token->next)
            {
                cmd->outfile = safe_strdup(token->next->value);
                cmd->append = 1;
                token = token->next;
            }
            token = token->next;
        }

        cmd->args[i] = NULL;

        if (!head)
            head = cmd;
        else
            last->next = cmd;
        last = cmd;

        if (token && token->type == PIPE)
            token = token->next;
    }

    return head;
}
