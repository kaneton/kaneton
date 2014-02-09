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

#define AS_SHARED_ADDRESS 0x0FFF1000

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel		_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void test_architecture_as_switch(void) __attribute__((section(".handler_code")));

void			test_architecture_as_switch(void)
{
  i_task		task;
  i_as			as;
  i_segment		seg;
  i_region		reg;
  o_as*			ko;
  t_paddr		kdir;
  at_cr3		kpdbr;
  o_as*			o;
  t_paddr		dir;
  at_cr3		pdbr;
  t_vaddr               ptr;
  o_region*		r;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_KERNEL,
		   TASK_BEHAVIOUR_INTERACTIVE,
		   TASK_PRIORITY_INTERACTIVE,
		   &task) != STATUS_OK)
    TEST_ERROR("[task_reserve] error");

  if (as_reserve(task, &as) != STATUS_OK)
    TEST_ERROR("[as_reserve] error");

  if (as_get(as, &o) != STATUS_OK)
    TEST_ERROR("[as_get] error");

  if (segment_reserve(as,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &seg) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(_kernel.as,
                     seg,
                     0,
                     REGION_OPTION_NONE,
                     0x0,
                     ___kaneton$pagesz,
                     &reg) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_get(_kernel.as, reg, &r) != STATUS_OK)
    TEST_ERROR("[region_get] error");

  ptr = r->address;

  if (region_reserve(as,
                     seg,
                     0,
                     REGION_OPTION_FORCE,
                     AS_SHARED_ADDRESS,
                     ___kaneton$pagesz,
                     &reg) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  if (segment_reserve(as,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &seg) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(as,
		     seg,
		     0,
		     REGION_OPTION_NONE,
		     0x0,
		     ___kaneton$pagesz,
		     &reg) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_get(as, reg, &r) != STATUS_OK)
    TEST_ERROR("[region_get] error");

  if (as_get(_kernel.as, &ko) != STATUS_OK)
    TEST_ERROR("[as_get] error");

  kdir = ko->machine.pd;

  if (architecture_paging_pdbr(kdir,
			       ARCHITECTURE_REGISTER_CR3_PCE |
			       ARCHITECTURE_REGISTER_CR3_PWB,
			       &kpdbr) != STATUS_OK)
    TEST_ERROR("[architecture_paging_pdbr] error");

  if (as_get(as, &o) != STATUS_OK)
    TEST_ERROR("[as_get] error");

  dir = o->machine.pd;

  if (architecture_paging_pdbr(dir,
			       ARCHITECTURE_REGISTER_CR3_PCE |
			       ARCHITECTURE_REGISTER_CR3_PWB,
			       &pdbr) != STATUS_OK)
    TEST_ERROR("[architecture_paging_pdbr] error");

  asm volatile("mov  %%cr3, %%ecx               ;\n"
               "mov  %0, %%cr3                  ;\n"
               "movl $0x41424344, 0x0FFF1000    ;\n"
               "mov  %%ecx, %%cr3               ;\n"
               : /**/
               : "a" (pdbr)
               : "ecx", "memory");

  if (*((t_uint32*) ptr) != 0x41424344)
    TEST_ERROR("the data written through the kernel address space is invalid "
	       "in the task's");

  TEST_SIGNATURE(vsdowkf43g3hg3g30hg);

  TEST_LEAVE();
}
