/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/include/paging/paging.h
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [wed aug 30 16:52:00 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * declares paging structures, macros, etc.
 *
 */

#ifndef IA32_IA32_PAGING_H
#define IA32_IA32_PAGING_H	1

/*                                                                  [cut] k2 */

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "../misc/types.h"

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * misc
 */

#define PD_MAX_ENTRIES		1024
#define PT_MAX_ENTRIES		1024
#define PD_NOTCACHED		0
#define PD_CACHED		1
#define PD_WRITETHROUGH		0
#define PD_WRITEBACK		1

#define PT_NOTCACHED		0
#define PT_CACHED		1
#define PT_WRITETHROUGH		0
#define PT_WRITEBACK		1
#define PT_READONLY		0
#define PT_WRITABLE		1
#define PT_PRIVILEGED		0
#define PT_USER			1

#define PG_NOTCACHED		0
#define PG_CACHED		1
#define PG_WRITETHROUGH		0
#define PG_WRITEBACK		1
#define PG_READONLY		0
#define PG_WRITABLE		1
#define PG_PRIVILEGED		0
#define PG_USER			1
#define PG_NONGLOBAL		0
#define PG_GLOBAL		1

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
 * the following flags are not arch-managed. they are "available" bits of the
 * entry descriptor.
 */

#define PDE_FLAG_USED		(1 << 9)

/*
 * page table entry flags
 */

#define PTE_FLAG_G		(1 << 8)
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
 * the following flags are not arch-managed. they are "available" bits of the
 * entry descriptor.
 */

#define PTE_FLAG_USED		(1 << 9)

/*
 * the following value is the mirror entry in the page directory.
 */

#define PD_MIRROR		1023

/*
 * ---------- macro functions -------------------------------------------------
 */

#define PDE_ENTRY(Paddr)						\
  (((Paddr) >> 22) & 0x3ff)

#define PTE_ENTRY(Paddr)						\
  (((Paddr) >> 12) & 0x3ff)

#define ENTRY_ADDR(D,T)							\
  ((((D) << 22) | ((T) << 12)))

#define MK_BASE(V)							\
  (((t_uint32)(V)) & 0xfffff000)

#define MK_OFFSET(V)							\
  (((t_uint32)(V)) & ~0xfffff000)

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
  t_paddr	entries;
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

/*                                                                 [cut] /k2 */

#endif
