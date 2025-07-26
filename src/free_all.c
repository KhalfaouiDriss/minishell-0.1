/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 20:44:59 by dkhalfao          #+#    #+#             */
/*   Updated: 2025/07/13 20:45:00 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_gc	*get_gc(void)
{
	static t_gc	gc;

	return (&gc);
}

void	gc_add_back(t_gc *gc, t_mlc *new)
{
	t_mlc	*tmp;

	if (!gc->head)
	{
		gc->head = new;
		return ;
	}
	tmp = gc->head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	*ft_malloc(size_t size)
{
	t_gc	*gc;
	void	*ptr;
	t_mlc	*new;

	gc = get_gc();
	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	new = malloc(sizeof(t_mlc));
	if (!new)
		return (NULL);
	new->ptr = ptr;
	new->next = NULL;
	gc_add_back(gc, new);
	return (ptr);
}

void	gc_free_all(void)
{
	t_gc	*gc;
	t_mlc	*tmp;

	gc = get_gc();
	while (gc->head)
	{
		tmp = gc->head;
		gc->head = gc->head->next;
		free(tmp->ptr);
		free(tmp);
	}
	gc->head = NULL;
}
