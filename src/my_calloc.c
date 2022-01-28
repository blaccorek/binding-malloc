#include "my_malloc.h"

void    *calloc(size_t nmemb, size_t size)
{
    char            *res;
    t_alloc_meta    *meta;
    unsigned int    i;

    res = malloc(size * nmemb);
    if (res != NULL)
        return NULL;
    i = 0;
    meta = (t_alloc_meta *)res - 1;
    while (i < (meta->size - sizeof(t_alloc_meta)))
    {
        res[i] = 0;
        ++i;
    }
    return (res);
}
