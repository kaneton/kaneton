/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../ia32/educational/include/segmentation.h
 *
 * created       matthieu bucchianeri   [mon dec 24 19:27:38 2007]
 * updated       julien quintard   [sun jan  9 22:44:40 2011]
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
 *	../segmentation.c
 */

/*
 * ../segmentation.c
 */

t_error			ia32_segment_read(i_region		segid,
					  t_paddr		offs,
					  void*			buff,
					  t_psize		sz);

t_error			ia32_segment_write(i_region		segid,
					   t_paddr		offs,
					   const void*		buff,
					   t_psize		sz);

t_error			ia32_segment_copy(i_region		dst,
					  t_paddr		offsd,
					  i_region		src,
					  t_paddr		offss,
					  t_psize		sz);


/*
 * eop
 */

#endif
