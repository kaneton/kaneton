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
 * last update   Sun May 29 13:04:10 2005   mycure
 */

#ifndef IA32_PAGING_H
#define IA32_PAGING_H		1

/*
 * defines
 */

#define PAGING_PD		0x11000 /* kernel page table */

#define PAGING_PT_IDENTITY	0x12000	/* identity mapping page table */

#define PAGING_NPDE		1024
#define PAGING_NPTE		1024

#define PAGING_PD_SHIFT		22
#define PAGING_PD_MASK		0x000003ff

#define PAGING_PT_SHIFT		12
#define PAGING_PT_MASK		0x000003ff

#define PAGING_PAGE_MASK	0xfffff000
#define PAGING_OFFSET_MASK	0x00000fff

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

#define PAGING_GLOBAL		0x100
#define PAGING_4M		0x080
#define PAGING_DIRTY		0x040
#define PAGING_ACCESSED		0x020
#define PAGING_PCD		0x010
#define PAGING_PWT		0x008
#define PAGING_U		0x004	/* user */
#define PAGING_S		0x000	/* supervisor */
#define PAGING_RW		0x002
#define PAGING_RO		0x000
#define PAGING_P		0x001	/* present */

/*
 * types
 */

typedef struct			s_pte
{
  t_uint32			flags : 12;
  t_uint32			address;
}				__PACKED__ t_pte;

typedef struct			s_pde
{
  t_uint32			flags : 12;
  t_uint32			address;
}				__PACKED__ t_pde;

typedef t_pte*			t_pt;
typedef t_pde*			t_pd;

#endif
