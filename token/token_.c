/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutaik <rboutaik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 16:07:59 by aachalla          #+#    #+#             */
/*   Updated: 2024/06/02 19:31:32 by rboutaik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	token_intervalle(char *cmd, char *delem[], int *i_start, int *i_final)
{
	int	i_dice;

	i_dice = 0;
	while (cmd[i_dice] == 32)
		i_dice++;
	*i_start = i_dice;
	while (cmd[i_dice])
	{
		if (ft_is_there(cmd + i_dice, delem, ft_check_qts(cmd, i_dice)) > 0)
			return ;
		if (cmd[i_dice] != 32 && ft_is_there(cmd + i_dice, delem,
				ft_check_qts(cmd, i_dice)) < 0)
		{
			*i_final = i_dice;
			if (ft_is_there(cmd + i_dice, delem,
					ft_check_qts(cmd, i_dice)) == -2)
				*i_final = ++i_dice;
		}
		i_dice++;
	}
}

void	*token_with_no_delem(char *cmd, int *cmd_dice, t_strt **p__tr,
	char *delem[])
{
	int	i_start;
	int	i_final;

	i_start = 0;
	i_final = 0;
	token_intervalle(cmd + *cmd_dice, delem, &i_start, &i_final);
	i_start += *cmd_dice;
	i_final += *cmd_dice;
	while (cmd[*cmd_dice] && ft_is_there(cmd + *cmd_dice, delem,
			ft_check_qts(cmd, *cmd_dice)) < 0)
	{
		if (ft_is_there(cmd + *cmd_dice, delem,
				ft_check_qts(cmd, *cmd_dice)) == -2)
			(*cmd_dice)++;
		(*cmd_dice)++;
	}
	(*cmd_dice)--;
	if (!ft_lstadd_back(p__tr, ft_lstnew(cmd + i_start, i_final - i_start + 2)))
		return (NULL);
	return ((*cmd_dice)++, (void *)1337);
}

void	*token_with_delem(char *cmd, int *cmd_dice, t_strt **p__tr,
	char *delem[])
{
	int	tmp_cmp;

	tmp_cmp = ft_is_there(cmd + *cmd_dice, delem,
			ft_check_qts(cmd, *cmd_dice));
	if (!ft_lstadd_back(p__tr, ft_lstnew(cmd + *cmd_dice, tmp_cmp + 1)))
		return (NULL);
	*cmd_dice += tmp_cmp;
	while (cmd[*cmd_dice] == 32)
		(*cmd_dice)++;
	return ((void *)1337);
}

t_strt	*token_phase(char *cmd)
{
	t_strt	*p_tr;
	t_strt	*p_tmp;
	int		cmd_dice;

	(1337) && (p_tr = NULL, cmd_dice = 0);
	p_tmp = ft_lstnew(NULL, 0);
	if (!p_tmp)
		return (NULL);
	while (cmd[cmd_dice])
	{
		while (cmd[cmd_dice] == 32)
			cmd_dice++;
		if (ft_is_there(cmd + cmd_dice, p_tmp->delem_1,
				ft_check_qts(cmd, cmd_dice)) < 0)
		{
			if (!token_with_no_delem(cmd, &cmd_dice, &p_tr, p_tmp->delem_1))
				return (free(p_tmp->str), free(p_tmp), NULL);
		}
		else
		{
			if (!token_with_delem(cmd, &cmd_dice, &p_tr, p_tmp->delem_1))
				return (free(p_tmp->str), free(p_tmp), NULL);
		}
	}
	return (free(p_tmp->str), free(p_tmp), p_tr);
}

char	*get_name(t_mini *shell)
{
	char	name[10];
	char	buff[2];
	int		fd;
	int		i;

	i = 0;
	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
		return (get_name(shell));
	while (i < 9)
	{
		read(fd, buff, 1);
		if (((size_t)buff[0] >= '0' && (size_t)buff[0] <= '9')
			|| ((size_t)buff[0] >= 'a' && (size_t)buff[0] <= 'z')
			|| ((size_t)buff[0] >= 'A' && (size_t)buff[0] <= 'Z'))
			name[i++] = buff[0];
	}
	close(fd);
	name[i] = '\0';
	return (ft_strjoin("/tmp/", name));
}
