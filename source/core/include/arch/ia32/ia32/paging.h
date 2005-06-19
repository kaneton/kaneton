/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * paging.h
 * 
 * path          /home/mycure/kaneton/core/include/arch/ia32
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sun May 29 00:39:42 2005   mycure
 * last update   Sun Jun 19 14:30:04 2005   mycure
 */

#ifndef IA32_IA32_PAGING_H
#define IA32_IA32_PAGING_H	1

/*
 * dependencies
 */

#include <arch/ia32/ia32/types.h>

/*
 * defines
 */

#define PAGING_PD		0x401000	/* kernel page table */
#define PAGING_PT0		0x402000	/* page table 0 */
#define PAGING_PT4		0x406000	/* page table 4 */

#define PAGING_NPDE		1024
#define PAGING_NPTE		1024

#define PAGING_PD_SHIFT		22
#define PAGING_PD_MASK		0x000003ff

#define PAGING_PT_SHIFT		12
#define PAGING_PT_MASK		0x000003ff

#define PAGING_ADDRESS		0xfffff000
#define PAGING_FLAGS		0x00000fff

/*
 * macros
 */

#define PAGING_PDE(_vaddr_)						\
  ((_vaddr_ >> PAGING_PD_SHIFT) & PAGING_PD_MASK)

#define PAGING_PTE(_vaddr_)						\
  ((_vaddr_ >> PAGING_PT_SHIFT) & PAGING_PT_MASK)

/*
 * paging flags
 */

#define PAGING_GLOBAL		0x100	/* global */
#define PAGING_PS		0x080	/* page size */
#define PAGING_DIRTY		0x040	/* dirty */
#define PAGING_ACCESSED		0x020	/* accessed */
#define PAGING_PCD		0x010	/* page-level cache disable */
#define PAGING_PWT		0x008	/* page-level write through */
#define PAGING_U		0x004	/* user */
#define PAGING_S		0x000	/* supervisor */
#define PAGING_RW		0x002	/* read/write */
#define PAGING_RO		0x000	/* read only */
#define PAGING_P		0x001	/* present */

/*
 * types
 */

typedef t_uint32		t_pde;
typedef t_uint32		t_pte;

#endif
