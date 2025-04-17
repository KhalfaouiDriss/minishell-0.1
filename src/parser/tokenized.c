#include "../../include/minishell.h"

// ------------------- Utils -------------------

int is_special(char c) {
    return (c == '|' || c == '>' || c == '<');
}

char *substr(const char *src, int start, int end) {
    char *str = malloc(end - start + 1);
    if (!str) return NULL;
    memcpy(str, &src[start], end - start);
    str[end - start] = '\0';
    return str;
}

t_token *new_token(char *val, int type) {
    t_token *t = malloc(sizeof(t_token));
    if (!t) return NULL;
    t->value = strdup(val);
    t->type = type;
    t->next = NULL;
    t->error = NULL;
    return t;
}

void add_token(t_token **head, t_token *new) {
    if (!*head)
        *head = new;
    else {
        t_token *tmp = *head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
}

// ------------------- Main Lexer Function -------------------

t_token *lexer_split_to_tokens(const char *input) {
    t_token *head = NULL;
    int i = 0;

    while (input[i]) {
        while (isspace(input[i]))
            i++;

        if (!input[i])
            break;

        // Handle quoted strings or complex words
        if (input[i] == '\'' || input[i] == '"') {
            char quote = input[i++];
            int start = i;
            while (input[i] && input[i] != quote)
                i++;
            char *val = substr(input, start, i);
            add_token(&head, new_token(val, WORD));
            free(val);
            if (input[i]) i++;
        }
        // Handle redirection operators (>, >>, <, <<)
        else if (is_special(input[i])) {
            int start = i;
            if ((input[i] == '<' || input[i] == '>') && input[i] == input[i+1])
                i += 2;
            else
                i += 1;

            char *val = substr(input, start, i);
            int type = (strcmp(val, "|") == 0) ? PIPE :
                       (strcmp(val, "<") == 0) ? REDIR_IN :
                       (strcmp(val, ">") == 0) ? REDIR_OUT :
                       (strcmp(val, ">>") == 0) ? REDIR_APPEND :
                       (strcmp(val, "<<") == 0) ? REDIR_HEREDOC : ERROR;
            add_token(&head, new_token(val, type));
            free(val);
        }
        // Handle normal words (commands, options)
        else {
            int start = i;
            int in_quote = 0;
            char quote = 0;

            while (input[i]) {
                if (!in_quote && (input[i] == '\'' || input[i] == '"')) {
                    in_quote = 1;
                    quote = input[i++];
                    while (input[i] && input[i] != quote)
                        i++;
                    if (input[i]) i++; // skip closing quote
                    in_quote = 0;
                } else if (!in_quote && (isspace(input[i]) || is_special(input[i]))) {
                    break;
                } else {
                    i++;
                }
            }

            char *val = substr(input, start, i);
            int type = WORD;
            if (val[0] == '-' && strlen(val) > 1)
                type = OPTION;
            add_token(&head, new_token(val, type));
            free(val);
        }
    }

    return head;
}

// ------------------- Debug -------------------

void print_tokens(t_token *head) {
    while (head) {
        printf("Token: %-15s | Type: %d\n", head->value, head->type);
        head = head->next;
    }
}