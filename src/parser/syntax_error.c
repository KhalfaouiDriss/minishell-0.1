#include "../../include/minishell.h"

int is_operator(char *str, int i)
{
	if (str[i] == '<')
		return (str[i + 1] == '<') ? 2 : 1;
	if (str[i] == '>')
		return (str[i + 1] == '>') ? 2 : 1;
	if (str[i] == '|')
		return 1;
	return 0;
}

int skip_spaces(char *str, int i)
{
	while (str[i] && isspace(str[i]))
		i++;
	return i;
}

int check_syntax_errors(t_shell *shell)
{
	int i = 0, op_len;
	char *input = shell->input;

	i = skip_spaces(input, 0);
	if (!input[i])
		return 0; // empty input is OK

	while (input[i])
	{
		i = skip_spaces(input, i);
		op_len = is_operator(input, i);
		if (op_len)
		{
			i += op_len;
			i = skip_spaces(input, i);
			if (!input[i] || is_operator(input, i)) {
				printf("syntax error near unexpected token '%.*s'\n", op_len, &input[i - op_len]);
				return 1;
			}
		}
		else
		{
			while (input[i] && !isspace(input[i]) && !is_operator(input, i))
				i++;
		}
	}
	return 0;
}
