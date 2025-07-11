#include "../../include/minishell.h"

char	*get_env_value(t_env *env, const char *name)
{
	while (env)
	{
		if (ft_strncmp(env->name, name, ft_strlen(name)) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	update_env_var(t_env *env, const char *name, const char *value)
{
	t_env	*tmp;
	t_env	*new_var;

	tmp = env;
	while (tmp)
	{
		if (ft_strncmp(tmp->name, name, ft_strlen(name)) == 0)
		{
			if (tmp->value)
				free(tmp->value);
			if (value)
				tmp->value = ft_strdupv2(value);
			else
				tmp->value = NULL;
			return ;
		}
		tmp = tmp->next;
	}
	new_var = ft_malloc(sizeof(t_env));
	if (!new_var)
		return ;
	new_var->name = ft_strdup(name);
	new_var->value = value ? ft_strdup(value) : NULL;
	new_var->next = NULL;
	tmp = env;
	if (!tmp)
		return ;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_var;
}

void	ft_cd(t_shell *shell, char **args)
{
	const char	*target_dir;
	char		*oldpwd;
	char		*newpwd;
	char		*fallback;
	int			attempts;
	char		*tmp;
	struct stat	sb;
	char		*buffer;

	if (args[2])
	{
		write(2, "cd: too many arguments\n", 24);
		shell->exit_status = 1;
		return ;
	}
	if (!args[1])
	{
		target_dir = get_env_value(shell->env, "HOME");
		if (!target_dir)
		{
			write(2, "cd: HOME not set\n", 17);
			shell->exit_status = 1;
			return ;
		}
	}
	else
		target_dir = args[1];
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		fallback = get_env_value(shell->env, "PWD");
		attempts = 10;
		if (chdir(fallback) == -1 && attempts--)
		{
			tmp = fallback;
			fallback = ft_strjoin(tmp, "/..");
			free(tmp);
		}
		if (fallback)
		{
			update_env_var(shell->env, "PWD", fallback);
			free(fallback);
		}
	}
	if (stat(target_dir, &sb) == 0)
	{
		if (S_ISDIR(sb.st_mode))
		{
			if (!(sb.st_mode & S_IXUSR))
				write(2, "Permission denied\n", 19);
		}
		else
			write(2, "Not a directory\n", 17);
		shell->exit_status = 1;
	}
	if (chdir(target_dir) == -1)
	{
		buffer = ft_strjoin(target_dir, ": No such file or directory\n");
		write(2, buffer, ft_strlen(buffer));
		shell->exit_status = 1;
		free(oldpwd);
		return ;
	}
	update_env_var(shell->env, "OLDPWD", oldpwd);
	free(oldpwd);
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
	{
		perror("cd: getcwd after chdir");
		shell->exit_status = 1;
		return ;
	}
	update_env_var(shell->env, "PWD", newpwd);
	free(newpwd);
	shell->exit_status = 0;
}
