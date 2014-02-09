/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...glue/ibm-pc.ia32/educational/scheduler.c
 *
 * created       matthieu bucchianeri   [sat jun  3 22:45:19 2006]
 * updated       julien quintard   [tue feb 15 21:03:52 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the scheduler manager's glue.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the scheduler manager.
 */

extern m_scheduler	_scheduler;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the scheduler dispatcher.
 */

d_scheduler		glue_scheduler_dispatch =
  {
    NULL,
    glue_scheduler_dump,
    glue_scheduler_start,
    glue_scheduler_stop,
    glue_scheduler_quantum,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function dumps the scheduler manager's machine part.
 */

t_status			glue_scheduler_dump(void)
{
  module_call(console, message,
	      '#',
	      "  machine: timer(%qd)\n",
	      _scheduler.machine.timer);

  MACHINE_LEAVE();
}

/*
 * this function starts the scheduler.
 *
 * steps:
 *
 * 1) reserve a timer so that a context switch is triggered every quantum
 *    milliseconds.
 */

t_status                 glue_scheduler_start(i_cpu              cpu)
{
  /*
   * 1)
   */

  if (timer_reserve(TIMER_TYPE_FUNCTION,
                    TIMER_ROUTINE(glue_scheduler_switch),
                    TIMER_DATA(NULL),
                    _scheduler.quantum,
                    TIMER_OPTION_REPEAT,
                    &_scheduler.machine.timer) != STATUS_OK)
    MACHINE_ESCAPE("unable to reserve the timer");

  MACHINE_LEAVE();
}

/*
 * this function contributes to stopping the scheduler by manually triggering
 * the timer interrupt in order to induce an immediate scheduler election.
 *
 * during this election, the scheduler will notice the scheduler's state
 * has changed to stop and will do the necessary to stop electing threads.
 *
 * 1) retrieve the current CPU's scheduler.
 * 2) if the CPU to stop is the current one, yield the execution in order
 *    to induce a scheduler election.
 * 3) release the scheduler timer.
 */

t_status			glue_scheduler_stop(i_cpu		cpu)
{
  o_scheduler*		scheduler;

  /*
   * 1)
   */

  if (scheduler_current(&scheduler) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the current scheduler");

  /*
   * 2)
   */

  if (scheduler->cpu == cpu)
    {
      if (scheduler_yield() != STATUS_OK)
	MACHINE_ESCAPE("unable to yield the execution");
    }

  /*
   * 3)
   */

  if (timer_release(_scheduler.machine.timer) != STATUS_OK)
    MACHINE_ESCAPE("unable to release the timer");

  MACHINE_LEAVE();
}

/*
 * this handler is triggered whenever the scheduler quantum of time has
 * elapsed. this function requests the scheduler to elect a new thread
 * and performs the context switch.
 *
 * steps:
 *
 * 1) retrieve the current CPU's scheduler.
 * 2) save the identifier of the currently running thread.
 * 3) request a thread election to the scheduler.
 * 4) save the identifier of the thread about to be executed.
 * 5) perform a context switch between the two threads.
 */

void			glue_scheduler_switch(void)
{
  o_scheduler*		scheduler;
  i_thread		current;
  i_thread		future;

  /*
   * 1)
   */

  assert(scheduler_current(&scheduler) == STATUS_OK);

  /*
   * 2)
   */

  current = scheduler->thread;

  /*
   * 3)
   */

  assert(scheduler_elect() == STATUS_OK);

  /*
   * 4)
   */

  future = scheduler->thread;

  /*
   * 5)
   */

  assert(architecture_context_switch(current, future) == STATUS_OK);
}

/*
 * this function sets the scheduler quantum value.
 *
 * steps:
 *
 * 1) note that since the quantum has been updated, the scheduler timer
 *    must also be adujsted.
 */

t_status			glue_scheduler_quantum(t_quantum	quantum)
{
  /*
   * 1)
   */

  if (timer_update(_scheduler.machine.timer, quantum) != STATUS_OK)
    MACHINE_ESCAPE("unable to adjust the timer's delay to the "
		   "scheduler's quantum");

  MACHINE_LEAVE();
}
