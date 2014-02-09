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

#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * flushes a single pte cache given a page address.
 */

t_status		ia32_tlb_invalidate(t_vaddr			page)
{
  /*						     [block::tlb_invalidate] */

  asm volatile("invlpg (%0)"
	       :
	       : "r" (page)
	       : "memory");

  /*						  [endblock::tlb_invalidate] */

  return STATUS_OK;
}

/*
 * flushes the whole pd and pt caches.
 */

t_status		ia32_tlb_flush(void)
{
  /*							  [block::tlb_flush] */

  asm volatile("movl %%cr3, %%eax\n\t"
	       "movl %%eax, %%cr3"
	       :
	       :
	       : "%eax", "memory");

  /*						       [endblock::tlb_flush] */

  return STATUS_OK;
}
