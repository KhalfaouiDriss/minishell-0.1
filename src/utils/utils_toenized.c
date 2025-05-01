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

char *find_env_node(t_env *env, char *key)
{
    t_env *tmp = env;
    size_t key_len;

    key_len = ft_strlen(key);
    while (tmp)
    {
        if (ft_strncmp(tmp->name, key, key_len) == 0 && tmp->name[key_len] == '\0')
        {
            // printf("found %s\n", tmp->name);
            return tmp->value;
        }
        tmp = tmp->next;
    }
    return NULL;
}
char *handle_variable_token(char *str, int *i, t_shell *shell)
{
	int		start;
	int		len;
	char	*var_name;
	char	*var_value = NULL;
	t_env	*env = shell->env;

	(*i)++; // تجاوز علامة $
	start = *i;

	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;

	len = *i - start;
	if (len == 0)
		var_name = ft_strdup("");
	else
		var_name = ft_substr(str, start, len);

	while (env)
	{
		if (ft_strncmp(env->name, var_name, ft_strlen(var_name)) == 0)
		{
			var_value = ft_strdup(env->value);
			break;
		}
		env = env->next;
	}
	if (!var_value)
		var_value = ft_strdup(""); // متغير غير موجود

	free(var_name);
	return var_value; // أرجع القيمة بدل إضافتها هنا
}





// تحويل نوع التوكن إلى سترينغ للعرض
const char *token_type_to_str(int type)
{
    switch (type)
    {
    case WORD:
        return "WORD";
    case OPTION:
        return "OPTION";
    case PIPE:
        return "PIPE";
    case REDIR_IN:
        return "REDIR_IN";
    case REDIR_OUT:
        return "REDIR_OUT";
    case REDIR_APPEND:
        return "REDIR_APPEND";
    case REDIR_HEREDOC:
        return "REDIR_HEREDOC";
    case ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

// طباعة جميع التوكنات للديباغ
void print_tokens(t_token *head)
{
    while (head)
    {
        printf("Token: %s | Type: %s | Error type: %d | quote type : %d", head->value, token_type_to_str(head->type), head->error, head->quot_type);
        // if (head->error)
        //     printf(" | Error type: %d", head->error);
        printf("\n");
        head = head->next;
    }
}
