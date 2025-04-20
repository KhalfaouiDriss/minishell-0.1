#include "../../include/minishell.h"

// يتحقق مما إذا كان الحرف من الرموز الخاصة
int is_special(char c)
{
    return (c == '|' || c == '>' || c == '<');
}

// ينشئ توكن جديد بشكل آمن
t_token *new_token(char *val, int type, int error_type)
{
    t_token *t = (t_token *)malloc(sizeof(t_token));
    if (!t)
        return NULL;
    if (val)
    {
        t->value = strdup(val);
        if (!t->value)
        {
            free(t);
            return NULL;
        }
    }
    else
    {
        t->value = NULL;
    }

    t->type = type;
    t->error = error_type;
    t->next = NULL;
    return t;
}

// يضيف توكن إلى نهاية اللائحة المرتبطة
void add_token(t_token **head, t_token *new)
{
    if (!new || !new->value || new->value[0] == '\0')
    {
        if (new)
        {
            free(new->value);
            free(new);
        }
        return;
    }

    if (!*head)
    {
        *head = new;
        return;
    }

    t_token *tmp = *head;
    while (tmp->next)
        tmp = tmp->next;

    tmp->next = new;
}

// يتعامل مع التوكن من نوع متغير بيئة
void handle_variable_token(const char *input, int *i, t_token **head)
{
    int start;
    int var_len;
    char *var_name = NULL;
    char *var_value = NULL;

    (*i)++;
    start = *i;

    // حالة $?
    if (input[*i] == '?')
    {
        (*i)++;
        add_token(head, new_token(strdup("0"), WORD, 0)); // تأكدنا من strdup
        return;
    }

    // جمع اسم المتغير
    while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
        (*i)++;

    if (start == *i)
    {
        // مجرد $ بدون اسم
        add_token(head, new_token(strdup("$"), WORD, 0));
        return;
    }

    var_len = *i - start;
    var_name = ft_substr(input, start, var_len);
    if (!var_name)
        return;

    var_value = getenv(var_name);

    if (var_value)
        add_token(head, new_token(strdup(var_value), WORD, 0)); // strdup هنا مهم لتجنب مشاركة الذاكرة
    else
        add_token(head, new_token(strdup(""), WORD, 0)); // نعطي توكن فارغ

    free(var_name);
}

// تحويل نوع التوكن إلى سترينغ للعرض
const char *token_type_to_str(int type)
{
    switch (type)
    {
        case WORD: return "WORD";
        case OPTION: return "OPTION";
        case PIPE: return "PIPE";
        case REDIR_IN: return "REDIR_IN";
        case REDIR_OUT: return "REDIR_OUT";
        case REDIR_APPEND: return "REDIR_APPEND";
        case REDIR_HEREDOC: return "REDIR_HEREDOC";
        case ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

// طباعة جميع التوكنات للديباغ
void print_tokens(t_token *head)
{
    while (head)
    {
        printf("Token: %s | Type: %s | Error type: %d", head->value, token_type_to_str(head->type), head->error);
        // if (head->error)
        //     printf(" | Error type: %d", head->error);
        printf("\n");
        head = head->next;
    }
}
