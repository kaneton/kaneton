/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...achine/architecture/ia32/event/map/map.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:21:09 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "map.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel		_kernel;
extern m_as		_as;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_event_map(void)
{
  as_idtr		idtr;
  t_paddr		paddr;
  t_vaddr		vaddr;
  s_iterator		it;
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
		       &tskid) != STATUS_OK)
	TEST_ERROR("[task_reserve] error");

      if (as_reserve(tskid, &asid) != STATUS_OK)
	TEST_ERROR("[as_reserve] error");
    }

  memset(&idtr, 0, sizeof (idtr));

  ARCHITECTURE_SIDT(idtr);

  if (as_physical(_kernel.as, idtr.address, &paddr) != STATUS_OK)
    TEST_ERROR("[as_physical] error");

  set_foreach(SET_OPTION_FORWARD, _as.ass, &it, state)
    {
      if (set_object(_as.ass, it, (void**)&o) != STATUS_OK)
	TEST_ERROR("[set_object] error");

      if (as_virtual(o->id, paddr, &vaddr) != STATUS_OK)
	TEST_ERROR("[as_virtual] error: the IDT does not seem to be "
		   "mapped in this address space");
    }

  TEST_SIGNATURE(cmnweiovw0fg934gh);

  TEST_LEAVE();
}
