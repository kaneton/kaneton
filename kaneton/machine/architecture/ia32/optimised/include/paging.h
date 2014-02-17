/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/include/paging/paging.h
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [tue mar 13 11:24:14 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * declares paging structures, macros, etc.
 *
 */

#ifndef ARCHITECTURE_PAGING_H
#define ARCHITECTURE_PAGING_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * misc
 */

#define IA32_PD_MAX_ENTRIES	1024
#define IA32_PT_MAX_ENTRIES	1024
#define IA32_PD_NOTCACHED	0
#define IA32_PD_CACHED		1
#define IA32_PD_WRITETHROUGH	0
#define IA32_PD_WRITEBACK	1
#define IA32_PD_CURRENT		NULL

#define IA32_PT_NOTCACHED	0
#define IA32_PT_CACHED		1
#define IA32_PT_WRITETHROUGH	0
#define IA32_PT_WRITEBACK	1
#define IA32_PT_READONLY	0
#define IA32_PT_WRITABLE	1
#define IA32_PT_PRIVILEGED	0
#define IA32_PT_USER		1

#define IA32_PG_NOTCACHED	0
#define IA32_PG_CACHED		1
#define IA32_PG_WRITETHROUGH	0
#define IA32_PG_WRITEBACK	1
#define IA32_PG_READONLY	0
#define IA32_PG_WRITABLE	1
#define IA32_PG_PRIVILEGED	0
#define IA32_PG_USER		1
#define IA32_PG_NONGLOBAL	0
#define IA32_PG_GLOBAL		1

/*
 * page directory entry flags
 */

#define IA32_PDE_FLAG_A			(1 << 5)
#define IA32_PDE_FLAG_CD		(1 << 4)
#define IA32_PDE_FLAG_WT		(1 << 3)
#define IA32_PDE_FLAG_USER		(1 << 2)
#define IA32_PDE_FLAG_SUPERVISOR	(0 << 2)
#define IA32_PDE_FLAG_RO		(0 << 1)
#define IA32_PDE_FLAG_RW		(1 << 1)
#define IA32_PDE_FLAG_P			(1 << 0)

/*
 * the following flags are not arch-managed. they are "available" bits of the
 * entry descriptor.
 */

#define IA32_PDE_FLAG_USED		(1 << 9)

/*
 * page table entry flags
 */

#define IA32_PTE_FLAG_G			(1 << 8)
#define IA32_PTE_FLAG_D			(1 << 6)
#define IA32_PTE_FLAG_A			(1 << 5)
#define IA32_PTE_FLAG_CD		(1 << 4)
#define IA32_PTE_FLAG_WT		(1 << 3)
#define IA32_PTE_FLAG_USER		(1 << 2)
#define IA32_PTE_FLAG_SUPERVISOR	(0 << 2)
#define IA32_PTE_FLAG_RO		(0 << 1)
#define IA32_PTE_FLAG_RW		(1 << 1)
#define IA32_PTE_FLAG_P			(1 << 0)

/*
 * the following flags are not arch-managed. they are "available" bits of the
 * entry descriptor.
 */

#define IA32_PTE_FLAG_USED		(1 << 9)

/*
 * the following value is the mirror entry in the page directory.
 */

#define IA32_PD_MIRROR			1023

/*
 * ---------- macro functions -------------------------------------------------
 */

#define IA32_PDE_ENTRY(Paddr)						\
  (((Paddr) >> 22) & 0x3ff)

#define IA32_PTE_ENTRY(Paddr)						\
  (((Paddr) >> 12) & 0x3ff)

#define IA32_ENTRY_ADDR(D,T)						\
  ((((D) << 22) | ((T) << 12)))

#define IA32_MK_BASE(V)							\
  (((t_uint32)(V)) & 0xfffff000)

#define IA32_MK_OFFSET(V)						\
  (((t_uint32)(V)) & ~0xfffff000)

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * abstract record for page
 */

typedef struct
{
  t_paddr	addr;
  t_uint8	present;
  t_uint8	rw;
  t_uint8	user;
  t_uint8	global;
  t_uint8	cached;
  t_uint8	writeback;
}		t_ia32_page;

/*
 * abstract record for table
 */

typedef struct
{
  t_paddr	paddr;
  t_vaddr	vaddr;
  t_uint8	present;
  t_uint8	rw;
  t_uint8	user;
  t_uint8	cached;
  t_uint8	writeback;
}		t_ia32_table;

typedef t_uint32		t_ia32_pde;
typedef t_uint32		t_ia32_pte;

/*
 * abstract record for directory
 */

typedef t_ia32_pde* t_ia32_directory;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../paging.c
 *	../pd.c
 *	../pt.c
 *	../tlb.c
 */

/*
 * ../paging.c
 */

t_status		ia32_paging_init(void);

t_status		ia32_paging_clean(void);


/*
 * ../pd.c
 */

t_status		ia32_pd_dump(t_ia32_directory*		dir);

t_status		ia32_pd_build(t_paddr			base,
				      t_ia32_directory*		directory);

t_status		ia32_pd_base(t_ia32_directory*		dir,
				     t_paddr*			base);

t_status		ia32_pd_activate(t_ia32_directory	dir,
					 t_uint32		cached,
					 t_uint32		writeback);

t_status		ia32_pd_get_cr3(t_uint32*		cr3,
					t_ia32_directory	dir,
					t_uint32		cached,
					t_uint32		writeback);

t_status		ia32_pd_add_table(t_ia32_directory*	dir,
					  t_uint16		entry,
					  t_ia32_table		table);

t_status		ia32_pd_get_table(t_ia32_directory*	dir,
					  t_uint16		entry,
					  t_ia32_table*		table);

t_status		ia32_pd_delete_table(t_ia32_directory*	dir,
					     t_uint16		entry);


/*
 * ../pt.c
 */

t_status		ia32_pt_dump(t_ia32_pte*		tab);

t_status		ia32_pt_build(t_paddr			base,
				      t_ia32_table*		table);

t_status		ia32_pt_add_page(t_ia32_table*		tab,
					 t_uint16		entry,
					 t_ia32_page		page);

t_status		ia32_pt_get_page(t_ia32_table*		tab,
					 t_uint16		entry,
					 t_ia32_page*		page);

t_status		ia32_pt_delete_page(t_ia32_table*	tab,
					    t_uint16		entry);


/*
 * ../tlb.c
 */

t_status		ia32_tlb_invalidate(t_vaddr			page);

t_status		ia32_tlb_flush(void);


/*
 * eop
 */

#endif
