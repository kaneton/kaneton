/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/paging/paging.c
 *
 * created       matthieu bucchianeri   [tue dec 20 13:45:05 2005]
 * updated       matthieu bucchianeri   [sat oct 14 12:24:39 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * manage paging.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*                                                                  [cut] k2 */

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the init variable.
 */

extern t_init*		init;

/*
 * the page directory.
 */

extern t_ia32_directory	pd;

/*                                                                 [cut] /k2 */

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

/*
 * initialise paging.
 *
 * steps:
 *
 * 1) copies the directory structure from the init variable.
 * 2) enables paging.
 */

t_error			paging_init(void)
{

  /*
   * 1)
   */

  pd = init->machdep.pd;

  /*
   * 2)
   */

  paging_enable();

  return ERROR_NONE;
}

/*
 * enable paging  by setting  the higher order  bit of  CR0. activates
 * global pages with bit 7 of CR4.
 */

t_error			paging_enable(void)
{
  asm volatile("movl %%cr0, %%eax\n\t"
	       "orl $0x80000000, %%eax\n\t"
	       "movl %%eax, %%cr0\n\t"
	       :
	       :
	       : "%eax");

  asm volatile("movl %%cr4, %%eax\n\t"
	       "orl $128, %%eax\n\t"
	       "movl %%eax, %%cr4\n\t"
	       :
	       :
	       : "%eax");

  return ERROR_NONE;
}

/*
 * cleans paging.
 */

t_error			paging_clean(void)
{
  return ERROR_NONE;
}

/*                                                                 [cut] /k2 */
