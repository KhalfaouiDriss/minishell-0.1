#include "../../include/minishell.h"

void	add_env(t_shell *shell, char *name, char *value)
{
	t_env	*new_env = malloc(sizeof(t_env));
	t_env	*last;

	if (!new_env)
		return ;

	new_env->name = ft_strdup(name);
	new_env->value = ft_strdup(value);
	new_env->next = NULL;

	if (!shell->env)
		shell->env = new_env;
	else
	{
		last = shell->env;
		while (last->next)
			last = last->next;
		last->next = new_env;
	}
}

void	process_env_variable(t_shell *shell, char *env_str)
{
	char	*egl = ft_strchr(env_str, '=');
	char	*name;
	char	*value;

	if (!egl)
		return ;

	name = ft_substr(env_str, 0, ft_strlen(env_str) - ft_strlen(egl));
	value = egl + 1;

	if (!name)
		return ;

	add_env(shell, name, value);
	free(name);
}

void	init_env(t_shell *shell, char **envp)
{
	int	i = 0;

	shell->env = NULL;
	while (envp[i])
	{
		process_env_variable(shell, envp[i]);
		i++;
	}
}