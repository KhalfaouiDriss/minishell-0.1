#include "../../include/minishell.h"

char *get_env_value(t_env *env, const char *name)
{
	while (env)
	{
		if (strcmp(env->name, name) == 0)
			return env->value;
		env = env->next;
	}
	return NULL;
}

void update_env_var(t_env *env, const char *name, const char *value)
{
	t_env *tmp = env;
	while (tmp)
	{
		if (strcmp(tmp->name, name) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			return;
		}
		tmp = tmp->next;
	}

	// Add new variable if not found
	t_env *new_var = malloc(sizeof(t_env));
	if (!new_var)
		return;
	new_var->name = ft_strdup(name);
	new_var->value = ft_strdup(value);
	new_var->next = NULL;

	tmp = env;
	while (tmp && tmp->next)
		tmp = tmp->next;
	if (tmp)
		tmp->next = new_var;
}

void ft_cd(t_shell *shell, char **args)
{
	const char *dir;
	char *oldpwd;
	char *cwd;

	if (!args[0] || strcmp(args[0], "cd") != 0)
		return;

	if (args[1] == NULL)
	{
		dir = get_env_value(shell->env, "HOME");
		if (!dir)
		{
			printf("-bash: cd: HOME not set\n");
			return;
		}
	}
	else
		dir = args[1];

	oldpwd = getcwd(NULL, 0); // Save OLDPWD before changing directory
	if (!oldpwd)
	{
		perror("getcwd");
		return;
	}

	if (chdir(dir) == -1)
	{
		printf("-bash: cd: %s: No such file or directory\n", dir);
		free(oldpwd);
		return;
	}

	cwd = getcwd(NULL, 0); // New PWD
	if (!cwd)
	{
		perror("getcwd");
		free(oldpwd);
		return;
	}

	update_env_var(shell->env, "OLDPWD", oldpwd);
	update_env_var(shell->env, "PWD", cwd);

	free(oldpwd);
	free(cwd);
}
