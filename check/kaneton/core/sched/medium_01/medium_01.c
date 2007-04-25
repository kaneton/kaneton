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

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

static volatile int executed = 0;

static void thread1(void);

asm ("thread1:				\n"
     "	cli				\n"
     "	addl $1, executed		\n"
     "	sti				\n"
     "1:				\n"
     "	jmp 1b				");

/*
 * XXX
 */

void		check_sched_medium_01(void)
{
  t_id		id;
  t_uint32	start;
  int		i;

  TEST_ENTER();

  CLI();
  for (i = 0; i < 6; i++)
    {
      ASSERT(check_thread_create(ktask, THREAD_PRIOR, (t_vaddr)thread1, &id) == 0,
	     "error creating thread\n");

      ASSERT(thread_state(id, SCHED_STATE_RUN) == ERROR_NONE,
	     "cannot start thread\n");
    }
  STI();

  start = check_cmos_sec();
  while ((start + 3) % 60 != check_cmos_sec())
    ;

  ASSERT(executed == 6, "One or more threads not executed\n")

  TEST_LEAVE();
}
