/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutaik <rboutaik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 16:11:21 by aachalla          #+#    #+#             */
/*   Updated: 2024/06/02 19:31:30 by rboutaik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	lstnew_init(t_strt **t)
{
	(*t)->command = NULL;
	(*t)->type = 0;
	(*t)->option = NULL;
	(*t)->redirect = NULL;
	(*t)->here_doc = NULL;
	(*t)->here_doc_fd = -1;
	(*t)->delemiter = NULL;
	(*t)->append = NULL;
	(*t)->in_file = NULL;
	(*t)->ou_file = NULL;
	(*t)->next = NULL;
	(*t)->prev = NULL;
	(*t)->pr_exec = NULL;
	(*t)->pr_exec_size = 0;
	(*t)->cmd_flag = 0;
	(*t)->opt_flag = 0;
	(*t)->exit_cmd_flag = 0;
	(*t)->ofile_flag = NULL;
	(*t)->ofile_flag_size = 0;
	(*t)->ifile_flag = NULL;
	(*t)->ifile_flag_size = 0;
	(*t)->var_qts = NULL;
	(1337) && ((*t)->var_qts_size = 0, (*t)->var_qts_dejavu = 0);
	(*t)->chil_cmd = NULL;
	(*t)->expandable = 1;
}

t_strt	*ft_lstnew(char *content, int size)
{
	t_strt	*t;

	t = malloc(sizeof(t_strt));
	if (!t)
		return (perror("minishell"), NULL);
	t->str = malloc(size + 1);
	if (!t->str)
		return (free(t), perror("minishell"), NULL);
	ft_strncpy(t->str, content, size - 1);
	t->delem_1[0] = "(";
	t->delem_1[1] = ")";
	t->delem_1[2] = "<<";
	t->delem_1[3] = ">>";
	t->delem_1[4] = "||";
	t->delem_1[5] = "|";
	t->delem_1[6] = "&&";
	t->delem_2[0] = " ";
	t->delem_2[1] = "<<";
	t->delem_2[2] = ">>";
	t->delem_2[3] = ">";
	t->delem_2[4] = "<";
	t->delem_2[5] = "\"";
	t->delem_2[6] = "'";
	lstnew_init(&t);
	return (t);
}

int	ft_lstadd_back(t_strt **lst, t_strt *new)
{
	t_strt	*t;

	if (!lst || !new)
		return (0);
	t = *lst;
	if (!*lst)
		*lst = new;
	else
	{
		while (t->next)
			t = t->next;
		t->next = new;
		new->prev = t;
	}
	return (1337);
}

int	ft_is_there(char *cmd, char *delem[], int btw_quotes)
{
	int	i_dice;
	int	tmp_cmp;

	i_dice = -1;
	while (++i_dice < 7)
	{
		tmp_cmp = ft_strcmp_delm(cmd, delem[i_dice]);
		if (tmp_cmp == 2 && (i_dice == 2 || i_dice == 3) && !btw_quotes)
			return (-2);
		if (tmp_cmp > 0 && !btw_quotes)
			return (tmp_cmp);
	}
	return (-1);
}

int	ft_is_delm(char *str, char *c[])
{
	int	i_dice;
	int	tmp_cmp;

	i_dice = -1;
	while (++i_dice < 7)
	{
		tmp_cmp = ft_strcmp_delm(str, c[i_dice]);
		if (tmp_cmp > 0)
			return (tmp_cmp);
	}
	return (-1);
}
