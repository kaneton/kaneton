/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/as/03/03.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [tue mar 14 15:55:28 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../../../../check/common/common.h"

extern t_asid	kasid;
extern t_init*	init;

void		check_as_03(void)
{
  t_tskid	task;
  t_asid	as;
  o_as*		o;
  t_segid	seg;
  t_regid	reg;
  t_directory	kdir;
  int*		ptr;
  int		i;

  TEST_ENTER;

  if (task_reserve(TASK_CLASS_PROGRAM, TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE, &task) != ERROR_NONE)
    {
      printf("error creating task\n");
      TEST_LEAVE;
    }

  if (as_reserve(task, &as) != ERROR_NONE)
    {
      printf("error creating as\n");
      TEST_LEAVE;
    }

  if (segment_reserve(as, 1024 * PAGESZ, PERM_READ | PERM_WRITE, &seg) !=
      ERROR_NONE)
    {
      printf("error reserving segment\n");
      TEST_LEAVE;
    }

  /* reserve kcode and kstack */

  if (region_reserve(as, (t_segid)init->kcode, 0, REGION_OPT_FORCE,
		     (t_vaddr)init->kcode, init->kcodesz, &reg) != ERROR_NONE)
    {
      printf("error reserving region for kcode\n");
      TEST_LEAVE;
    }
  if (region_reserve(as, (t_segid)init->kstack, 0, REGION_OPT_FORCE,
		     (t_vaddr)init->kstack, init->kstacksz, &reg) != ERROR_NONE)
    {
      printf("error reserving region for kstack\n");
      TEST_LEAVE;
    }

  /* reserve console */

  if (region_reserve(as, 0x1000, 0, REGION_OPT_FORCE,
		     0x1000, INIT_ISA_SIZE - 0x1000,
		     &reg) != ERROR_NONE)
    {
      printf("error reserving region for ISA\n");
      TEST_LEAVE;
    }

  /* test region */

  if (region_reserve(as, seg, 0, REGION_OPT_FORCE, 1024 * PAGESZ,
		     1024 * PAGESZ, &reg) != ERROR_NONE)
    {
      printf("error reserving region\n");
      TEST_LEAVE;
    }

  ptr = (int*)reg;

  region_dump(as);

  if (as_get(kasid, &o) != ERROR_NONE)
    {
      printf("cannot get kernel as\n");
      TEST_LEAVE;
    }

  kdir = o->machdep.pd;

  if (as_get(as, &o) != ERROR_NONE)
    {
      printf("cannot get created as\n");
      TEST_LEAVE;
    }

  if (pd_activate(o->machdep.pd) != ERROR_NONE)
    {
      printf("cannot switch as\n");
      TEST_LEAVE;
    }

  /* R/W test */

  printf("testing r/w at address 0x%p\n", ptr);
  *ptr = 0x41424344;
  i = *ptr;
  printf("%x\n", *ptr);

  pd_activate(kdir);

  printf("i = %x\n", i);

  if (region_release(as, reg) != ERROR_NONE)
    printf("failed to release region");

  if (segment_release(seg) != ERROR_NONE)
    printf("failed to release segment");

  if (as_release(as) != ERROR_NONE)
    printf("failed to release as\n");

  if (task_release(task) != ERROR_NONE)
    printf("failed to release task\n");

  TEST_LEAVE;
}
