/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...achine/architecture/ia32/event/map/map.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [wed dec  1 05:46:21 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "map.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*	_kernel;
extern m_as*		_as;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_event_map(void)
{
  t_ia32_idt_register	idtr;
  t_paddr		paddr;
  t_vaddr		vaddr;
  t_iterator		it;
  t_state		state;
  int			i;
  o_as*			o;
  i_as			asid;
  i_task		tskid;

  TEST_ENTER();

  for (i = 0; i < 3; i++)
    {
      if (task_reserve(TASK_CLASS_KERNEL,
		       TASK_BEHAVIOUR_INTERACTIVE,
		       TASK_PRIORITY_INTERACTIVE,
		       &tskid) != ERROR_OK)
	TEST_ERROR("[task_reserve] error\n");

      if (as_reserve(tskid, &asid) != ERROR_OK)
	TEST_ERROR("[as_reserve] error\n");
    }

  memset(&idtr, 0, sizeof (idtr));

  SIDT(idtr);

  if (as_paddr(_kernel->as, idtr.address, &paddr) != ERROR_OK)
    TEST_ERROR("[as_paddr] error\n");

  set_foreach(SET_OPTION_FORWARD, _as->ass, &it, state)
    {
      if (set_object(_as->ass, it, (void**)&o) != ERROR_OK)
	TEST_ERROR("[set_object] error\n");

      if (as_vaddr(o->id, paddr, &vaddr) != ERROR_OK)
	TEST_ERROR("[as_vaddr] error: the IDT does not seem to be "
		   "mapped in this address space\n");
    }

  TEST_SIGNATURE(cmnweiovw0fg934gh);

  TEST_LEAVE();
}
