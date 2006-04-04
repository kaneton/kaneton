/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/task/01/01.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [tue apr  4 01:36:19 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

extern t_init*	init;

void		check_task_01(void)
{
  t_ia32_pde	pde_start;
  t_ia32_pde	pde_end;
  t_ia32_pte	pte_start;
  t_ia32_pte	pte_end;
  t_ia32_pde	pde;
  t_ia32_pte	pte;
  t_uint32	reg;

  TEST_ENTER;

  /*  for (reg = 0; reg < init->nregions*/

  TEST_LEAVE;
}
