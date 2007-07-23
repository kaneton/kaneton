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

#include <libc.h>
#include <kaneton.h>
#include "../common/common.h"

static volatile int executed1 = 0;
static volatile int executed2 = 0;
static volatile int finished = 0;
static volatile int error = 0;

static void	thread1(void)
{
  char		recv[64];
  i_node	from;
  t_size	recv_sz = 0;
  int		i;

  ASSERT(syscall_message_register(0, 64) == ERROR_NONE,
	 "cannot register message type\n");

  executed1 = 1;

  while (syscall_message_poll(0, recv,  &recv_sz, &from) != ERROR_NONE)
    ;

  for (i = 0; i < 64; i++)
    if (recv[i] != (char)('A' + i))
      error = 1;

  finished = 1;

  while (1)
    ;
}

static void	thread2(void)
{
  i_node        dest;
  char		buff[64];
  int		i;

  ASSERT(syscall_message_register(0, 64) == ERROR_NONE,
	 "cannot register message type\n");

  executed2 = 1;

  while (!executed1)
    ;

  dest.task = 1;

  for (i = 0; i < 64; i++)
    buff[i] = 'A' + i;

  syscall_message_send(dest, 0, buff, 64);

  while (1)
    ;
}

/*
 * XXX
 */

void		check_message_async_01(void)
{
  i_task	tsk1, tsk2;
  t_id		id;
  t_uint32	start;

  TEST_ENTER();

  ASSERT(check_task_create(TASK_CLASS_PROGRAM, &tsk1) == 0,
	"error creating task\n");

  ASSERT(check_thread_create(tsk1, THREAD_HPRIOR, (t_vaddr)thread1, &id) == 0,
	 "error creating thread\n");

  ASSERT(check_task_create(TASK_CLASS_PROGRAM, &tsk2) == 0,
	"error creating task\n");

  ASSERT(check_thread_create(tsk2, THREAD_HPRIOR, (t_vaddr)thread2, &id) == 0,
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

  ASSERT(executed1 == 1 && executed2 == 1, "Thread not executed\n");

  ASSERT(finished == 1, "Messages not exchanged\n");

  ASSERT(error == 0, "Error in exchanged data\n");

  TEST_LEAVE();
}
