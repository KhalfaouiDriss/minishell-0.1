#include "../../include/minishell.h"

char *get_env_value(t_env *env, char *name)
{
	while (env)
	{
		if (ft_strncmp(env->name, name, ft_strlen(name) + 1) == 0)
			return env->value;
		env = env->next;
	}
	return NULL;
}

void update_env_var(t_env *env, char *name, char *value)
{
	t_env *tmp = env;
	while (tmp)
	{
		if (ft_strncmp(env->name, name, ft_strlen(name) + 1) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			return;
		}
		tmp = tmp->next;
	}

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

	if(args[2])
	{
		write(2, "too many arguments\n", 20);
		shell->exit_status = 1;
		return;
	}
	if (args[1] == NULL)
	{
		dir = get_env_value(shell->env, "HOME");
		if (!dir)
		{
			write(2, "-bash: cd: HOME not set\n", 25);
			shell->exit_status = 1;
			return;
		}
	}
	else
		dir = args[1];

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror("minishell");
		shell->exit_status = 1;
		return;
	}

	if (chdir(dir) == -1)
	{
		write(2, "cd: ", 5);
		write(2, dir, ft_strlen(dir));
		write(2, ": No such file or directory\n", 29);
		shell->exit_status = 1;
		free(oldpwd);
		return;
	}

	update_env_var(shell->env, "OLDPWD", oldpwd);
	free(oldpwd);
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		shell->exit_status = 1;
		return;
	}
	update_env_var(shell->env, "PWD", cwd);
	free(cwd);
}
