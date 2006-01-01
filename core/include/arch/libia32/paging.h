/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/include/arch/libia32/paging.h
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [fri dec 30 19:35:45 2005]
 */

#ifndef IA32_IA32_PAGING_H
#define IA32_IA32_PAGING_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/libia32/types.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

/* old --------------------------------------------------------------------- */

/*
 * paging defines
 */

#define PAGING_NPDE		1024
#define PAGING_NPTE		1024

#define PAGING_PD_SHIFT		22
#define PAGING_PD_MASK		0x000003ff

#define PAGING_PT_SHIFT		12
#define PAGING_PT_MASK		0x000003ff

#define PAGING_ADDRESS		0xfffff000
#define PAGING_FLAGS		0x00000fff

/*
 * paging flags
 */

#define PAGING_GLOBAL		(1 << 8)	/* global */
#define PAGING_PS		(1 << 7)	/* page size */
#define PAGING_DIRTY		(1 << 6)	/* dirty */
#define PAGING_ACCESSED		(1 << 5)	/* accessed */
#define PAGING_PCD		(1 << 4)	/* page-level cache disable */
#define PAGING_PWT		(1 << 3)	/* page-level write through */
#define PAGING_U		(1 << 2)	/* user */
#define PAGING_S		(0 << 2)	/* supervisor */
#define PAGING_RW		(1 << 1)	/* read/write */
#define PAGING_RO		(0 << 1)	/* read only */
#define PAGING_P		(1 << 0)	/* present */

/*
 * ---------- macros ----------------------------------------------------------
 */

#define PAGING_PDE(_vaddr_)						\
  ((_vaddr_ >> PAGING_PD_SHIFT) & PAGING_PD_MASK)

#define PAGING_PTE(_vaddr_)						\
  ((_vaddr_ >> PAGING_PT_SHIFT) & PAGING_PT_MASK)

/*
 * ---------- types -----------------------------------------------------------
 */

typedef t_uint32		t_pdeold;
typedef t_uint32		t_pteold;

/* /old -------------------------------------------------------------------- */


/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * misc
 */

#define PD_MAX_ENTRIES		1024
#define PT_MAX_ENTRIES		1024

/*
 * page directory entry flags
 */

#define PDE_FLAG_A		(1 << 5)
#define PDE_FLAG_CD		(1 << 4)
#define PDE_FLAG_WT		(1 << 3)
#define PDE_FLAG_USER		(1 << 2)
#define PDE_FLAG_SUPERVISOR	(0 << 2)
#define PDE_FLAG_RO		(0 << 1)
#define PDE_FLAG_RW		(1 << 1)
#define PDE_FLAG_P		(1 << 0)

/*
 * page table entry flags
 */

#define PTE_FLAG_D		(1 << 6)
#define PTE_FLAG_A		(1 << 5)
#define PTE_FLAG_CD		(1 << 4)
#define PTE_FLAG_WT		(1 << 3)
#define PTE_FLAG_USER		(1 << 2)
#define PTE_FLAG_SUPERVISOR	(0 << 2)
#define PTE_FLAG_RO		(0 << 1)
#define PTE_FLAG_RW		(1 << 1)
#define PTE_FLAG_P		(1 << 0)

/*
 * ---------- macros ----------------------------------------------------------
 */

#define PDE_ENTRY(Paddr)						\
  (((Paddr) >> 22) & 0x3ff)

#define PTE_ENTRY(Paddr)						\
  (((Paddr) >> 12) & 0x3ff)

#define ENTRY_ADDR(D,T)							\
  ((void*)(((D) << 22) | ((T) << 12)))

#define MK_BASE(V)							\
  (((t_uint32)(V)) & 0xfffffc00)

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * abstract record for page
 */

typedef struct
{
  void*		addr;
  t_uint8	present;
  t_uint8	rw;
  t_uint8	user;
}		t_page;

/*
 * abstract record for table
 */

typedef struct
{
  void*		entries;
  t_uint8	present;
  t_uint8	rw;
  t_uint8	user;
}		t_table;

typedef t_uint32		t_pde;
typedef t_uint32		t_pte;

/*
 * abstract record for directory
 */

typedef t_pde* t_directory;

#endif
