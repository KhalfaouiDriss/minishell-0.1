#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

// Token types
#define ERROR         0
#define WORD          1
#define PIPE          2
#define REDIR_IN      3
#define REDIR_OUT     4
#define REDIR_APPEND  5
#define REDIR_HEREDOC 6
#define OPTION        7

typedef struct s_token {
    char *value;
    int type;
    struct s_token *next;
    void *error;
} t_token;

// ------------------- Test -------------------

int main(int argc, char **argv) {
    if (argc < 2)
        return 1;

    t_token *tokens = lexer_split_to_tokens(argv[1]);
    print_tokens(tokens);
    return 0;
}
