/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/libc/include/alloc.h
 *
 * created       julien quintard   [sun jun 10 17:33:03 2007]
 * updated       matthieu bucchianeri   [sat sep  1 13:00:54 2007]
 */

#ifndef LIBC_LIBDATA_ALLOC_H
#define LIBC_LIBDATA_ALLOC_H       	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <libc/types.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * area block signature
 */

#define AREA_SIGNATURE		0xab121337

/*
 * chunk metadata signature
 */

#define CHUNK_SIGNATURE		0xBA153E01

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * chunk forward declaration.
 */

struct s_chunk;

/*
 * area
 */

typedef struct			s_area
{
  vsize_t			size;

#ifdef MALLOC_SIGN_ENABLE
  u_int32_t			signature;
#endif

  struct s_chunk*		first_free_chunk;

  struct s_area*		prev_area;
  struct s_area*		next_area;
}				t_area;

/*
 * chunk
 */

typedef struct			s_chunk
{
  vsize_t			size;

#ifdef MALLOC_SIGN_ENABLE
  u_int32_t			signature;
#endif

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

  vaddr_t			reserve;

  u_int32_t			nalloc;
  u_int32_t			nfree;
}				t_alloc;

#endif
