#include "../../include/minishell.h"

int is_operator(char *str, int i)
{
	if (str[i] == '<')
	{
		if (str[i + 1] == '<')
			return 2;
		else
			return 1;
	}
	else if (str[i] == '>')
	{
		if (str[i + 1] == '>')
			return 2;
		else
			return 1;
	}
	else if (str[i] == '|')
	{
		return 1;
	}
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
	char quote;

	i = skip_spaces(input, 0);
	if (!input[i])
		return 0;

	while (input[i])
	{
		if (input[i] == '"' || input[i] == '\'')
		{
			quote = input[i++];
			while (input[i] && input[i] != quote)
				i++;
			if (!input[i])
			{
				printf("syntax error: unmatched quote\n");
				shell->exit_status = 130;
				return 1;
			}
			i++;
			continue;
		}

		i = skip_spaces(input, i);
		op_len = is_operator(input, i);
		if (op_len)
		{
			i += op_len;
			i = skip_spaces(input, i);

			// ✅ السماح فقط بوجود '<<' بعد أي operator، والباقي نرفضه
			if (!input[i] || (is_operator(input, i) && !(input[i] == '<' && input[i + 1] == '<'))) {
				printf("syntax error near unexpected token '%.*s'\n", op_len, &input[i - op_len]);
				shell->exit_status = 2;
				return 1;
			}
		}
		else
		{
			while (input[i] && !isspace(input[i]) && !is_operator(input, i))
			{
				if (input[i] == '"' || input[i] == '\'')
				{
					quote = input[i++];
					while (input[i] && input[i] != quote)
						i++;
					if (!input[i])
					{
						printf("syntax error: unmatched quote\n");
						shell->exit_status = 130;
						return 1;
					}
				}
				i++;
			}
		}
	}
	return 0;
}


