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
 * updated       julien quintard   [thu jan 13 10:30:55 2011]
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
 * this is an empty flag.
 */

#define ARCHITECTURE_PTE_NONE		0x0

/*
 * this flag indicates that the page referenced by the page table
 * entry is present.
 */

#define ARCHITECTURE_PTE_PRESENT	(1 << 0)

/*
 * these flags indicate whether the referenced page is accessible in
 * read/write or read-only.
 */

#define ARCHITECTURE_PTE_RW		(1 << 1)
#define ARCHITECTURE_PTE_RO		(0 << 1)

/*
 * these flags indicate whether the page CPL3 accesses are allowed
 * or not.
 */

#define ARCHITECTURE_PTE_USER		(1 << 2)
#define ARCHITECTURE_PTE_SUPERVISOR	(0 << 2)

/*
 * these flags activate either the PWT - Page-level Write Through caching
 * mechanism for the reference page table or the PWB - Page-level Write Back.
 */

#define ARCHITECTURE_PTE_PWT		(1 << 3)
#define ARCHITECTURE_PTE_PWB		(0 << 3)

/*
 * these flags disable or enable the cache by setting the PCD - Page-level
 * Cache Disable bit.
 */

#define ARCHITECTURE_PTE_PCD		(1 << 4)
#define ARCHITECTURE_PTE_PCE		(0 << 4)

/*
 * this flag is set by the microprocessor whenever this entry is accessed.
 */

#define ARCHITECTURE_PTE_ACCESSED	(1 << 5)

/*
 * this flag is set by the microprocessor whenever the referenced page
 * is modified.
 */

#define ARCHITECTURE_PTE_DIRTY		(1 << 6)

/*
 * this flag indicates that the page is 'global' i.e must not be cached.
 *
 * note that the global support must be activated through CR4.
 */

#define ARCHITECTURE_PTE_GLOBAL		(1 << 8)

/*
 * the following flag is not architecture-specific. instead, a software flag
 * has been created by using an architecture unused bit.
 */

#define ARCHITECTURE_PTE_USED		(1 << 9)

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
 * eop
 */

#endif
