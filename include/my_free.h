#ifndef MY_FREE_H_
#define MY_FREE_H_

#include <string.h>

#include "my_malloc.h"

int     is_valid(t_alloc_meta *meta);
void    free(void *ptr);
void    my_free(void *ptr);

#endif /* !MY_FREE_H_ */
