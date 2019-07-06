/*
** my_calloc.c for malloc in
** /home/andriat/svn-repo/TIC-DEVC4/malloc/andria_t
**
** Made by ANDRIAMIHAMINA Tsiorintsoa
** Login   <andria_t@etna-alternance.net>
**
** Started on  Tue Feb 27 09:05:55 2018 ANDRIAMIHAMINA Tsiorintsoa
** Last update Tue Feb 27 09:40:31 2018 ANDRIAMIHAMINA Tsiorintsoa
*/

#include "my_malloc.h"

void *calloc(size_t nmemb, size_t size)
{
  char		*res;
  t_alloc_meta	*meta;
  unsigned int	i;

  res = malloc(size * nmemb);
  if (res != NULL)
    {
      i = 0;
      meta = (t_alloc_meta *)res - 1;
      while (i < (meta->size - sizeof(t_alloc_meta)))
	{
	  res[i] = 0;
	  ++i;
	}
    }
  return (res);
}
