/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/paging/tlb.c
 *
 * created       matthieu bucchianeri   [tue dec 20 19:57:00 2005]
 * updated       matthieu bucchianeri   [mon jan 30 23:55:14 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * manage translation lookaside buffers.
 *
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * this  file  must  be   filled  with  everything  about  translation
 * lookaside buffers.
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

t_error			tlb_invalidate(t_paddr			page)
{
  asm volatile("invlpg %0"
	       :
	       : "g" (page));

  return ERROR_NONE;
}

/*
 * flushes the whole pd and pt caches.
 */

t_error			tlb_flush(void)
{
  asm volatile("movl %%cr0, %%eax\n\t"
	       "movl %%eax, %%cr0"
	       :
	       :
	       : "%eax");

  return ERROR_NONE;
}

/*                                                                 [cut] /k2 */
