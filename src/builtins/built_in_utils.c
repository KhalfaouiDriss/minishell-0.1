/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 11:27:39 by sel-bech          #+#    #+#             */
/*   Updated: 2025/07/20 10:53:52 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	print_too_many_args(t_shell *shell)
{
	write(2, "exit\n", 5);
	write(2, "exit: too many arguments\n", 26);
	shell->exit_status = 1;
	return (1);
}

void	exit_success(t_shell *shell, long long code)
{
	close_fd_bin(shell->in, shell->out);
	printf("exit\n");
	clean_shell(shell);
	exit((unsigned char)code);
}

long long	ft_atoll(char *str)
{
	long long	res;
	int			sign;

	res = 0;
	sign = 1;
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
