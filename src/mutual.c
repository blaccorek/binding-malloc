#include "my_malloc.h"

static int  init_table(int index)
{
    unsigned int    i;

    i = 0;
    SIZETABLE[index] = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE,
                            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (SIZETABLE[index] == (void *)-1)
        SIZETABLE[index] = NULL;
    else
    {
        SIZETABLE[index]->used = 0;
        SIZETABLE[index]->max = PAGE_SIZE / sizeof(void *) - 1;

        while (i < SIZETABLE[index]->max)
            SIZETABLE[index]->meta[i++] = NULL;
    }
    return (SIZETABLE[index] != NULL);
}

static int  increase_table(int index)
{
    t_free_block    *block;
    void            *ret;

    block = SIZETABLE[index];
    ret = mmap(&(block->meta[block->max]),
               PAGE_SIZE, PROT_READ | PROT_WRITE,
               MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ret != (void *)-1)
    {
        block->max += PAGE_SIZE / sizeof(void *);
        return (1);
    }
    return (0);
}

t_free_block    *add_block(int index, t_alloc_meta *meta)
{
    int             no_error;
    t_free_block    *block;

    no_error = 1;
    if (SIZETABLE[index] == NULL)
        no_error = init_table(index);
    else if (SIZETABLE[index]->used == (SIZETABLE[index]->max - 1))
        no_error = increase_table(index);

    if (no_error)
    {
        block = SIZETABLE[index];
        if (block->meta[block->used] == NULL)
        {
            block->meta[block->used] = meta;
            block->used++;
            meta->magic = F_MAGIC;

            if (index < MIN_INDEX)
                MIN_INDEX = index;

            if (index > MAX_INDEX)
                MAX_INDEX = index;

            return block;
        }
    }
    return (NULL);
}

static void adjust_indexes()
{
    int i;

    i = 0;
    MIN_INDEX = -1;
    MAX_INDEX = -1;
    while (i < SIZETABLE_MAX)
    {
        if (SIZETABLE[i] != NULL && SIZETABLE[i]->used > 0)
        {
            if (MIN_INDEX == -1)
                MIN_INDEX = i;

            if (i > MAX_INDEX)
                MAX_INDEX = i;
        }
        ++i;
    }
}

void    *split_free_block(t_free_block *block, size_t size)
{
    t_alloc_meta    *meta;
    t_alloc_meta    *meta_left;

    meta = block->meta[block->used - 1];
    meta_left = NULL;
    if (meta->magic == F_MAGIC && meta->size % BLOCK_SIZE == 0)
    {
        meta->magic = A_MAGIC;
        if (meta->size > size)
        {
            meta_left = (void *)meta + size;
            meta_left->size = meta->size - size;
            meta->size = size;
            add_block(INDEX_FOR(meta_left->size), meta_left);
        }
    }
    else
        meta = NULL;

    block->meta[block->used - 1] = NULL;
    block->used--;
    adjust_indexes();
    return (meta == NULL ? NULL : meta + 1);
}
