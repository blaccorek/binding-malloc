/*
** my_realloc.c for malloc in
** /home/andriat/svn-repo/TIC-DEVC4/malloc/andria_t
**
** Made by ANDRIAMIHAMINA Tsiorintsoa
** Login   <andria_t@etna-alternance.net>
**
** Started on  Fri Feb 23 07:34:31 2018 ANDRIAMIHAMINA Tsiorintsoa
** Last update Sat Feb 24 11:41:30 2018 ANDRIAMIHAMINA Tsiorintsoa
*/

#include "my_realloc.h"

static void	m_cpy(char *src, char *dest, size_t size)
{
  unsigned int	i;

  i = 0;
  while (i < size)
    {
      dest[i] = src[i];
      ++i;
    }
}

void		*realloc(void * ptr, size_t size)
{
  void		*result;
  t_alloc_meta	*meta_src;
  t_alloc_meta	*meta_dest;

  result = malloc(size);
  if (result != NULL)
    {
      if (ptr != NULL)
	{
	  meta_src = (t_alloc_meta *)ptr - 1;
	  if (is_valid(meta_src))
	    {
	      meta_dest = (t_alloc_meta *)result - 1;
	      m_cpy(ptr, result, MIN(meta_src->size, meta_dest->size)
		    - sizeof(t_alloc_meta));
	      free(ptr);
	    }
	}
    }
  return (result);
}
