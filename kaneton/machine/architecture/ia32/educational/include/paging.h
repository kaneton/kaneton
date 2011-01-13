/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ecture/ia32/educational/include/paging.h
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       julien quintard   [wed jan 12 03:56:05 2011]
 */

#ifndef ARCHITECTURE_PAGING_H
#define ARCHITECTURE_PAGING_H	1

/*
 * ---------- macro-functions -------------------------------------------------
 */

/*
 * return the aligned base of a given address.
 */

#define ARCHITECTURE_PAGING_BASE(_vaddr_)				\
  ((_vaddr_) & 0xfffff000)

/*
 * computes a virtual address according to the given directory and table
 * entries.
 */
#define ARCHITECTURE_PAGING_ADDRESS(_pde_, _pte_)			\
  (t_vaddr)(((_pde_) << 22) | ((_pte_) << 12))

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <architecture/pd.h>
#include <architecture/pt.h>
#include <architecture/register.h>


























/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * misc
 */

#define IA32_PAGE_DIRECTORY_MAX_ENTRIES                 1024
#define IA32_PAGE_DIRECTORY_NOTCACHED                   0
#define IA32_PAGE_DIRECTORY_CACHED                      1
#define IA32_PAGE_DIRECTORY_WRITETHROUGH                0
#define IA32_PAGE_DIRECTORY_WRITEBACK                   1
#define IA32_PAGE_DIRECTORY_CURRENT                     NULL

#define IA32_PAGE_TABLE_MAX_ENTRIES				1024
#define IA32_PAGE_TABLE_NOTCACHED			0
#define IA32_PAGE_TABLE_CACHED				1
#define IA32_PAGE_TABLE_WRITETHROUGH			0
#define IA32_PAGE_TABLE_WRITEBACK			1
#define IA32_PAGE_TABLE_READONLY			0
#define IA32_PAGE_TABLE_WRITABLE			1
#define IA32_PAGE_TABLE_PRIVILEGED			0
#define IA32_PAGE_TABLE_USER				1

#define IA32_PAGE_DIRECTORY_ENTRY_FLAG_A                (1 << 5)
#define IA32_PAGE_DIRECTORY_ENTRY_FLAG_CD               (1 << 4)
#define IA32_PAGE_DIRECTORY_ENTRY_FLAG_WT               (1 << 3)
#define IA32_PAGE_DIRECTORY_ENTRY_FLAG_USER             (1 << 2)
#define IA32_PAGE_DIRECTORY_ENTRY_FLAG_SUPERVISOR       (0 << 2)
#define IA32_PAGE_DIRECTORY_ENTRY_FLAG_RO               (0 << 1)
#define IA32_PAGE_DIRECTORY_ENTRY_FLAG_RW               (1 << 1)
#define IA32_PAGE_DIRECTORY_ENTRY_FLAG_P                (1 << 0)

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
  (t_vaddr)((((_directory_entry_) << 22) | ((_table_entry_) << 12)))

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
 *
 * XXX si jamais on garde les records abstract, les mettre dans architecture.h
 * et bien explique leur role!
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

#include <architecture/pd.h>

// XXX
typedef t_uint32		t_ia32_pde;
typedef t_uint32		t_ia32_pte;
typedef at_pde*			t_ia32_directory;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../paging.c
 *	../pd.c
 *	../pt.c
 */

/*
 * ../paging.c
 */

t_error			architecture_paging_enable(void);

t_error			architecture_paging_setup(void);

t_error			architecture_paging_cr3(at_pd		pd,
						t_flags		flags,
						at_cr3*		cr3);

t_error			architecture_paging_import(at_pd	pd,
						   t_flags	flags);


/*
 * ../pd.c
 */

t_error			architecture_pd_dump(at_pd		pd);

t_error			architecture_pd_build(at_pd		pd);

t_error			architecture_pd_insert(at_pd		pd,
					       at_pdei		index,
					       t_paddr		address,
					       t_flags		flags);

t_error			architecture_pd_delete(at_pd		pd,
					       at_pdei		index);

t_error			architecture_pd_map(t_paddr		paddr,
					    at_pd*		table);

t_error			architecture_pd_unmap(at_pd		table);


/*
 * ../pt.c
 */

t_error			architecture_pt_dump(at_pt		pt,
					     mt_margin		margin);

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
