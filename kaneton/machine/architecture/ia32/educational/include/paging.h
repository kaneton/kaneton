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

#define IA32_PAGE_DIRECTORY_MAX_ENTRIES			1024
#define IA32_PAGE_DIRECTORY_NOTCACHED			0
#define IA32_PAGE_DIRECTORY_CACHED			1
#define IA32_PAGE_DIRECTORY_WRITETHROUGH		0
#define IA32_PAGE_DIRECTORY_WRITEBACK			1
#define IA32_PAGE_DIRECTORY_CURRENT			NULL

#define IA32_PAGE_TABLE_MAX_ENTRIES			1024
#define IA32_PAGE_TABLE_NOTCACHED			0
#define IA32_PAGE_TABLE_CACHED				1
#define IA32_PAGE_TABLE_WRITETHROUGH			0
#define IA32_PAGE_TABLE_WRITEBACK			1
#define IA32_PAGE_TABLE_READONLY			0
#define IA32_PAGE_TABLE_WRITABLE			1
#define IA32_PAGE_TABLE_PRIVILEGED			0
#define IA32_PAGE_TABLE_USER				1

#define IA32_PAGE_NOTCACHED				0
#define IA32_PAGE_CACHED				1
#define IA32_PAGE_WRITETHROUGH				0
#define IA32_PAGE_WRITEBACK				1
#define IA32_PAGE_READONLY				0
#define IA32_PAGE_WRITABLE				1
#define IA32_PAGE_PRIVILEGED				0
#define IA32_PAGE_USER					1
#define IA32_PAGE_NONGLOBAL				0
#define IA32_PAGE_GLOBAL				1

/*
 * page directory entry flags
 */

#define IA32_PAGE_DIRECTORY_ENTRY_FLAG_A		(1 << 5)
#define IA32_PAGE_DIRECTORY_ENTRY_FLAG_CD		(1 << 4)
#define IA32_PAGE_DIRECTORY_ENTRY_FLAG_WT		(1 << 3)
#define IA32_PAGE_DIRECTORY_ENTRY_FLAG_USER		(1 << 2)
#define IA32_PAGE_DIRECTORY_ENTRY_FLAG_SUPERVISOR	(0 << 2)
#define IA32_PAGE_DIRECTORY_ENTRY_FLAG_RO		(0 << 1)
#define IA32_PAGE_DIRECTORY_ENTRY_FLAG_RW		(1 << 1)
#define IA32_PAGE_DIRECTORY_ENTRY_FLAG_P		(1 << 0)

/*
 * the following flags are not arch-managed. they are "available" bits of the
 * entry descriptor.
 */

#define IA32_PAGE_DIRECTORY_ENTRY_FLAG_USED		(1 << 9)

/*
 * page table entry flags
 */

#define IA32_PAGE_TABLE_ENTRY_FLAG_G			(1 << 8)
#define IA32_PAGE_TABLE_ENTRY_FLAG_D			(1 << 6)
#define IA32_PAGE_TABLE_ENTRY_FLAG_A			(1 << 5)
#define IA32_PAGE_TABLE_ENTRY_FLAG_CD			(1 << 4)
#define IA32_PAGE_TABLE_ENTRY_FLAG_WT			(1 << 3)
#define IA32_PAGE_TABLE_ENTRY_FLAG_USER			(1 << 2)
#define IA32_PAGE_TABLE_ENTRY_FLAG_SUPERVISOR		(0 << 2)
#define IA32_PAGE_TABLE_ENTRY_FLAG_RO			(0 << 1)
#define IA32_PAGE_TABLE_ENTRY_FLAG_RW			(1 << 1)
#define IA32_PAGE_TABLE_ENTRY_FLAG_P			(1 << 0)

/*
 * the following flags are not arch-managed. they are "available" bits of the
 * entry descriptor.
 */

#define IA32_PAGE_TABLE_ENTRY_FLAG_USED			(1 << 9)

/*
 * the following value is the mirror entry in the page directory.
 */

#define IA32_PAGE_DIRECTORY_MIRROR			1023

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * compute the page directory entry's index associated with the given
 * virtual address.
 */
#define IA32_PAGE_DIRECTORY_ENTRY_INDEX(_vaddr_)			\
  (((_vaddr_) >> 22) & 0x3ff)

/*
 * compute the page table entry's index associated with the given
 * virtual address.
 */
#define IA32_PAGE_TABLE_ENTRY_INDEX(_vaddr_)				\
  (((_vaddr_) >> 12) & 0x3ff)

/*
 * compute a physical address according to the given directory and table
 * entries.
 */
#define IA32_ENTRY_ADDRESS(_directory_entry_, _table_entry_)		\
  ((((_directory_entry_) << 22) | ((_table_entry_) << 12)))

/*
 * computes the aligned address of a directory, table or page address.
 */
#define IA32_BASE(_vaddr_)						\
  (((t_uint32)(_vaddr_)) & 0xfffff000)

/*
 * computes the offset from an aligned directory, table or page address.
 */
#define IA32_OFFSET(_vaddr_)						\
  (((t_uint32)(_vaddr_)) & ~0xfffff000)

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

t_error			ia32_paging_init(void);

t_error			ia32_paging_clean(void);


/*
 * ../pd.c
 */

t_error			XXX_ia32_page_map(t_paddr		paddr,
					  t_vaddr*		vaddr);

t_error			XXX_ia32_page_unmap(t_vaddr		vaddr);

t_error			XXX_ia32_directory_dump(t_paddr		paddr);

t_error			XXX_ia32_table_dump(t_ia32_directory	directory,
					    t_uint32		index);

t_error			ia32_pd_dump(t_ia32_directory*		dir);

t_error			ia32_pd_build(t_paddr			base,
				      t_ia32_directory*		directory);

t_error			ia32_pd_base(t_ia32_directory*		dir,
				     t_paddr*			base);

t_error			ia32_pd_activate(t_ia32_directory	dir,
					 t_uint32		cached,
					 t_uint32		writeback);

t_error			ia32_pd_get_cr3(t_uint32*		cr3,
					t_ia32_directory	dir,
					t_uint32		cached,
					t_uint32		writeback);

t_error			ia32_pd_add_table(t_ia32_directory*	dir,
					  t_uint16		entry,
					  t_ia32_table		table);

t_error			ia32_pd_get_table(t_ia32_directory*	dir,
					  t_uint16		entry,
					  t_ia32_table*		table);

t_error			ia32_pd_delete_table(t_ia32_directory*	dir,
					     t_uint16		entry);


/*
 * ../pt.c
 */

t_error			ia32_pt_dump(t_ia32_pte*		tab,
				     int			pde);

t_error			ia32_pt_build(t_paddr			base,
				      t_ia32_table*		table);

t_error			ia32_pt_add_page(t_ia32_table*		tab,
					 t_uint16		entry,
					 t_ia32_page		page);

t_error			ia32_pt_get_page(t_ia32_table*		tab,
					 t_uint16		entry,
					 t_ia32_page*		page);

t_error			ia32_pt_delete_page(t_ia32_table*	tab,
					    t_uint16		entry);


/*
 * ../tlb.c
 */

t_error			ia32_tlb_invalidate(t_vaddr			page);

t_error			ia32_tlb_flush(void);


/*
 * eop
 */

#endif
