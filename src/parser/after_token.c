#include "../../include/minishell.h"

int count_args(t_token *token) {
    int count = 0;
    while (token && token->type != PIPE) {
        if (token->type == WORD)
            count++;
        else if (token->type >= REDIR_IN && token->type <= REDIR_HEREDOC)
            token = token->next;
        token = token->next;
    }
    return count;
}

t_cmd *parse_tokens(t_token *token) {
    t_cmd *head = NULL;
    t_cmd *last = NULL;

    while (token) {
        t_cmd *cmd = calloc(1, sizeof(t_cmd));
        int argc = count_args(token);
        cmd->args = calloc(argc + 1, sizeof(char*));

        int i = 0;
        while (token && token->type != PIPE) {
            if (token->type == WORD) {
                cmd->args[i++] = safe_strdup(token->value);
            }
            else if (token->type == REDIR_IN && token->next) {
                cmd->infile = safe_strdup(token->next->value);
                cmd->heredoc = 0;
                token = token->next;
            }
            else if (token->type == REDIR_HEREDOC && token->next) {
                cmd->infile = safe_strdup(token->next->value);
                cmd->heredoc = 1;
                token = token->next;
            }
            else if (token->type == REDIR_OUT && token->next) {
                cmd->outfile = safe_strdup(token->next->value);
                cmd->append = 0;
                token = token->next;
            }
            else if (token->type == REDIR_APPEND && token->next) {
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
