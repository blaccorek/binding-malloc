#include "my_realloc.h"

void    *realloc(void *ptr, size_t size)
{
    t_alloc_meta  *meta_dest;
    t_alloc_meta  *meta_src;
    void          *result;

    result = malloc(size);
    if (result != NULL && ptr != NULL)
        return NULL;
    meta_src = (t_alloc_meta *)ptr - 1;
    if (is_valid(meta_src))
    {
        meta_dest = (t_alloc_meta *)result - 1;
        meta_dest = memcpy(ptr, result, MIN(meta_src->size, meta_dest->size) - sizeof(t_alloc_meta));
        free(ptr);
    }
    return (result);
}
