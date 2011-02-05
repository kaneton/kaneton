/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...chitecture/ia32/educational/include/pt.h
 *
 * created       julien quintard   [mon jan 10 09:31:37 2011]
 * updated       julien quintard   [sat feb  5 15:00:03 2011]
 */

#ifndef ARCHITECTURE_PT_H
#define ARCHITECTURE_PT_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * defines the number entries composing a page table.
 */

#define ARCHITECTURE_PT_SIZE		1024

/*
 * this value defines the page table entry---within the mirror page directory
 * entry---which acts as the mirror entry i.e the page table entry referencing
 * the page directory itself.
 *
 * this value must be coupled with ARCHITECTURE_PD_MIRROR in order to
 * access to the page directory through the mirroring.
 */

#define ARCHITECTURE_PT_MIRROR		1023

/*
 * this is an empty flag.
 */

#define ARCHITECTURE_PTE_NONE		0x0

/*
 * this flag indicates that the page referenced by the page table
 * entry is present.
 */

#define ARCHITECTURE_PTE_PRESENT	(1 << 0)

/*
 * ---------- macro-functions -------------------------------------------------
 */

/*
 * return the target address of a page directory entry.
 */

#define ARCHITECTURE_PTE_ADDRESS(_pte_)					\
  ((_pte_) & 0xfffff000)

/*
 * computes the page table entry's index associated with the given
 * virtual address.
 */
#define ARCHITECTURE_PT_INDEX(_vaddr_)					\
  (((_vaddr_) >> 12) & 0x3ff)

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <architecture/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * this type represents a 4-byte page table entry.
 */

typedef t_uint32		at_pte;

/*
 * this type represents a page table i.e a sequence of 1024 page
 * table entries.
 */

typedef at_pte*			at_pt;

/*
 * this type represents the index of a page table entry.
 */

typedef t_uint32		at_ptei;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../pt.c
 */

/*
 * ../pt.c
 */

t_error			architecture_pt_build(at_pt		pt);

t_error			architecture_pt_insert(at_pt		pt,
					       at_ptei		index,
					       t_paddr		address,
					       t_flags		flags);

t_error			architecture_pt_delete(at_pt		pt,
					       at_ptei		index);

t_error			architecture_pt_map(t_paddr		paddr,
					    at_pt*		table);

t_error			architecture_pt_unmap(at_pt		table);


/*
 * eop
 */

#endif
