#include "../include/minishell.h"

t_gc *get_gc(void)
{
    static t_gc gc;
    return (&gc);
}

void *ft_malloc(size_t size)
{
    t_gc *gc = get_gc();
    void *ptr = malloc(size);
    if (!ptr)
        return (NULL);
    t_mlc *new = malloc(sizeof(t_mlc));
    if (!new)
        return (NULL);
    new->ptr = ptr;
    new->next = gc->head;
    gc->head = new;
    return (ptr);
}

void gc_free_all(void)
{
    t_gc *gc = get_gc();
    t_mlc *tmp;

    while (gc->head)
    {
        tmp = gc->head;
        // printf("gc : %s\n", (char *)tmp->ptr);
        gc->head = gc->head->next;
        // free(tmp->ptr);
        // free(tmp);
    }
}
