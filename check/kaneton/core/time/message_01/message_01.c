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

extern i_task ktask;

static volatile t_uint32 timed = 0;

/*
 * XXX
 */

void		check_time_message_01_entry(void)
{
  o_timer_message	evt;
  t_vsize		sz;
  i_node		sender;
  i_timer	id;
  t_uint32	start;

  TEST_ENTER();

  start = check_cmos_sec();
  while (start == check_cmos_sec())
    ;

  assert(timer_reserve(TIMER_MESSAGE, TIMER_TASK(ktask),
		       0x41414141,
		       500, TIMER_REPEAT_DISABLE, &id) == ERROR_NONE,
	 "Cannot timer_reserve\n");

  start = check_cmos_sec();
  while ((start + 3) % 60 != check_cmos_sec() && !timed)
    ;

  assert(message_poll(ktask, MESSAGE_TYPE_TIMER, (t_vaddr)&evt, &sz,
		      &sender) == ERROR_NONE, "Timer failed\n");

  assert(evt.id == id, " x Bad event id in message\n");
  assert(evt.data == 0x41414141, " x Bad pv data in message\n");

  assert(sender.task == 0, " x Bad sender task\n");

  TEST_LEAVE();
}

CHECK_WITH_THREAD(check_time_message_01)
