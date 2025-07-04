#include "../../include/minishell.h"

static int	is_numeric(const char *str)
{
	int i = 0;

	if (!str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}
long long	ft_atoll(const char *str)
{
	long long	res = 0;
	int			sign = 1;

	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
		res = res * 10 + (*str++ - '0');
	return (res * sign);
}

int	ft_exit(t_shell *shell, char **args)
{
	long long	exit_code = 0;

	if (args[1])
	{
		if (!is_numeric(args[1]) || ft_strlen(args[1]) > 19)
		{
			write(2, "exit\n", 5);
			write(2, args[1], ft_strlen(args[1]));
			write(2, ": numeric argument required\n", 28);
			gc_free_all();
			exit(2);
		}
		exit_code = ft_atoll(args[1]);
		if (args[2])
		{
			write(2, "exit\n", 5);
			write(2, "minishell: exit: too many arguments\n", 36);
			shell->exit_status = 1;
			return (1);
		}
		printf("exit\n");
		gc_free_all();
		exit((unsigned char)exit_code);
	}
    printf("exit\n");
	gc_free_all();
	exit(shell->exit_status);
}
