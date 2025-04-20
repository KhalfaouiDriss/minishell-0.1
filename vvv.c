#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "include/minishell.h"

void handle_quots_token(const char *input, int *i, t_token **head)
{
    int start;
    char quote;
    char *tok = NULL;
    char *op = NULL;
    char *fin = NULL;
    int type = WORD;

    quote = input[*i];  // Store the quote type
    (*i)++;             // Skip the quote
    start = *i;

    while (input[*i] && input[*i] != quote)
        (*i)++;

    if (!input[*i]) {
        add_token(head, new_token(NULL, 0, QUETS_INVA));
        return;
    }

    tok = ft_substr(input, start, *i - start);

    if (tok && tok[0] == '-' && ft_strlen(tok) == 1) {
        (*i)++;
        while (input[*i] && input[*i] == ' ')
            (*i)++;
        if (input[*i] == '"' || input[*i] == '\'') {
            char q = input[*i];
            (*i)++;
            start = *i;
            while (input[*i] && input[*i] != q)
                (*i)++;
            if (!input[*i]) {
                free(tok);
                add_token(head, new_token(NULL, 0, QUETS_INVA));
                return;
            }
            op = ft_substr(input, start, *i - start);
        } else {
            start = *i;
            while (input[*i] && !is_special(input[*i]) && input[*i] != ' ')
                (*i)++;
            op = ft_substr(input, start, *i - start);
        }
        fin = ft_strjoin(tok, op);
        free(tok);
        free(op);
        tok = fin;
        type = OPTION;
    }

    add_token(head, new_token(tok, type, 0));
    free(tok);
    (*i)++;  // Skip closing quote
}

int is_charachter(char c)
{
    return (c != '"' && c != '\'' && !is_special(c) && c != ' ');
}

t_token *lexer_split_to_tokens(const char *input)
{
    int i = 0;
    int start;
    t_token *head = NULL;

    while (input[i]) {
        while (input[i] == ' ')
            i++;

        if (input[i] == '"' || input[i] == '\'')
            handle_quots_token(input, &i, &head);
        else if (is_special(input[i]))
            handle_special_token(input, &i, &head);
        else if (input[i] == '$')
            handle_variable_token(input, &i, &head);
        else {
            start = i;
            while (is_charachter(input[i]))
                i++;
            add_token(&head, new_token(ft_substr(input, start, i - start), WORD, 0));
        }
    }
    return head;
}

// ------------------- Test -------------------

int main(int argc, char **argv) {
    if (argc < 2)
        return 1;

    t_token *tokens = lexer_split_to_tokens(argv[1]);
    print_tokens(tokens);
    return 0;
}
