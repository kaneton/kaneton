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

static volatile int executed[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

#define THREAD(Id)							\
  static void	thread##Id(void)					\
  {									\
    asm volatile("cli");						\
    executed[(Id) - 1] = 1;						\
    asm volatile("sti");						\
									\
    while (1)								\
      ;									\
  }

THREAD(1);
THREAD(2);
THREAD(3);
THREAD(4);
THREAD(5);
THREAD(6);
THREAD(7);
THREAD(8);

/*
 * XXX
 */

void		check_sched_medium_02(void)
{
  t_id		id[8];
  t_uint32	start;
  int		i;

  TEST_ENTER();

  CLI();
  ASSERT(check_thread_create(ktask, THREAD_PRIOR, (t_vaddr)thread1, &id[0]) == 0,
	 "error creating thread\n");
  ASSERT(check_thread_create(ktask, THREAD_PRIOR, (t_vaddr)thread2, &id[1]) == 0,
	 "error creating thread\n");
  ASSERT(check_thread_create(ktask, THREAD_PRIOR, (t_vaddr)thread3, &id[2]) == 0,
	 "error creating thread\n");
  ASSERT(check_thread_create(ktask, THREAD_PRIOR, (t_vaddr)thread4, &id[3]) == 0,
	 "error creating thread\n");
  ASSERT(check_thread_create(ktask, THREAD_PRIOR, (t_vaddr)thread5, &id[4]) == 0,
	 "error creating thread\n");
  ASSERT(check_thread_create(ktask, THREAD_PRIOR, (t_vaddr)thread6, &id[5]) == 0,
	 "error creating thread\n");
  ASSERT(check_thread_create(ktask, THREAD_PRIOR, (t_vaddr)thread7, &id[6]) == 0,
	 "error creating thread\n");
  ASSERT(check_thread_create(ktask, THREAD_PRIOR, (t_vaddr)thread8, &id[7]) == 0,
	 "error creating thread\n");

  for (i = 0; i < 8; i++)
    {
      ASSERT(thread_state(id[i], SCHED_STATE_RUN) == ERROR_NONE,
	     "cannot start thread\n");
    }
  STI();

  start = check_cmos_sec();
  while ((start + 3) % 60 != check_cmos_sec())
    ;

  for (i = 0; i < 8; i++)
    ASSERT(executed[i] == 1, "One or more threads not executed\n")

  TEST_LEAVE();
}
