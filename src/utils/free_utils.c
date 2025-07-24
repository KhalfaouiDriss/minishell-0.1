/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-bech <sel-bech@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 20:34:22 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/24 13:59:11 by sel-bech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_split(char **lst)
{
	int	i;

	i = 0;
	if (!lst)
		return ;
	while (lst[i])
	{
		free(lst[i]);
		i++;
	}
	free(lst);
	lst = NULL;
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		if (tokens->value)
			free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
	tokens = NULL;
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		if (env->name)
			free(env->name);
		if (env->value)
			free(env->value);
		free(env);
		env = tmp;
	}
	env = NULL;
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*current;
	t_cmd	*next;
	int		i;

	current = cmds;
	while (current)
	{
		next = current->next;
		i = 0;
		while (current->args[i])
			free(current->args[i++]);
		free(current->args);
		free(current->infile);
		free(current->outfile);
		free(current);
		current = next;
	}
}

void	free_all(t_shell *shell, int enve)
{
	if (!shell)
		return ;
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
	if (shell->token)
		free_tokens(shell->token);
	if (shell->args)
		free_split(shell->args);
	if (shell->cmd_list)
		free_cmds(shell->cmd_list);
	if (shell->env && enve)
		free_env(shell->env);
	shell->token = NULL;
	shell->args = NULL;
	shell->cmd_list = NULL;
}
