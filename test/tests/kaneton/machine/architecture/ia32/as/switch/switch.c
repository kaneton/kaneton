/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ine/architecture/ia32/as/switch/switch.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat nov 20 23:55:32 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "switch.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern t_init*		init;
extern i_as		kasid;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_ia32_as_switch(void)
{
  i_task		task;
  i_as			as;
  int			i;
  int			j;
  i_segment		seg;
  i_region		reg;
  t_vaddr		addr;
  o_as*			o;
  t_ia32_directory	kdir;
  int*			ptr1;
  int*			ptr2;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE,
		   &task) != ERROR_NONE)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_NONE)
    TEST_ERROR("[as_reserve] error\n");

  if (as_get(as, &o) != ERROR_NONE)
    TEST_ERROR("[as_get] error\n");

  if (region_reserve(as,
		     (i_segment)init->kstack,
		     0,
		     REGION_OPT_FORCE,
		     (t_vaddr)init->kstack,
		     init->kstacksz,
		     &reg) != ERROR_NONE)
    TEST_ERROR("[region_reserve] error: unable to map the kstack segment "
	       "at a precise address\n");

  if (segment_reserve(as,
		      PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &seg) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(as,
		     seg,
		     0,
		     REGION_OPT_NONE,
		     0,
		     PAGESZ,
		     &reg) != ERROR_NONE)
    TEST_ERROR("[region_reserve] error");

  if (map_reserve(as,
		  MAP_OPT_NONE,
		  PAGESZ,
		  PERM_READ | PERM_WRITE,
		  &addr) != ERROR_NONE)
    TEST_ERROR("[map_reserve] error");

  if (as_get(kasid, &o) != ERROR_NONE)
    TEST_ERROR("[as_get] error\n");

  kdir = o->machine.pd;

  if (as_get(as, &o) != ERROR_NONE)
    TEST_ERROR("[as_get] error\n");

  ptr1 = (int*)(t_uint32)reg;
  ptr2 = (int*)addr;

  if (ia32_pd_activate(o->machine.pd,
		       IA32_PD_CACHED,
		       IA32_PD_WRITEBACK) != ERROR_NONE)
    TEST_ERROR("[ia32_pd_activate] error: unable to activate the task's "
	       "address space\n");

  *ptr1 = 0x41424344;
  i = *ptr1;
  *ptr2 = 0x40414243;
  j = *ptr2;

  if (ia32_pd_activate(kdir,
		       IA32_PD_CACHED,
		       IA32_PD_WRITEBACK) != ERROR_NONE)
    TEST_ERROR("[ia32_pd_activate] error: unable to activate the kernel's "
	       "address space\n");

  if (i != 0x41424344)
    TEST_ERROR("the data written through the kernel address space is invalid "
	       "in the task's\n");

  if (j != 0x40414243)
    TEST_ERROR("the data written through the kernel address space is invalid "
	       "in the task's\n");

  TEST_LEAVE();
}
