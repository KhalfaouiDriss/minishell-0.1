#include "../../include/minishell.h"

void	add_env(t_shell *shell, char *name, char *value)
{
	t_env	*new_env = malloc(sizeof(t_env));
	t_env	*last;

	if (!new_env)
		return ;

	new_env->name = ft_strdup2(name);
	new_env->value = ft_strdup2(value);
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
}
char	*ft_strjoin2(const char *s1, const char *s2)
{
	size_t	lens1;
	size_t	lens2;
	char	*res;

	if (!s1 || !s2)
		return (NULL);
	lens1 = ft_strlen(s1);
	lens2 = ft_strlen(s2);
	res = malloc(lens1 + lens2 + 1);
	if (!res)
		return (NULL);
	ft_strlcpy(res, s1, ft_strlen(s1) + 1);
	ft_strlcat(res, s2, lens1 + lens2 + 1);
	return (res);
}


void init_new_env(t_shell *shell)
{
	int i;
	t_env *tmp;
	char *temp1;
	char *temp2;

	i = 0;
	tmp = shell->env;
	while (tmp)
	{
		temp1 = ft_strjoin2(tmp->name, "=");
		if(tmp->value)
		{
			temp2 = ft_strjoin2(temp1, tmp->value);
			free(temp1);
			shell->new_env[i] = ft_strdup(temp2);
			free(temp2);
		}
		else
		{
			shell->new_env[i] = ft_strdup(temp1);
			free(temp1);
		}
		i++;
		tmp = tmp->next;
	}
	shell->new_env[i - 1] = NULL;
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
	shell->new_env = ft_malloc((sizeof(char *) * (i + 1)));
	init_new_env(shell);
}