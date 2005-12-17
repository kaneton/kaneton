/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * paging.h
 * 
 * path          /home/mycure/kaneton/core/include/arch/ia32/ia32
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sun May 29 00:39:42 2005   mycure
 * last update   Sun Jun 19 20:37:38 2005   mycure
 */

#ifndef IA32_IA32_PAGING_H
#define IA32_IA32_PAGING_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/ia32/types.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

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

typedef t_uint32		t_pde;
typedef t_uint32		t_pte;

#endif
