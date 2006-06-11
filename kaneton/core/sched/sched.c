/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/sched/sched.c
 *
 * created       matthieu bucchianeri   [sat jun  3 22:36:59 2006]
 * updated       matthieu bucchianeri   [sun jun 11 19:19:54 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 */

/*
 * ---------- assignments -----------------------------------------------------
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
 * ---------- functions -------------------------------------------------------
 */

/*                                                                 [cut] /k5 */

/*
 * this function sets the scheduler quantum value.
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
 */

t_error			sched_yield(void)
{
  SCHED_ENTER(sched);

  /* XXX */

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this  function returns  the  identifier of  the currently  executed
 * thread.
 */

t_error			sched_current(i_thread*			thread)
{
  SCHED_ENTER(sched);

  /* XXX */

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function returns the identifier of the next thread to execute.
 */

t_error			sched_next(i_thread*			thread)
{
  SCHED_ENTER(sched);

  /* XXX */

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function switches execution to the specified thread.
 */

t_error			sched_switch(i_thread			thread)
{
  SCHED_ENTER(sched);

  /* XXX */

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function adds a thread to the scheduler.
 */

t_error			sched_add(i_thread			thread)
{
  SCHED_ENTER(sched);

  /* XXX */

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function removes a thread from the scheduler.
 */

t_error			sched_remove(i_thread			thread)
{
  SCHED_ENTER(sched);

  /* XXX */

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function updates a thread into the scheduler.
 */

t_error			sched_update(i_thread			thread)
{
  SCHED_ENTER(sched);

  /* XXX */

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function initialises the scheduler manager.
 *
 * steps:
 *
 * 1) allocates and initialises the scheduler manager structure.
 * 2) tries to reserve a statistics object.
 * 3) calls the machine-dependent code.
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

  /*
   * 2)
   */

  STATS_RESERVE("sched", &sched->stats);

  /*
   * 3)
   */

  if (machdep_call(sched, sched_init) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  return (ERROR_NONE);

}

/*
 * this function just reinitialises the scheduler manager.
 *
 * steps:
 *
 * 1) calls the machine-dependent code.
 * 2) releases the stats object.
 * 3) frees the scheduler manager structure's memory.
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

  STATS_RELEASE(sched->stats);

  /*
   * 3)
   */

  free(sched);

  return (ERROR_NONE);

}

/*                                                                 [cut] /k5 */
