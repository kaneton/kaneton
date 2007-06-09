/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/misc/extensions.c
 *
 * created       matthieu bucchianeri   [wed jul 26 14:34:41 2006]
 * updated       matthieu bucchianeri   [wed jul 26 14:39:58 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * Manage extentions.
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

void			extensions_enable_sse(void)
{
  asm volatile("movl %%cr0, %%eax\n\t"
	       "andl $0xFFFFFFF9, %%eax\n\t"
	       "movl %%eax, %%cr0\n\t"
	       "movl %%cr4, %%eax\n\t"
	       "orl $512, %%eax\n\t"
	       "movl %%eax, %%cr4"
	       :
	       :
	       : "%eax");
}
