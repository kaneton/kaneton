/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...hitecture/ia32/generic/include/segment.h
 *
 * created       matthieu bucchianeri   [mon dec 24 19:27:38 2007]
 * updated       matthieu bucchianeri   [mon dec 24 19:28:20 2007]
 */

#ifndef ARCHITECTURE_SEGMENT_H
#define ARCHITECTURE_SEGMENT_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../segment.c
 */

/*
 * ../segment.c
 */

t_status		ia32_segment_read(i_region		segid,
					  t_paddr		offs,
					  void*			buff,
					  t_psize		sz);

t_status		ia32_segment_write(i_region		segid,
					   t_paddr		offs,
					   const void*		buff,
					   t_psize		sz);

t_status		ia32_segment_copy(i_region		dst,
					  t_paddr		offsd,
					  i_region		src,
					  t_paddr		offss,
					  t_psize		sz);


/*
 * eop
 */

#endif
