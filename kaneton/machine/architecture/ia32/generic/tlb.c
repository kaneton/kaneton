/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/paging/tlb.c
 *
 * created       matthieu bucchianeri   [tue dec 20 19:57:00 2005]
 * updated       matthieu bucchianeri   [tue feb  6 19:31:01 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * manage translation lookaside buffers.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc/libc.h>
#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * flushes a single pte cache given a page address.
 */

t_error			ia32_tlb_invalidate(t_vaddr			page)
{

  asm volatile("invlpg (%0)"
	       :
	       : "r" (page)
	       : "memory");

  return ERROR_NONE;
}

/*
 * flushes the whole pd and pt caches.
 */

t_error			ia32_tlb_flush(void)
{
  asm volatile("movl %%cr3, %%eax\n\t"
	       "movl %%eax, %%cr3"
	       :
	       :
	       : "%eax", "memory");

  return ERROR_NONE;
}
