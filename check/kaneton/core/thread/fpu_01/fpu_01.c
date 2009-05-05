/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/sched/context_02/context_02.c
 *
 * created       matthieu bucchianeri   [sun apr  2 19:04:14 2006]
 * updated       matthieu bucchianeri   [sun may 27 21:26:09 2007]
 */

#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

static volatile int executed1 = 0;
static volatile int executed2 = 0;
static volatile int invalid = 0;


static struct
{
  t_uint16	fcw;
  t_uint16	reserved1;
  t_uint16	fsw;
  t_uint16	reserved2;
  t_uint16	ftw;
  t_uint16	reserved3;
  t_uint32	fpu_eip;
  t_uint16	fpu_cs;
  t_uint32	opcode:11;
  t_uint32	reserved4:5;
  t_uint32	fpu_ptr;
  t_uint16	fpu_ds;
  t_uint16	reserved5;
  t_uint8	st0[10];
  t_uint8	st1[10];
  t_uint8	st2[10];
  t_uint8	st3[10];
  t_uint8	st4[10];
  t_uint8	st5[10];
  t_uint8	st6[10];
  t_uint8	st7[10];
}		__attribute__ ((packed)) regs1, regs2;

static void thread1(void)
{
  volatile double a, b, c, d, e, f;

  executed1 = 1;

  CLI();
  /* dummy operations */
  a = 0.2 * 1.5;
  b = a * 6.7;
  c = a * b * 12334656.0;
  d = 1.6 * 2.4;
  e = a * b * c * d;
  f = a * e;
  asm volatile ("fsave %0"
		:
		: "m" (regs1));
  /* ok this is stupid but fsave reset FPU */
  asm volatile ("frstor %0"
		:
		: "m" (regs1));
  STI();

  while (1)
    {
      asm volatile ("fsave %0"
		    :
		    : "m" (regs2));
      asm volatile ("frstor %0"
		    :
		    : "m" (regs2));
      if (memcmp(&regs1, &regs2, sizeof (regs1)))
	invalid = 1;
    }
}

static void thread2(void)
{
  volatile double a, b, c, d, e, f;

  executed2 = 1;

  while (1)
    {
      /* fuck up the FPU state */
      a = 5.6 * 4.5 + 2.0 / 4.5;
      b = a - 6.7;
      c = a * b;
      d = 3.4 + 6.7;
      e = a * b * c * d;
      f = 0.0;
    }
}


/*
 * XXX
 */

void		check_thread_fpu_01_entry(void)
{
  t_id		id;
  t_uint32	start;
  i_task	tsk;

  TEST_ENTER();

  memset(&regs1, 0, sizeof (regs1));
  memset(&regs2, 0, sizeof (regs2));

  CLI();
  /*  assert(check_task_create(TASK_CLASS_GUEST, &tsk) == 0,
      "error creating task\n");*/
  tsk = ktask;

  assert(check_thread_create(tsk, THREAD_PRIOR, (t_vaddr)thread1, &id) == 0,
	 "error creating thread\n");

  assert(check_thread_create(tsk, THREAD_PRIOR, (t_vaddr)thread2, &id) == 0,
	 "error creating thread\n");

  assert(task_state(tsk, SCHEDULER_STATE_RUN) == ERROR_NONE,
	 "cannot start task\n");
  STI();

  start = check_cmos_sec();
  while ((start + 3) % 60 != check_cmos_sec() && !invalid)
    ;

  assert(executed1 == 1 && executed2 == 1, "Thread not executed\n");

  assert(invalid == 0, "Error in context switching, "
	"some FPU registers are not preserved\n");

  TEST_LEAVE();
}

CHECK_WITH_THREAD(check_thread_fpu_01)
