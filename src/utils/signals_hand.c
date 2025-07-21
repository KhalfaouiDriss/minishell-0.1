/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_hand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 20:36:53 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/19 15:01:41 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	global_state(int set)
{
	static int	state;

	if (set != -1)
		state = set;
	return (state);
}

char	*get_variable(t_shell *shell, char *line, int *i)
{
	char	*var_name;
	char	*var_value;
	int		start;

	(*i)++;
	start = *i;
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
		(*i)++;
	var_name = ft_substr(line, start, *i - start);
	var_value = find_env_node(shell->env, var_name);
	if (var_value)
		return (var_value);
	else
		return (ft_strdup(""));
}

void	handel_sig(int sig)
{
	t_shell	*shell;

	(void)sig;
	write(1, "\n", 1);
	shell = get_shell();
	close(shell->her_fd);
	clean_shell(shell);
	exit(130);
}

void	get_sig(int sig)
{
	int	state;

	state = global_state(-1);
	if (state == 1)
		return ;
	if (sig == SIGINT)
		global_state(2);
	if (state != 3)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}
