/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/k4_tests/kaneton/check/kaneton/core/message/async_01u/async_01u.c
 *
 * created       matthieu bucchianeri   [sun apr  2 19:04:14 2006]
 * updated       matthieu bucchianeri   [sun jun 10 20:05:51 2007]
 */

#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

static volatile int executed1 = 0;
static volatile int executed2 = 0;
static volatile int finished = 0;
static volatile int error = 0;
static i_task destt;
static i_task source;
static t_id machine;

static void	thread1(void)
{
  static char	recv[64];
  i_node	from;
  t_size	recv_sz = 0;

  executed1 = 1;

  while (syscall_message_poll(0, (t_vaddr)recv,  &recv_sz,
			      &from) != ERROR_NONE)
    ;

  if (recv_sz != 64 || from.task != source)
    {
      printf("recvsz = %u ( != %u ); sender.task = %qd ( != %qd )\n",
	     recv_sz, 64, from.task, source);
      error = 1;
    }

  finished = 1;

  while (1)
    ;
}

static void	thread2(void)
{
  i_node        dest;
  static char	buff[64];
  int		i;

  executed2 = 1;

  dest.machine = machine;
  dest.task = destt;

  for (i = 0; i < 64; i++)
    buff[i] = i;

  syscall_message_send(dest, 0, (t_vaddr)buff, 64);

  while (1)
    ;
}

/*
 * XXX
 */

void		check_message_return_01(void)
{
  i_task	tsk1, tsk2;
  t_id		id;
  t_uint32	start;

  TEST_ENTER();

  ASSERT(check_task_create(TASK_CLASS_PROGRAM, &tsk1) == 0,
	"error creating task\n");

  ASSERT(message_register(tsk1, 0, 3000) == ERROR_NONE,
	 "cannot register message type\n");

  machine = kernel->machine;
  destt = tsk1;

  ASSERT(check_thread_create(tsk1, THREAD_HPRIOR, (t_vaddr)thread1, &id) == 0,
	 "error creating thread\n");

  ASSERT(check_task_create(TASK_CLASS_PROGRAM, &tsk2) == 0,
	"error creating task\n");

  ASSERT(message_register(tsk2, 0, 3000) == ERROR_NONE,
	 "cannot register message type\n");

  source = tsk2;

  ASSERT(check_thread_create(tsk2, THREAD_PRIOR, (t_vaddr)thread2, &id) == 0,
	 "error creating thread\n");

  CLI();
  ASSERT(task_state(tsk1, SCHEDULER_STATE_RUN) == ERROR_NONE,
	 "cannot start task\n");
  ASSERT(task_state(tsk2, SCHEDULER_STATE_RUN) == ERROR_NONE,
	 "cannot start task\n");
  STI();

  start = check_cmos_sec();
  while ((start + 3) % 60 != check_cmos_sec() && !finished && !error)
    ;

  CLI();

  ASSERT(executed1 == 1 && executed2 == 1, "Thread not executed\n");

  ASSERT(finished == 1, "Messages not exchanged\n");

  ASSERT(error == 0, "Invalid sender or size\n");

  task_state(tsk1, SCHEDULER_STATE_STOP);
  task_state(tsk2, SCHEDULER_STATE_STOP);

  task_release(tsk1);
  task_release(tsk2);

  STI();

  TEST_LEAVE();
}
