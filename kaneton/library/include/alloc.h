/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.STABLE/kaneton/library/include/alloc.h
 *
 * created       julien quintard   [sun jun 10 17:33:03 2007]
 * updated       julien quintard   [sun dec  5 15:55:46 2010]
 */

#ifndef LIBRARY_ALLOC_H
#define LIBRARY_ALLOC_H       	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <library/types.h>

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
