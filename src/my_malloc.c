/*
** my_malloc.c for malloc in /home/andriat/svn-repo/TIC-DEVC4/malloc/andria_t/src
**
** Made by ANDRIAMIHAMINA Tsiorintsoa
** Login   <andria_t@etna-alternance.net>
**
** Started on  Sun Jan 14 21:02:44 2018 ANDRIAMIHAMINA Tsiorintsoa
** Last update Thu Mar  1 10:35:01 2018 ANDRIAMIHAMINA Tsiorintsoa
*/

#include "my_malloc.h"

t_mem_info	*g_mem_info = (void *)-1;
pthread_mutex_t	g_protector = PTHREAD_MUTEX_INITIALIZER;

static void	*alloc_big_size(size_t size)
{
  t_alloc_meta	*meta;

  meta = mmap(NULL, PAGE_FOR(size), PROT_READ|PROT_WRITE,
	      MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
  if (meta != (void *)-1)
    {
      meta->magic = A_MAGIC;
      meta->size = size;
      return (meta + 1);
    }
  return (NULL);
}

static void	*increase_free_space(size_t size)
{
  t_alloc_meta	*meta;
  t_free_block	*block;

  meta = sbrk(INIT_BLOCK);
  if (meta != (void *)-1)
    {
      meta->size = INIT_BLOCK;
      block = add_block(INDEX_FOR(meta->size - 1), meta);
      if (block != NULL)
	return (split_free_block(block, size));
    }
  return (NULL);
}

static void	*search_free_node(size_t size)
{
  int		index;
  t_free_block	*block;

  index = INDEX_FOR(size);
  if (index < 0)
    return (alloc_big_size(size));
  if (index > MAX_INDEX)
    return (increase_free_space(size));
  if (index < MIN_INDEX)
    index = MIN_INDEX;
  block = SIZETABLE[index];
  while (block == NULL || block->used == 0)
    block = SIZETABLE[++index];
  if (block != NULL && block->used > 0)
    return (split_free_block(block, size));
  else
    return (increase_free_space(size));
}

static int	init()
{
  unsigned int	i;
  t_alloc_meta	*meta;

  i = 0;
  MEM_INFO = sbrk(INIT_BLOCK);
  if (MEM_INFO != (void *)-1)
    {
      SIZETABLE = mmap(NULL, SIZETABLE_MAX * sizeof(void *),
		       PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1,
		       0);
      if (SIZETABLE != (void *)-1)
	{
	  meta = FIRST_PTR;
	  meta->magic = F_MAGIC;
	  meta->size = (size_t)sbrk(0) - (size_t)meta;
	  MAX_INDEX = INDEX_FOR(meta->size);
	  MIN_INDEX = MAX_INDEX;
	  while (i < SIZETABLE_MAX - 1)
	    SIZETABLE[i++] = NULL;
	  add_block(MAX_INDEX, meta);
	}
    }
  return (MEM_INFO != (void *)-1 && SIZETABLE != (void *)-1);
}

void		*malloc(size_t size)
{
  void		*free_node;
  static int	initialised = 0;

  free_node = NULL;
  pthread_mutex_lock(&g_protector);
  if (!initialised)
    initialised = init();
  if (initialised)
    {
      size = BLOCK_SIZE_FOR(REAL_SIZE(size));
      free_node = search_free_node(size);
    }
  pthread_mutex_unlock(&g_protector);
  return (free_node);
}
