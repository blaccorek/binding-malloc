/*
** my_realloc.h for malloc in
** /home/andriat/svn-repo/TIC-DEVC4/malloc/andria_t
**
** Made by ANDRIAMIHAMINA Tsiorintsoa
** Login   <andria_t@etna-alternance.net>
**
** Started on  Fri Feb 23 07:52:01 2018 ANDRIAMIHAMINA Tsiorintsoa
** Last update Fri Feb 23 07:57:58 2018 ANDRIAMIHAMINA Tsiorintsoa
*/

#ifndef MY_REALLOC_H_
# define MY_REALLOC_H_

# include "my_free.h"

# define	MIN(s1, s2)	((s1) < (s2) ? (s1) : (s2))

void	*realloc(void *, size_t);

#endif /* !MY_REALLOC_H_ */
