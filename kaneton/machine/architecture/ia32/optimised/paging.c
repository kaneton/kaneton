/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/paging/paging.c
 *
 * created       matthieu bucchianeri   [tue dec 20 13:45:05 2005]
 * updated       matthieu bucchianeri   [tue feb  6 19:34:23 2007]
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

#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- globals ---------------------------------------------------------
 */


/*							    [block::globals] */


/*
 * the init variable.
 */

extern t_init*		init;

/*
 * the page directory.
 */

extern t_ia32_directory	ia32_pd;

/*							 [endblock::globals] */

/*
 * ---------- functions -------------------------------------------------------
 */

/*							  [block::functions] */

/*
 * enable paging  by setting  the higher order  bit of  CR0. activates
 * global pages with bit 7 of CR4.
 */

static void		ia32_paging_enable(void)
{
  asm volatile("movl %%cr0, %%eax\n\t"
	       "orl $0x80000000, %%eax\n\t"
	       "movl %%eax, %%cr0\n\t"
	       :
	       :
	       : "%eax", "memory");

  asm volatile("movl %%cr4, %%eax\n\t"
	       "orl $128, %%eax\n\t"
	       "movl %%eax, %%cr4\n\t"
	       :
	       :
	       : "%eax");
}

/*
 * initialize paging.
 *
 * steps:
 *
 * 1) copies the directory structure from the init variable.
 * 2) enables paging.
 */

t_status		ia32_paging_init(void)
{

  /*
   * 1)
   */

  ia32_pd = init->machine.pd;

  /*
   * 2)
   */

  ia32_paging_enable();

  return STATUS_OK;
}

/*
 * cleans paging.
 */

t_status		ia32_paging_clean(void)
{
  return STATUS_OK;
}

/*						       [endblock::functions] */
