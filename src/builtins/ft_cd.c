#include "../../include/minishell.h"

char *get_env_value(t_env *env, const char *name)
{
	while (env)
	{
		if (ft_strncmp(env->name, name, ft_strlen(name)) == 0)
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
		if (ft_strncmp(tmp->name, name, ft_strlen(name)) == 0)
		{
			tmp->value = value ? ft_strdup(value) : NULL;
			return;
		}
		tmp = tmp->next;
	}

	t_env *new_var = ft_malloc(sizeof(t_env));
	if (!new_var)
		return;

	new_var->name = ft_strdup(name);
	new_var->value = value ? ft_strdup(value) : NULL;
	new_var->next = NULL;

	tmp = env;
	if (!tmp)
		return; 
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_var;
}

void ft_cd(t_shell *shell, char **args)
{
	const char *target_dir;
	char *oldpwd;
	char *newpwd;

	if (args[2])
	{
		write(2, "cd: too many arguments\n", 24);
		shell->exit_status = 1;
		return;
	}

	if (!args[1])
	{
		target_dir = get_env_value(shell->env, "HOME");
		if (!target_dir)
		{
			write(2, "cd: HOME not set\n", 17);
			shell->exit_status = 1;
			return;
		}
	}
	else
		target_dir = args[1];

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror("cd: getcwd");
		free_split(args);
		args[0] = "cd";
		args[1] = "HOME";
		ft_cd(shell, args);
		shell->exit_status = 1;
		return;
	}

	if (chdir(target_dir) == -1)
	{
		write(2, "cd: ", 4);
		write(2, target_dir, ft_strlen(target_dir));
		write(2, ": No such file or directory\n", 29);
		
		shell->exit_status = 1;
		free(oldpwd);
		return;
	}

	update_env_var(shell->env, "OLDPWD", oldpwd);
	free(oldpwd);

	newpwd = getcwd(NULL, 0);
	if (!newpwd)
	{
		perror("cd: getcwd after chdir");
		shell->exit_status = 1;
		return;
	}
	update_env_var(shell->env, "PWD", newpwd);
	free(newpwd);

	shell->exit_status = 0;
}
