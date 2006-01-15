/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/libs/libia32/include/paging/paging.h
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [sun jan 15 19:07:31 2006]
 */

#ifndef IA32_IA32_PAGING_H
#define IA32_IA32_PAGING_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "../misc/types.h"

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * misc
 */

#define PD_MAX_ENTRIES		1024
#define PT_MAX_ENTRIES		1024
#define PAGE_SIZE		4096

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

#define MK_OFFSET(V)							\
  (((t_uint32)(V)) & ~0xfffffc00)

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
