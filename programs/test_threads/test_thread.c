/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/programs/test_threads/test_thread.c
 *
 * created       matthieu bucchianeri   [sat aug 25 18:07:46 2007]
 * updated       matthieu bucchianeri   [sat aug 25 18:19:02 2007]
 */

#include <libc.h>
#include <crt.h>
#include <libkaneton.h>

int		main()
{
  i_thread	th;

  printf(" -- test thread --\n");

  scheduler_current(&th);

  //thread_state(th, SCHEDULER_STATE_STOP);

  task_state(_crt_get_task_id(), SCHEDULER_STATE_STOP);

  printf(" -- bouh!\n");

  return 0;
}
