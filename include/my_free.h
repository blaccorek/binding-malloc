/*
** my_free.h for malloc in /home/andriat/svn-repo/TIC-DEVC4/malloc/andria_t
**
** Made by ANDRIAMIHAMINA Tsiorintsoa
** Login   <andria_t@etna-alternance.net>
**
** Started on  Mon Jan 15 10:01:17 2018 ANDRIAMIHAMINA Tsiorintsoa
** Last update Fri Feb 23 07:57:36 2018 ANDRIAMIHAMINA Tsiorintsoa
*/

#ifndef MY_FREE_H_
# define MY_FREE_H_

# include "my_malloc.h"

int	is_valid(t_alloc_meta *meta);
void	free(void *ptr);
void	my_free(void *ptr);

#endif /* !MY_FREE_H_ */
