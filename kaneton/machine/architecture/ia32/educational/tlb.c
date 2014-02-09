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
 * updated       julien quintard   [sat feb  5 10:40:55 2011]
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

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function invalidates a single address from the TLB.
 */

t_status		architecture_tlb_invalidate(t_vaddr	address)
{
  asm volatile("invlpg (%0)"
	       :
	       : "r" (address)
	       : "memory");

  MACHINE_LEAVE();
}

/*
 * this function flushes the whole TLB's content.
 */

t_status		architecture_tlb_flush(void)
{
  asm volatile("movl %%cr3, %%eax\n\t"
	       "movl %%eax, %%cr3"
	       :
	       :
	       : "%eax", "memory");

  MACHINE_LEAVE();
}
