/*
 * licence kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibc/include/libdata/alloc.h
 *
 * created       julien quintard   [fri feb 11 02:40:57 2005]
 * updated       matthieu bucchianeri   [thu aug 17 14:38:03 2006]
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
 * function pointers to map_reserve/map_release
 */

typedef t_error (*t_pfn_map_reserve)(i_as, t_opts, t_vsize, t_perms, t_vaddr*);
typedef t_error (*t_pfn_map_release)(i_as, t_vaddr);

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

  t_uint32			signature;

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

  t_uint32			signature;

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

  t_pfn_map_reserve		map_reserve;
  t_pfn_map_release		map_release;
  i_as				asid;

  t_vaddr			reserve;

  t_uint32			nalloc;
  t_uint32			nfree;
}				t_alloc;

#endif
