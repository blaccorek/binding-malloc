/*
** my_malloc.h for malloc in /home/andriat/svn-repo/TIC-DEVC4/malloc/andria_t/include
**
** Made by ANDRIAMIHAMINA Tsiorintsoa
** Login   <andria_t@etna-alternance.net>
**
** Started on  Sun Jan 14 21:06:05 2018 ANDRIAMIHAMINA Tsiorintsoa
** Last update Thu Mar  1 09:59:40 2018 ANDRIAMIHAMINA Tsiorintsoa
*/

#ifndef		MY_MALLOC_H_
# define	MY_MALLOC_H_

# include <pthread.h>
# include <stddef.h>
# include <sys/mman.h>
# include <unistd.h>

# define A_MAGIC	0xfeedbeef
# define F_MAGIC	0xdeadbeef

# define PAGE_SIZE	4096

# define INIT_BLOCK	(32 * PAGE_SIZE)
# define BLOCK_SIZE	16

/*
** ACCESS MEMORY INFO
*/
# define MEM_INFO	g_mem_info
# define SIZETABLE	g_mem_info->size_table
# define SIZETABLE_MAX	(INIT_BLOCK / BLOCK_SIZE)

# define FIRST_PTR	((void *)g_mem_info + BLOCK_SIZE_FOR(sizeof(t_mem_info)))
# define LAST_PTR	sbrk(0)

# define MIN_INDEX	g_mem_info->min_index
# define MAX_INDEX	g_mem_info->max_index

/*
** Macro functions
*/
# define REAL_SIZE(size)	(size + sizeof(t_alloc_meta))

# define BLOCK_SIZE_FOR(size)	((((size) / BLOCK_SIZE) + 1) * BLOCK_SIZE)

# define PAGE_FOR(size)		((((size) / PAGE_SIZE) + 1) * PAGE_SIZE)

# define INDEX_FOR(size)	((size) < INIT_BLOCK ? (int)((size) / BLOCK_SIZE) : -1)

typedef struct		s_alloc_meta
{
  unsigned int		magic;
  unsigned int		size;
}			t_alloc_meta;

typedef struct		s_free_block
{
  unsigned int		max;
  unsigned int		used;
  t_alloc_meta		*meta[];
}			t_free_block;

typedef struct	s_mem_info
{
  int		min_index;
  int		max_index;
  t_free_block	**size_table;
}		t_mem_info;

extern t_mem_info	*g_mem_info;
extern pthread_mutex_t	g_protector;

void		*malloc(size_t);
void		*my_malloc(size_t);
t_free_block	*add_block(int index, t_alloc_meta *meta);
void		*split_free_block(t_free_block *block, size_t size);

#endif		/* !MY_MALLOC_H_ */
