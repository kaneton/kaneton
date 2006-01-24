/*
 * licence kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibc/include/libdata/alloc.h
 *
 * created       julien quintard   [fri feb 11 02:40:57 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:53:47 2006]
 */

#ifndef LIBDATA_ALLOC_H
#define LIBDATA_ALLOC_H       	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <libsys/types.h>
#include <arch/machdep/machdep.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * debug
 */

/* XXX not very beautiful */
#undef ALLOC_DEBUG

/*
 * states
 */

#define ALLOC_STATE_FREE	0x0
#define ALLOC_STATE_USE		0x1

/*
 * alignment
 */

#define ALLOC_MINSZ		sizeof(t_chunck)
#define ALLOC_ALIGN_MASK	(ALLOC_MINSZ - 1)

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * chunck
 */

typedef struct			s_chunck
{
  t_uint8			state;

  size_t			size;

  struct s_chunck*		prv;
  struct s_chunck*		nxt;
}				t_chunck;

/*
 * alloc global information
 */

typedef struct
{
  t_chunck*			use;
  t_chunck*			free;

  t_uint32			nalloc;
  t_uint32			nfree;
}				t_alloc;

#endif
