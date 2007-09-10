/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/bootloader/arch/ia32-virtual/04/04.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:15 2005]
 * updated       matthieu bucchianeri   [sat oct 14 16:46:03 2006]
 */

#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

extern t_init*	init;

/*
 * pmode, paging and context tests
 */

void		check_ia32_virtual_04(void)
{
  register t_uint32	cr0;
  register t_uint16	cs;
  register t_uint16	ds;
  register t_uint16	ss;
  register t_uint32	esp;

  TEST_ENTER();

  asm volatile("movl %%cr0, %0"
	       : "=r" (cr0));

  if (!(cr0 & 0x1))
    printf("protected mode is not enabled !\n");
  if (!(cr0 & 0x80000000))
    printf("paging is not enabled !\n");

  asm volatile("movw %%cs, %0\n\t"
	       "movw %%ds, %1\n\t"
	       "movw %%ss, %2\n\t"
	       "movl %%esp, %3\n\t"
	       : "=r" (cs), "=r" (ds), "=r" (ss), "=r" (esp));

  if (cs & 0x3 || ds & 0x3 || ss & 0x3)
    printf("bad rpl in segment selectors\n");

  if (esp > init->kstack + init->kstacksz || esp <= init->kstack)
    printf("bad stack pointer (out of kernel stack)\n");

  TEST_LEAVE();
}
