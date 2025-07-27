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

static int	is_numeric(char *str)
{
	int	i = 0;

	if (!str || !*str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	is_overflow(char *str)
{
	long long	n;
	char		*end;

	n = ft_atoll(str);
	end = str;
	if (*end == '+' || *end == '-')
		end++;
	while (*end == '0')
		end++;
	if (ft_strlen(end) > 19)
		return (1);
	if (n > LLONG_MAX || n < LLONG_MIN)
		return (1);
	return (0);
}

static void	print_numeric_error(char *arg, t_shell *shell)
{
	write(2, "exit\n", 5);
	write(2, "exit: ", 6);
	write(2, arg, ft_strlen(arg));
	write(2, ": numeric argument required\n", 28);
	clean_shell(shell);
	close_fd_bin(shell->in, shell->out);
	exit(2);
}

int	ft_exit(t_shell *shell, char **args)
{
	char		*trimmed;
	long long	exit_code;

	if (args[1])
	{
		if (args[2])
			return print_too_many_args(shell);
		trimmed = ft_strtrim(args[1], " ");
		if (!trimmed || !is_numeric(trimmed) || is_overflow(trimmed))
			print_numeric_error(args[1], shell);
		exit_code = ft_atoll(trimmed);
		free(trimmed);
		write(1, "exit\n", 5);
		close_fd_bin(shell->in, shell->out);
		clean_shell(shell);
		exit((unsigned char)exit_code);
	}
	write(1, "exit\n", 5);
	close_fd_bin(shell->in, shell->out);
	clean_shell(shell);
	exit(shell->exit_status);
}
