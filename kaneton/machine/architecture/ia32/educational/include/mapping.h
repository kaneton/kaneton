/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...cture/ia32/educational/include/mapping.h
 *
 * created       matthieu bucchianeri   [mon dec 24 19:28:30 2007]
 * updated       julien quintard   [mon nov 22 11:26:57 2010]
 */

#ifndef ARCHITECTURE_MAPPING_H
#define ARCHITECTURE_MAPPING_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../mapping.c
 */

/*
 * ../mapping.c
 */

t_error			ia32_map_chunk(t_vaddr		v,
				       t_paddr		p,
				       void*		alloc);

t_error			ia32_unmap_chunk(t_vaddr	v);

t_error			ia32_map_pd(t_ia32_directory*	pd);

t_error			ia32_map_pt(t_ia32_table*	pt);

t_error			ia32_map_region(i_as		asid,
					i_segment	segid,
					t_paddr		offset,
					t_options	options,
					t_vaddr		address,
					t_vsize		size);

t_error			ia32_unmap_region(i_as		asid,
					  t_vaddr	address,
					  t_vsize	size);


/*
 * eop
 */

#endif
