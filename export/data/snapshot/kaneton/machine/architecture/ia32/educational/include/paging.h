/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ecture/ia32/educational/include/paging.h
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       julien quintard   [mon apr 11 13:48:17 2011]
 */

#ifndef ARCHITECTURE_PAGING_H
#define ARCHITECTURE_PAGING_H	1

/*
 * ---------- macro-functions -------------------------------------------------
 */

/*
 * computes a virtual address according to the given directory and table
 * entries.
 */

#define ARCHITECTURE_PAGING_ADDRESS(_pdei_, _ptei_)			\
  (t_vaddr)(((_pdei_) << 22) | ((_ptei_) << 12))

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>

#include <architecture/register.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../paging.c
 */

/*
 * ../paging.c
 */

t_status			architecture_paging_pdbr(t_paddr	pd,
						 t_flags	flags,
						 at_cr3*	pdbr);

t_status			architecture_paging_map(i_as		id,
						i_segment	segment,
						t_paddr		offset,
						t_options	options,
						t_vaddr		address,
						t_vsize		size);

t_status			architecture_paging_unmap(i_as		id,
						  t_vaddr	address,
						  t_vsize	size);

t_status			architecture_paging_read(i_segment	id,
						 t_paddr	offset,
						 void*		buffer,
						 t_psize	size);

t_status			architecture_paging_write(i_segment	id,
						  t_paddr	offset,
						  const void*	buffer,
						  t_psize	size);

t_status			architecture_paging_copy(i_region	dst,
						 t_paddr	to,
						 i_region	src,
						 t_paddr	from,
						 t_psize	size);


/*
 * eop
 */

#endif
