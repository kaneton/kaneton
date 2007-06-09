/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libsun4u/include/mmu/tlb.h
 *
 * created       matthieu bucchianeri   [mon aug 21 20:05:50 2006]
 * updated       matthieu bucchianeri   [mon aug 28 17:09:52 2006]
 */

#ifndef LIBSUN4U_TLB_H
#define LIBSUN4U_TLB_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "../misc/types.h"

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * number of TLB entries. XXX this is valid for UltraSPARC IIi only.
 */

#define N_ITLB_ENTRY	64
#define N_DTLB_ENTRY	64

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * specify the kind of TLB.
 */

typedef enum
  {
			sparc64_tlb_itlb,
			sparc64_tlb_dtlb,
			sparc64_tlb_both
  }			t_sparc64_tlb_type;

/*
 * specify page size.
 */

typedef enum
  {
			sparc64_page_8k,
			sparc64_page_64k,
			sparc64_page_512k,
			sparc64_page_4m
  }			t_sparc64_pagesz;

/*
 * this is a TLB entry.
 */

typedef struct
{
  t_vaddr		virtual;
  t_paddr		physical;
  t_sparc64_pagesz	size;
  t_uint64		writable:1;
  t_uint64		global:1;
  t_uint64		locked:1;
  t_uint64		privileged:1;
  t_uint64		valid:1;
  t_uint64		context;
}			t_sparc64_tlb_entry;

#endif
