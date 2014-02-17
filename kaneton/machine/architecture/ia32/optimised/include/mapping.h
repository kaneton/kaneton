/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...hitecture/ia32/generic/include/mapping.h
 *
 * created       matthieu bucchianeri   [mon dec 24 19:28:30 2007]
 * updated       matthieu bucchianeri   [mon dec 24 19:28:47 2007]
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

t_status		ia32_map_chunk(t_vaddr		v,
				       t_paddr		p,
				       void*		alloc);

t_status		ia32_unmap_chunk(t_vaddr	v);

t_status		ia32_map_pd(t_ia32_directory*	pd);

t_status		ia32_map_pt(t_ia32_table*	pt);

t_status		ia32_map_region(i_as		asid,
					i_segment	segid,
					t_paddr		offset,
					t_opts		opts,
					t_vaddr		address,
					t_vsize		size);

t_status		ia32_unmap_region(i_as		asid,
					  t_vaddr	address,
					  t_vsize	size);


/*
 * eop
 */

#endif
