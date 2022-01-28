#ifndef MY_REALLOC_H_
#define MY_REALLOC_H_

#include "my_free.h"

#define MIN(s1, s2) ((s1) < (s2) ? (s1) : (s2))

void *realloc(void *, size_t);

#endif /* !MY_REALLOC_H_ */
