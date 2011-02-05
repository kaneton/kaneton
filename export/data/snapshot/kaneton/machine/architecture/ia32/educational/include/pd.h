/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...chitecture/ia32/educational/include/pd.h
 *
 * created       julien quintard   [mon jan 10 09:05:19 2011]
 * updated       julien quintard   [sat feb  5 14:59:37 2011]
 */

#ifndef ARCHITECTURE_PD_H
#define ARCHITECTURE_PD_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * defines the number entries composing a page directory.
 */

#define ARCHITECTURE_PD_SIZE		1024

/*
 * this value defines the page directory entry which acts as the mirror
 * entry i.e the entry referencing the page directory itself.
 */

#define ARCHITECTURE_PD_MIRROR		1023

/*
 * this flag indicates that the page table referenced by the page directory
 * entry is present.
 */

#define ARCHITECTURE_PDE_PRESENT	(1 << 0)

/*
 * ---------- macro-functions -------------------------------------------------
 */

/*
 * return the target address of a page directory entry.
 */

#define ARCHITECTURE_PDE_ADDRESS(_pde_)					\
  ((_pde_) & 0xfffff000)

/*
 * this macro-function computes the page directory entry's index associated
 * with the given virtual address.
 */

#define ARCHITECTURE_PD_INDEX(_vaddr_)					\
  (((_vaddr_) >> 22) & 0x3ff)

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <architecture/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * this type represents a 4-byte page directory entry.
 */

typedef t_uint32		at_pde;

/*
 * this type represents a page directory i.e a sequence of 1024 page
 * directory entries.
 */

typedef at_pde*			at_pd;

/*
 * this type represents the index of a page directory entry.
 */

typedef t_uint32		at_pdei;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../pd.c
 */

/*
 * ../pd.c
 */

t_error			architecture_pd_build(at_pd		pd);

t_error			architecture_pd_insert(at_pd		pd,
					       at_pdei		index,
					       t_paddr		address,
					       t_flags		flags);

t_error			architecture_pd_delete(at_pd		pd,
					       at_pdei		index);

t_error			architecture_pd_map(t_paddr		paddr,
					    at_pd*		table);

t_error			architecture_pd_unmap(at_pd		table);


/*
 * eop
 */

#endif
