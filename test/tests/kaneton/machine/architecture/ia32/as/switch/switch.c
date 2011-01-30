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
 * updated       julien quintard   [sun jan 30 21:19:47 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "switch.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel		_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_as_switch(void)
{
  i_task		task;
  i_as			as;
  int			i;
  int			j;
  i_segment		seg;
  i_region		reg;
  t_vaddr		addr;
  o_as*			ko;
  t_paddr		kdir;
  at_cr3		kpdbr;
  o_as*			o;
  t_paddr		dir;
  at_cr3		pdbr;
  int*			ptr1;
  int*			ptr2;
  o_region*		r;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_KERNEL,
		   TASK_BEHAVIOUR_INTERACTIVE,
		   TASK_PRIORITY_INTERACTIVE,
		   &task) != ERROR_OK)
    TEST_ERROR("[task_reserve] error");

  if (as_reserve(task, &as) != ERROR_OK)
    TEST_ERROR("[as_reserve] error");

  if (as_get(as, &o) != ERROR_OK)
    TEST_ERROR("[as_get] error");

  if (segment_reserve(as,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(as,
		     seg,
		     0,
		     REGION_OPTION_NONE,
		     0,
		     ___kaneton$pagesz,
		     &reg) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_get(as, reg, &r) != ERROR_OK)
    TEST_ERROR("[region_get] error");

  if (map_reserve(as,
		  MAP_OPTION_NONE,
		  ___kaneton$pagesz,
		  PERMISSION_READ | PERMISSION_WRITE,
		  &addr) != ERROR_OK)
    TEST_ERROR("[map_reserve] error");

  ptr1 = (int*)r->address;
  ptr2 = (int*)addr;

  if (as_get(_kernel.as, &ko) != ERROR_OK)
    TEST_ERROR("[as_get] error");

  kdir = ko->machine.pd;

  if (architecture_paging_pdbr(kdir,
			       ARCHITECTURE_REGISTER_CR3_PCE |
			       ARCHITECTURE_REGISTER_CR3_PWB,
			       &kpdbr) != ERROR_OK)
    TEST_ERROR("[architecture_paging_pdbr] error");

  if (as_get(as, &o) != ERROR_OK)
    TEST_ERROR("[as_get] error");

  dir = o->machine.pd;

  if (architecture_paging_pdbr(dir,
			       ARCHITECTURE_REGISTER_CR3_PCE |
			       ARCHITECTURE_REGISTER_CR3_PWB,
			       &pdbr) != ERROR_OK)
    TEST_ERROR("[architecture_paging_pdbr] error");

  ARCHITECTURE_LCR3(pdbr);

  *ptr1 = 0x41424344;
  i = *ptr1;
  *ptr2 = 0x40414243;
  j = *ptr2;

  ARCHITECTURE_LCR3(kpdbr);

  if (i != 0x41424344)
    TEST_ERROR("the data written through the kernel address space is invalid "
	       "in the task's");

  if (j != 0x40414243)
    TEST_ERROR("the data written through the kernel address space is invalid "
	       "in the task's");

  TEST_SIGNATURE(vsdowkf43g3hg3g30hg);

  TEST_LEAVE();
}
