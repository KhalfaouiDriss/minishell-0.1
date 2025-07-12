#include "../../include/minishell.h"

int is_operator(char *str, int i)
{
	if (str[i] == '<')
	{
		if (str[i + 1] == '<')
			return 2;
		return 1;
	}
	else if (str[i] == '>')
	{
		if (str[i + 1] == '>')
			return 2;
		return 1;
	}
	else if (str[i] == '|')
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
	int i = 0, op_len, last_was_op = 1;
	char *input = shell->input;

	i = skip_spaces(input, 0);
	if (!input[i])
		return 0; 
	i = 0;
	while (input[i])
	{
		i = skip_spaces(input, i);
		if (!input[i])
			break;

		op_len = is_operator(input, i);
		if (op_len)
		{
			if (last_was_op) {
				printf("syntax error near unexpected token '%.*s'\n", op_len, &input[i]);
				return 1;
			}
			last_was_op = 1;
			i += op_len;
		}
		else
		{
			while (input[i] && !isspace(input[i]) && !is_operator(input, i))
				i++;
			last_was_op = 0;
		}
	}

	if (last_was_op) {
		printf("syntax error: unexpected end of input after operator\n");
		return 1;
	}
	return 0;
}

