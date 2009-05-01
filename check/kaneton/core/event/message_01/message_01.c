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

extern i_task	ktask;

/*
 * reserve event onto int3 and throw an int3.
 */

void		check_event_message_01(void)
{
  o_event_message	evt;
  t_vsize		sz;
  i_node		sender;

  TEST_ENTER();

  assert(event_reserve(3,
		       EVENT_MESSAGE,
		       EVENT_TASK(ktask),
		       0x42424242) == ERROR_NONE,
	 "cannot event_reserve\n");

  printf(" - Throwing exception\n");
  asm volatile("int $3");

  assert(message_poll(ktask, MESSAGE_TYPE_EVENT, (t_vaddr)&evt, &sz,
		      &sender) == ERROR_NONE, " x Exception not caught\n");

  printf(" + Exception thrown\n");

  assert(evt.id == 3, " x Bad event id in message\n");
  assert(evt.data == 0x42424242, " x Bad pv data in message\n");

  assert(sender.task == 0, " x Bad sender task\n");

  printf(" - Execution continued\n");

  TEST_LEAVE();
}
