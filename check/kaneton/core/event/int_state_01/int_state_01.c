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

static volatile int	thrown = 0;

static void	check_int3(t_id	id)
{
  t_uint32	eflags;

  asm volatile("	pushf		\n"
	       "	popl %0		"
	       : "=g" (eflags));

  printf("Interrupt %s in ISR\n", (eflags & (1 << 9) ? "enabled" : "disabled"));
  thrown = 1;
}

/*
 * reserve event onto int3 and throw an int3.
 */

void		check_event_int_state_01_entry(void)
{
  static t_uint32	eflags;

  TEST_ENTER();

  assert(event_reserve(3,
		       EVENT_FUNCTION,
		       EVENT_HANDLER(check_int3), 0) == ERROR_NONE,
	 "cannot event_reserve\n");

  asm volatile("int $3");
  assert(thrown == 1, " x Exception not caught\n");

  asm volatile("	pushf		\n"
	       "	popl %0		"
	       : "=g" (eflags));

  printf("Interrupt %s after ISR\n", (eflags & (1 << 9) ? "enabled" : "disabled"));

  TEST_LEAVE();
}

CHECK_WITH_THREAD(check_event_int_state_01)
