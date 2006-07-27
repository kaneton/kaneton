/*
 * licence kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibc/include/libdata/alloc.h
 *
 * created       julien quintard   [fri feb 11 02:40:57 2005]
 * updated       matthieu bucchianeri   [wed jul 26 19:15:35 2006]
 */

#ifndef LIBDATA_ALLOC_H
#define LIBDATA_ALLOC_H       	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <libsys/types.h>
#include <arch/machdep/machdep.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

// XXX may be moved somewhere else

#define MAP_FAILED	((void*)-1)

#define PROT_READ	0x1		/* Page can be read.  */
#define PROT_WRITE	0x2		/* Page can be written.  */
#define PROT_EXEC	0x4		/* Page can be executed.  */
#define PROT_NONE	0x0		/* Page can not be accessed.  */

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * function pointers to mmap/munmap.
 */

typedef void* (*t_pfn_mmap)(void*, size_t, int, int, int, off_t);
typedef int (*t_pfn_munmap)(void*, size_t);

/*
 * chunk forward declaration.
 */

struct s_chunk;

/*
 * area
 */

typedef struct			s_area
{
  t_vsize			size;

  struct s_chunk*		first_free_chunk;

  struct s_area*		prev_area;
  struct s_area*		next_area;
}				t_area;

/*
 * chunk
 */

typedef struct			s_chunk
{
  t_vsize			size;

  struct s_chunk*		next_free;

  t_area*			area;
}				t_chunk;

/*
 * alloc global information
 */

typedef struct
{
  t_area*			areas;

  void*				lowest;
  void*				highest;

  t_pfn_mmap			mmap;
  t_pfn_munmap			munmap;

  t_uint32			nalloc;
  t_uint32			nfree;
}				t_alloc;

#endif
