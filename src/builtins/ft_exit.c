/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:37:09 by sel-bech          #+#    #+#             */
/*   Updated: 2025/07/19 11:28:28 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
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

long long	ft_atoll(char *str)
{
	long long	res;
	int			sign;

	res = 0;
	sign = 1;
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

void	close_fd_bin(int in, int out)
{
	if (in != -1)
		close(in);
	if (out != -1)
		close(out);
}

void	affiche_err(char *arg, char *trim, t_shell *shell)
{
	write(2, "exit\n", 5);
	write(2, arg, ft_strlen(arg));
	write(2, ": numeric argument required\n", 28);
	free(trim);
	clean_shell(shell);
	close_fd_bin(shell->in, shell->out);
}

int	ft_exit(t_shell *shell, char **args)
{
	char		*trimmed;
	long long	exit_code;

	if (args[1])
	{
		trimmed = ft_strtrim(args[1], " ");
		if (!trimmed || !is_numeric(trimmed) || ft_strlen(trimmed) > 19)
			return (affiche_err(args[1], trimmed, shell), exit(2), 0);
		if (args[2])
			return (handle_exit_error(shell, args, trimmed));
		exit_code = ft_atoll(trimmed);
		free(trimmed);
		exit_success(shell, exit_code);
	}
	close_fd_bin(shell->in, shell->out);
	printf("exit\n");
	clean_shell(shell);
	exit(shell->exit_status);
}
