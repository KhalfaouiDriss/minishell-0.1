#include "../../include/minishell.h"

char	*ft_strdupv2(const char *str)
{
	char *dest;
	size_t len;

	if (!str)
		return NULL;
	len = ft_strlen(str);
	dest = malloc(len + 1);
	if (!dest)
		return NULL;
	ft_strlcpy(dest, str, len + 1);
	return dest;
}

void	free_new_env(char **env)
{
	int i = 0;
	if (!env)
		return;
	while (env[i])
		free(env[i++]);
	free(env);
}

void	add_env_node(t_shell *shell, t_env **env, char *key, char *value)
{
	t_env *new_node = malloc(sizeof(t_env));
	t_env *tmp;
	int count = 0;

	if (!new_node)
		return;
	new_node->name = ft_strdupv2(key);
	new_node->value = value ? ft_strdupv2(value) : NULL;
	new_node->next = NULL;

	if (!*env)
		*env = new_node;
	else
	{
		tmp = *env;
		while (tmp->next)
		{
			tmp = tmp->next;
			count++;
		}
		tmp->next = new_node;
	}
	free_new_env(shell->new_env);
	shell->new_env = malloc(sizeof(char *) * (count + 3)); 
	init_new_env(shell);
}

void	ft_export(t_shell *shell, char **args)
{
	int i = 1;
	t_env *tmp;
	char *key, *value;

	if (!args[i])
		return ft_env(shell->env, 0);

	while (args[i])
	{
		if (!ft_isalpha(args[i][0]) && args[i][0] != '_')
		{
			ft_putstr_fd("minishell: export: ", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd(": not a valid identifier\n", 2);
			shell->exit_status = 1;
			i++;
			continue;
		}

		char *equal = ft_strchr(args[i], '=');
		if (equal)
		{
			int key_len = equal - args[i];
			key = ft_substr(args[i], 0, key_len);
			value = equal + 1;
		}
		else
		{
			key = ft_strdup(args[i]);
			value = NULL;
		}

		if (ft_strchr(key, '-'))
		{
			ft_putstr_fd("minishell: export: ", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd(": not a valid identifier\n", 2);
			shell->exit_status = 1;
			i++;
			continue;
		}

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

		// free(key);
		i++;
	}
}
