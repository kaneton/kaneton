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
 * last update   Sun May 29 12:44:17 2005   mycure
 */

#ifndef IA32_PAGING_H
#define IA32_PAGING_H		1

/*
 * macros
 */

XXX

/*
 * defines
 */

#define PAGING_PD		0x11000 /* kernel page table */

#define PAGING_PT_IDENTITY	0x12000	/* identity mapping page table */

#define PAGING_NPDE		1024
#define PAGING_NPTE		1024

/*
 * paging flags
 */

#define PAGING_GLOBAL		0x100
#define PAGING_4M_PAGESZ	0x080
#define PAGING_DIRTY		0x040
#define PAGING_ACCESSED		0x020
#define PAGING_PCD		0x010
#define PAGING_PWT		0x008
#define PAGING_USER		0x004
#define PAGING_SUPERVISOR	0x000
#define PAGING_READ_WRITE	0x002
#define PAGING_READ_ONLY	0x000
#define PAGING_PRESENT		0x001

/* XXX
#define PD_MASK0xffc00000
#define PD_SHIFT22

#define PT_MASK0x003ff000
#define PT_SHIFT12

#define OFFSET_MASK0x00000fff

#define PAGE_MASK0xfffff000
*/

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
