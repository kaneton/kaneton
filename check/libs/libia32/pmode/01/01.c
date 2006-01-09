/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/libs/libia32/pmode/01/01.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:12:15 2005]
 * updated       matthieu bucchianeri   [mon jan  9 12:39:39 2006]
 */

#include <klibc.h>
#include "../init/init.h"

/*
 * checks the cr0 register.
 */

void			check_pmode_01(void)
{
  register t_uint32	cr0;

  LIBIA32_TEST_ENTER;

  asm volatile("movl %%cr0, %0"
	       : "=r" (cr0));

  if (!(cr0 & 0x1))
    printf("bad cr0 !\n");

  LIBIA32_TEST_LEAVE;
}
