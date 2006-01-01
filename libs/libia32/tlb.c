/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/libs/libia32/tlb.c
 *
 * created       matthieu bucchianeri   [tue dec 20 19:57:00 2005]
 * updated       matthieu bucchianeri   [fri dec 30 19:54:44 2005]
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

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

/*
 * flushes a single pte cache given a page address.
 */

t_error			flush_tlb(t_vaddr	page)
{
  asm volatile("invlpg %0"
	       :
	       : "g" (page));

  return ERROR_NONE;
}

/*
 * flushes the whole pd and pt caches.
 */

t_error			flush_tlbs(void)
{
  asm volatile("movl %%cr0, %%eax\n\t"
	       "movl %%eax, %%cr0"
	       :
	       :
	       : "%eax");

  return ERROR_NONE;
}

/*                                                                 [cut] /k2 */
