/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/as/03/03.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [fri mar 17 14:41:53 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

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

  MY_ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  MY_ASSERT(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  MY_ASSERT(segment_reserve(as,
			    1024 * PAGESZ,
			    PERM_READ | PERM_WRITE,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  /* reserve kcode and kstack */

  MY_ASSERT(region_reserve(as,
			   (t_segid)init->kcode,
			   0,
			   REGION_OPT_FORCE,
			   (t_vaddr)init->kcode,
			   init->kcodesz,
			   &reg) == ERROR_NONE,
	   "error reserving region for kcode\n");

  MY_ASSERT(region_reserve(as,
			   (t_segid)init->kstack,
			   0,
			   REGION_OPT_FORCE,
			   (t_vaddr)init->kstack,
			   init->kstacksz,
			   &reg) == ERROR_NONE,
	    "error reserving region for kstack\n");

  /* reserve console */

  MY_ASSERT(region_reserve(as,
			   0x1000,
			   0,
			   REGION_OPT_FORCE,
			   0x1000,
			   INIT_ISA_SIZE - 0x1000,
			   &reg) == ERROR_NONE,
	    "error reserving region for ISA\n");

  /* test region */

  MY_ASSERT(region_reserve(as,
			   seg,
			   0,
			   REGION_OPT_FORCE,
			   1024 * PAGESZ,
			   1024 * PAGESZ,
			   &reg) == ERROR_NONE,
	    "error reserving region\n");

  ptr = (int*)reg;

  MY_ASSERT(as_get(kasid, &o) == ERROR_NONE,
	    "cannot get kernel as\n");

  kdir = o->machdep.pd;

  MY_ASSERT(as_get(as, &o) == ERROR_NONE,
	    "cannot get created as\n");

  MY_ASSERT(pd_activate(o->machdep.pd) == ERROR_NONE,
	    "cannot switch as\n");

  /* R/W test */

  *ptr = 0x41424344;
  i = *ptr;

  pd_activate(kdir);

  MY_ASSERT(i == 0x41424344, "read/write test failed\n");

  MY_ASSERT(segment_release(seg) == ERROR_NONE,
	    "failed to release segment");

  MY_ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  MY_ASSERT(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE;
}
