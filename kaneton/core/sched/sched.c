/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/sched/sched.c
 *
 * created       matthieu bucchianeri   [sat jun  3 22:36:59 2006]
 * updated       julien quintard   [sat jul  8 02:24:01 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

machdep_include(sched);

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * scheduler manager variable.
 */

m_sched*		sched = NULL;

/*
 * kernel task id.
 */

extern i_task		ktask;

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                 [cut] /k5 */

/*
 * dump the scheduler state.
 *
 * steps:
 *
 * 1) dump the current thread.
 * 2) dump all the schedule queue.
 */

t_error			sched_dump(void)
{
  t_iterator		i;
  t_state		st;
  i_thread*		thread;

  SCHED_ENTER(sched);

  /*
   * 1)
   */

  cons_msg('#', "scheduler current thread: %qd\n", sched->current);

  /*
   * 2)
   */

  cons_msg('#', "scheduler queue:");

  set_foreach(SET_OPT_FORWARD, sched->threads, &i, st)
    {
      if (set_object(sched->threads, i, (void**)&thread) != ERROR_NONE)
	SCHED_LEAVE(sched, ERROR_UNKNOWN);

      printf(" %qd", *thread);
    }

  printf("\n");

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function sets the scheduler quantum value.
 *
 * steps:
 *
 * 1) set the new quantum value.
 * 2) call the architecture dependent code.
 */

t_error			sched_quantum(t_quantum			quantum)
{
  SCHED_ENTER(sched);

  /*
   * 1)
   */

  sched->quantum = quantum;

  /*
   * 2)
   */

  if (machdep_call(sched, sched_quantum, quantum) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function permits the current task to relinquish the processor
 * voluntary.
 *
 * steps:
 *
 * 1) switch to the next thread.
 * 2) call the dependent code.
 */

t_error			sched_yield(void)
{
  SCHED_ENTER(sched);

  /*
   * 1)
   */

  if (sched_switch() != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (machdep_call(sched, sched_yield) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this  function returns  the  identifier of  the currently  executed
 * thread.
 */

t_error			sched_current(i_thread*			thread)
{
  SCHED_ENTER(sched);

  *thread = sched->current;

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function switches execution to the specified thread.
 *
 * steps:
 *
 * 1) get the next thread.
 * 2) check if the  currently scheduled thread is the  same as the one
 *    we are asked to switch to.
 * 3) call  the  machine  dependent   code  (which  does  the  context
 *    switching).
 * 4) set the current thread.
 * 5) push the newly executing thread back into the schedule list.
 */

t_error			sched_switch(void)
{
  t_iterator		it;
  i_thread*		thread;

  SCHED_ENTER(sched);

  /*
   * 1)
   */

  if (set_head(sched->threads, &it) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  if (set_object(sched->threads, it, (void**)&thread) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (sched->current == *thread)
    SCHED_LEAVE(sched, ERROR_NONE);

  /*
   * 3)
   */

  if (machdep_call(sched, sched_switch, *thread) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  /*
   * 4)
   */

  sched->current = *thread;

  /*
   * 5)
   */

  /* XXX push the thread back in the schedule list */

  if (sched_update(*thread) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function adds a thread to the scheduler.
 *
 * steps:
 *
 * 1) add the thread to the schedule list.
 * 2) update the thread's priority.
 * 3) call the machine dependent code.
 */

t_error			sched_add(i_thread			thread)
{
  SCHED_ENTER(sched);

  /*
   * 1)
   */

  if (set_add(sched->threads, &thread) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (sched_update(thread) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (machdep_call(sched, sched_add, thread) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function removes a thread from the scheduler.
 *
 * steps:
 *
 * 1) if the  currently scheduled thread  is being removed,  then stop
 *    its execution.
 * 2) remove from the schedule list.
 * 3) call architecture dependent code.
 */

t_error			sched_remove(i_thread			thread)
{
  SCHED_ENTER(sched);

  /*
   * 1)
   */

  if (sched->current == thread)
    if (sched_yield() != ERROR_NONE)
      SCHED_LEAVE(sched, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (set_remove(sched->threads, thread) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (machdep_call(sched, sched_remove, thread) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function updates a thread into the scheduler.
 *
 * steps:
 *
 * x) call the machine dependent code.
 */

t_error			sched_update(i_thread			thread)
{
  o_thread*		oth;
  o_task*		otsk;
  t_prior		task_prior;
  t_prior		thread_prior;

  SCHED_ENTER(sched);

  /*
   * 1)
   */

  if (thread_get(thread, &oth) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  if (task_get(oth->taskid, &otsk) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  task_prior = ((otsk->prior - TASK_LPRIOR_BACKGROUND) * 100) /
    (TASK_HPRIOR_CORE - TASK_LPRIOR_CORE);

  thread_prior = ((oth->prior - THREAD_LPRIOR) * 100) /
    (THREAD_HPRIOR - THREAD_LPRIOR);

  /* XXX */

  /*
   * x)
   */

  if (machdep_call(sched, sched_update, thread) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function initialises the scheduler manager.
 *
 * steps:
 *
 * 1) allocate and initialises the scheduler manager structure.
 * 2) try to reserve a statistics object.
 * 3) create the scheduled thread list.
 * 4) create the kernel thread.
 * 5) call the machine-dependent code.
 */

t_error			sched_init(void)
{

  /*
   * 1)
   */

  if ((sched = malloc(sizeof(m_sched))) == NULL)
    {
      cons_msg('!', "sched: cannot allocate memory for the scheduler "
	       "manager structure\n");

      return (ERROR_UNKNOWN);
    }

  memset(sched, 0x0, sizeof(m_sched));

  sched->current = ID_UNUSED;
  sched->quantum = SCHED_QUANTUM_INIT;
  sched->threads = ID_UNUSED;

  /*
   * 2)
   */

  STATS_RESERVE("sched", &sched->stats);

  /*
   * 3)
   */

  if (set_reserve(ll, SET_OPT_ALLOC, sizeof(i_thread), &sched->threads) !=
      ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (thread_reserve(ktask, &sched->current) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (sched_add(sched->current) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 5)
   */

  if (machdep_call(sched, sched_init) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}

/*
 * this function just reinitialises the scheduler manager.
 *
 * steps:
 *
 * 1) call the machine-dependent code.
 * 2) release the stats object.
 * 3) free the scheduler manager structure's memory.
 */

t_error			sched_clean(void)
{
  /*
   * 1)
   */

  if (machdep_call(sched, sched_clean) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (set_release(sched->threads) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  /*
   * 3)
   */

  STATS_RELEASE(sched->stats);

  /*
   * 4)
   */

  free(sched);

  return (ERROR_NONE);
}

/*                                                                 [cut] /k5 */
