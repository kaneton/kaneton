/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hine/architecture/ia32/educational/tlb.c
 *
 * created       matthieu bucchianeri   [tue dec 20 19:57:00 2005]
 * updated       julien quintard   [sat jan  8 16:44:09 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file contains functions for managing the TLB - Translation
 * Lookaside Buffer.
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
 * this function invalidates a single address from the TLB.
 */

t_error			architecture_tlb_invalidate(t_vaddr	address)
{
  /*						     [block::tlb_invalidate] */

  asm volatile("invlpg (%0)"
	       :
	       : "r" (address)
	       : "memory");

  /*						  [endblock::tlb_invalidate] */

  MACHINE_LEAVE();
}

/*
 * this function flushes the whole TLB's content.
 */

t_error			architecture_tlb_flush(void)
{
  /*							  [block::tlb_flush] */

  asm volatile("movl %%cr3, %%eax\n\t"
	       "movl %%eax, %%cr3"
	       :
	       :
	       : "%eax", "memory");

  /*						       [endblock::tlb_flush] */

  MACHINE_LEAVE();
}
