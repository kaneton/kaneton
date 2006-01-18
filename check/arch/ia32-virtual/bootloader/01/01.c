/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/arch/ia32-virtual/bootloader/01/01.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:15 2005]
 * updated       matthieu bucchianeri   [wed jan 18 20:11:16 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../../../check/common/common.h"

extern t_init*	init;

/*
 * t_init test XXX to be continued !
 */

void		check_bootloader_01(void)
{
  TEST_ENTER;

  if (init->initsz != PAGESZ)
    printf("bad initsz field\n");
  if (init->init != (t_paddr)init)
    printf("bad init field\n");

  TEST_LEAVE;
}
