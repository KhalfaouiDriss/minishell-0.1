/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 21:59:52 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/15 15:42:51 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_valid_export_key_start(char *str)
{
	int	c;

	c = (ft_isalpha(str[0]) || str[0] == '_');
	return (c);
}

void	print_export_error(t_shell *shell, char *arg, int *i)
{
	ft_putstr_fd("minishell: export: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": not a valid identifier\n", 2);
	shell->exit_status = 1;
	(*i)++;
}

void	parse_key_value(char *arg, char **key, char **value)
{
	char	*equal;
	int		key_len;

	equal = ft_strchr(arg, '=');
	if (equal)
	{
		key_len = equal - arg;
		*key = ft_substr(arg, 0, key_len);
		if (ft_strlen(equal + 1))
			*value = equal + 1;
		else
		{
			*value = NULL;
			printf("=====\n");
		}
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
}

void	handle_export_key(t_shell *shell, char *key, char *value)
{
	t_env	*tmp;

	tmp = shell->env;
	while (tmp && ft_strncmp(tmp->name, key, ft_strlen(key) + 1))
		tmp = tmp->next;
	if (tmp)
	{
		if (value)
		{
			free(tmp->value);
			tmp->value = ft_strdupv2(value);
		}
	}
	else
		add_env_node(shell, &(shell->env), key, value);
}

void	ft_export(t_shell *shell, char **args)
{
	int		i;
	t_env	*tmp;
	char	*key;
	char	*value;

	i = 1;
	if (!args[i])
		return (ft_env(shell->env, 0));
	while (args[i])
	{
		if (!is_valid_export_key_start(args[i]))
		{
			print_export_error(shell, args[i], &i);
			continue ;
		}
		parse_key_value(args[i], &key, &value);
		if (ft_strchr(key, '-'))
		{
			print_export_error(shell, args[i], &i);
			continue ;
		}
		handle_export_key(shell, key, value);
		i++;
	}
}
