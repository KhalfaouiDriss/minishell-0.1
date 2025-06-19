#include "../../include/minishell.h"

int count_args(t_token *token)
{
    int i;
    i = 0;
    while(token && token->type != PIPE)
    {
        if(token->type == OPTION || token->type == WORD)
            i++;
        if(token->type >= REDIR_IN && token->type <= REDIR_HEREDOC)
            token = token->next;
        token = token->next;

    }
    return i;
}

char *safe_strdup(char *s)
{
    if(!s)
        return NULL;
    return ft_strdup(s);
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

void init_str(t_cmd *cmd)
{
    cmd->infile = NULL;
    cmd->outfile = NULL;
    cmd->next = NULL;
    cmd->heredoc = 0;
    cmd->append = 0;
    cmd->outfile_fd = 0;
    cmd->c_flag = 0;
}

t_cmd *parse_tokens(t_shell *shell)
{
    t_cmd *head = NULL;
    t_cmd *last = NULL;
    t_token *token;

    token = shell->token;
    while (token)
    {
        if(!token->type)
        {
            printf("%s\n", token->value);
            // shell->exit_status = 127;
            return NULL;
        }
        token = token->next;
    }
    token = shell->token;
    while (token) {
        t_cmd *cmd = malloc(sizeof(t_cmd));
        if (!cmd)
            return NULL;
        init_str(cmd);
        int c = count_args(token);
        cmd->args = malloc((c + 1) * sizeof(char*));

        int i = 0;
        while(token && token->type != PIPE)
        {
            if(token->type == WORD || token->type == OPTION)
                cmd->args[i++] = safe_strdup(token->value);
            else if(token->type == REDIR_IN && token->next)
            {
                cmd->infile = safe_strdup(token->next->value);
                token = token->next;
            }
            else if(token->type == REDIR_HEREDOC && token->next)
            {
                cmd->heredoc_fd = handle_heredoc(token->next->value, shell);
                token = token->next;
            }
             else if(token->type == REDIR_OUT && token->next){
                cmd->outfile = safe_strdup(token->next->value);
                redirect_output(cmd,0);
                cmd->append = 0;
                token = token->next;
            }
             else if(token->type == REDIR_APPEND && token->next){
                cmd->outfile = safe_strdup(token->next->value);
                redirect_output(cmd,1);
                cmd->append = 1;
                token = token->next;
            }
            token = token->next;
        }
        cmd->args[i] = NULL;
        if(!head && !last){
            head = cmd;
            last = cmd;
        }
        else{
            last->next = cmd;
            last = last->next;
        }
        if(token && token->type == PIPE)
            token = token->next;
    }

    return head;
}
