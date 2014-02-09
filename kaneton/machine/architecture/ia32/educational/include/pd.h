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
 * updated       julien quintard   [fri jan 14 21:09:58 2011]
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
 * this is an empty flag.
 */

#define ARCHITECTURE_PDE_NONE		0x0

/*
 * this flag indicates that the page table referenced by the page directory
 * entry is present.
 */

#define ARCHITECTURE_PDE_PRESENT	(1 << 0)

/*
 * these flags indicate whether the referenced 4MB-page is accessible in
 * read/write or read-only.
 */

#define ARCHITECTURE_PDE_RW		(1 << 1)
#define ARCHITECTURE_PDE_RO		(0 << 1)

/*
 * these flags indicate whether the 4MB-page CPL3 accesses are allowed
 * or not.
 */

#define ARCHITECTURE_PDE_USER		(1 << 2)
#define ARCHITECTURE_PDE_SUPERVISOR	(0 << 2)

/*
 * these flags activate either the PWT - Page-level Write Through caching
 * mechanism for the reference page table or the PWB - Page-level Write Back.
 */

#define ARCHITECTURE_PDE_PWT		(1 << 3)
#define ARCHITECTURE_PDE_PWB		(0 << 3)

/*
 * these flags disable or enable the cache by setting the PCD - Page-level
 * Cache Disable bit for the referenced page table.
 */

#define ARCHITECTURE_PDE_PCD		(1 << 4)
#define ARCHITECTURE_PDE_PCE		(0 << 4)

/*
 * this flag is set by the microprocessor whenever this entry is accessed.
 */

#define ARCHITECTURE_PDE_ACCESSED	(1 << 5)

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

t_status		architecture_pd_dump(at_pd		pd);

t_status		architecture_pd_build(at_pd		pd);

t_status		architecture_pd_insert(at_pd		pd,
					       at_pdei		index,
					       t_paddr		address,
					       t_flags		flags);

t_status		architecture_pd_delete(at_pd		pd,
					       at_pdei		index);

t_status		architecture_pd_map(t_paddr		paddr,
					    at_pd*		table);

t_status		architecture_pd_unmap(at_pd		table);


/*
 * eop
 */

#endif
