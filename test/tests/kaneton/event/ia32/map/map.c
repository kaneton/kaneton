/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../test/tests/kaneton/event/ia32/map/map.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun nov 14 14:09:32 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "map.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_as*		as;
extern i_as		kasid;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_event_ia32_map(void)
{
  t_ia32_idtr		idtr;
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
		       TASK_BEHAV_INTERACTIVE,
		       TASK_PRIOR_INTERACTIVE,
		       &tskid) != ERROR_NONE)
	TEST_ERROR("[task_reserve] error\n");

      if (as_reserve(tskid, &asid) != ERROR_NONE)
	TEST_ERROR("[as_reserve] error\n");
    }

  memset(&idtr, 0, sizeof (idtr));

  SIDT(idtr);

  if (as_paddr(kasid, idtr.address, &paddr) != ERROR_NONE)
    TEST_ERROR("[as_paddr] error\n");

  set_foreach(SET_OPT_FORWARD, as->ass, &it, state)
    {
      if (set_object(as->ass, it, (void**)&o) != ERROR_NONE)
	TEST_ERROR("[set_object] error\n");

      if (as_vaddr(o->asid, paddr, &vaddr) != ERROR_NONE)
	TEST_ERROR("[as_vaddr] error: the IDT does not seem to be "
		   "mapped in this address space\n");
    }

  TEST_LEAVE();
}
