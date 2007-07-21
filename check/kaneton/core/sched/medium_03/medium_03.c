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

#include <libc.h>
#include <kaneton.h>
#include "../common/common.h"

static volatile unsigned long executed[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

static void thread1(void);
static void thread2(void);
static void thread3(void);
static void thread4(void);
static void thread5(void);
static void thread6(void);
static void thread7(void);
static void thread8(void);

#define THREAD(Id)							\
  asm ("thread" #Id ":				\n"			\
       "1:					\n"			\
       "	cli				\n"			\
       "	pushl %eax			\n"			\
       "	movl $" #Id ", %eax		\n"			\
       "	subl $1, %eax			\n"			\
       "	shll $2, %eax			\n"			\
       "	addl $executed, %eax		\n"			\
       "	addl $1, (%eax)			\n"			\
       "	popl %eax			\n"			\
       "	sti				\n"			\
       "	jmp 1b				");

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

void		check_sched_medium_03(void)
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
      ASSERT(thread_state(id[i], SCHEDULER_STATE_RUN) == ERROR_NONE,
	     "cannot start thread\n");
    }
  STI();

  start = check_cmos_sec();
  while ((start + 3) % 60 != check_cmos_sec())
    ;

  for (i = 0; i < 8; i++)
    ASSERT(executed[i] > 15, "One or more threads not executed\n")

  TEST_LEAVE();
}
