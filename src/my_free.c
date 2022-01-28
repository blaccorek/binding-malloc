#include "my_free.h"

static void print_error(const char *message)
{
    write(2, message, strlen(message));
}

int is_valid(t_alloc_meta *meta)
{
    if (meta != NULL && (size_t)meta >= (size_t)(FIRST_PTR))
    {
        if (meta->magic != A_MAGIC)
            print_error("free : invalid pointer\n");
        else if ((meta->size % 16) != 0)
            print_error("free : invalid next pointer\n");
        else
            return (meta->magic == A_MAGIC && (meta->size % 16) == 0);
    }
    print_error("free : invalid chunk pointer\n");
    return (0);
}

void    free(void *addr)
{
    int             index;
    t_alloc_meta    *meta;

    meta = NULL;
    if (addr == NULL)
        return;
    meta = (t_alloc_meta *)addr - 1;
    pthread_mutex_lock(&g_protector);

    if (MEM_INFO != NULL && is_valid(meta))
    {
        index = INDEX_FOR(meta->size);

        if (index == -1)
            munmap(meta, meta->size);
        else
            add_block(index, meta);
    }
    pthread_mutex_unlock(&g_protector);
}
