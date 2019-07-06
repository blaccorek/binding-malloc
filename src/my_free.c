/*
** my_free.c for malloc in /home/andriat/svn-repo/TIC-DEVC4/malloc/andria_t/src
**
** Made by ANDRIAMIHAMINA Tsiorintsoa
** Login   <andria_t@etna-alternance.net>
**
** Started on  Mon Jan 15 11:13:56 2018 ANDRIAMIHAMINA Tsiorintsoa
** Last update Thu Mar  1 10:44:39 2018 ANDRIAMIHAMINA Tsiorintsoa
*/

#include "my_free.h"

static int	len(const char *str)
{
  int		i;

  i = -1;
  while (str[++i] != 0);
  return (i);
}

int	is_valid(t_alloc_meta *meta)
{
  if (meta != NULL && (size_t)meta >= (size_t)(FIRST_PTR))
    {
      if (meta->magic != A_MAGIC)
	{
	  write(2, "free : invalid pointer\n",
		len("free : invalid pointer\n"));
	}
      else if ((meta->size % 16) != 0)
	{
	  write(2, "free : invalid next pointer\n",
		len("free : invalid next pointer\n"));
	}
      return (meta->magic == A_MAGIC && (meta->size % 16) == 0);
    }
    write(2, "free : invalid chunk pointer\n",
	  len("free : invalid chunk pointer\n"));
  return (0);
}

void		free(void* addr)
{
  int		index;
  t_alloc_meta	*meta;

  meta = NULL;
  if (addr)
    {
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
}
