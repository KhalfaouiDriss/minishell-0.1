/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_hand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 20:36:53 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/18 18:37:49 by dkhalfao         ###   ########.fr       */
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
	exit(2);
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

void	in_red(t_cmd *cmd, t_token *token)
{
	cmd->infile = ft_strdup(token->next->value);
	redirect_input(cmd->infile, cmd);
	*fake_glb() = cmd->infile_fd;
}
