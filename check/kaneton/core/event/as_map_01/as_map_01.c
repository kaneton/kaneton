/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/region/01/01.c
 *
 * created       matthieu bucchianeri   [sun apr  2 19:04:14 2006]
 * updated       matthieu bucchianeri   [sun apr  9 17:41:04 2006]
 */

#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

extern m_as*	as;
extern i_as	kasid;

/*
 * reserve event onto int3 and throw an int3.
 */

void		check_event_as_map_01(void)
{
  struct
  {
    t_uint16			size;
    t_uint32			address;
  } __attribute__((packed)) idtr;
  t_paddr			paddr;
  t_vaddr			vaddr;
  t_iterator			it;
  t_state			state;
  int				i;
  o_as*				o;
  i_as				asid;
  i_task			tskid;

  TEST_ENTER();

  for (i = 0; i < 3; i++)
    {
      ASSERT(task_reserve(TASK_CLASS_CORE,
			  TASK_BEHAV_INTERACTIVE,
			  TASK_PRIOR_INTERACTIVE,
			  &tskid) == ERROR_NONE,
	     "Cannot task_reserve\n");

      ASSERT(as_reserve(tskid, &asid) == ERROR_NONE,
	     "Cannot as_reserve\n");
    }

  memset(&idtr, 0, sizeof (idtr));

  SIDT(idtr);

  /* look for IDT paddr */
  ASSERT(as_paddr(kasid, idtr.address, &paddr) == ERROR_NONE,
	 "Cannot as_paddr\n");

  set_foreach(SET_OPT_FORWARD, as->ass, &it, state)
    {
      ASSERT(set_object(as->ass, it, (void**)&o) == ERROR_NONE,
	     "Cannot set_object\n");

      ASSERT(as_vaddr(o->asid, paddr, &vaddr) == ERROR_NONE,
	     "IDT not mapped in address space\n");
    }

  TEST_LEAVE();
}
