/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/arch/ia32-virtual/sched.c
 *
 * created       matthieu bucchianeri   [sat jun  3 22:45:19 2006]
 * updated       matthieu bucchianeri   [fri aug  4 17:58:58 2006]
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

/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_sched*		sched;

extern m_thread*	thread;

extern i_task		ktask;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the address space manager dispatch.
 */

d_sched			sched_dispatch =
  {

/*                                                                  [cut] k5 */

    ia32_sched_quantum,
    NULL,
    ia32_sched_switch,
    NULL,
    NULL,
    NULL,
    ia32_sched_init,
    ia32_sched_clean

/*                                                                 [cut] /k5 */

  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k5 */

/*
 * this function sets the scheduler quantum value.
 *
 * just update the timer delay.
 */

t_error			ia32_sched_quantum(t_quantum		quantum)
{
  SCHED_ENTER(sched);

  if (timer_delay(sched->machdep.timer, quantum) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function switches execution to the specified thread.
 *
 * steps:
 *
 * 1) store back the executing context into the executing thread.
 * 2) set current context as the elected thread one.
 * 3) update the I/O permissions bit map.
 */

t_error			ia32_sched_switch(i_thread		elected)
{
  o_thread*		from;
  o_thread*		to;
  o_task*		task;

  SCHED_ENTER(sched);

  if (!context)
    {
      cons_msg('!', "unable to switch context in this state\n");

      while (1)
	;
    }

  if (sched->current == elected)
      SCHED_LEAVE(sched, ERROR_NONE);

  /*
   * 1)
   */

  if (thread_get(sched->current, &from) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  context_copy(&from->machdep.context, context);
  if (cpucaps & IA32_CAPS_SSE)
    memcpy(&from->machdep.u.sse, SSE_STATE(), sizeof(t_sse_state));
  else
    memcpy(&from->machdep.u.x87, X87_STATE(), sizeof(t_x87_state));

  /*
   * 2)
   */

  if (thread_get(elected, &to) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  context_copy(context, &to->machdep.context);
  if (cpucaps & IA32_CAPS_SSE)
    memcpy(SSE_STATE(), &to->machdep.u.sse, sizeof(t_sse_state));
  else
    memcpy(X87_STATE(), &to->machdep.u.x87, sizeof(t_x87_state));

  /*
   * 3)
   */

  if (task_get(to->taskid, &task) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  memcpy((t_uint8*)thread->machdep.tss + thread->machdep.tss->io,
	 &task->machdep.iomap,
	 8192);

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function initialises the scheduler manager.
 *
 * initialise a new timer.
 */

t_error			ia32_sched_init(void)
{
  SCHED_ENTER(sched);

  if (timer_reserve(EVENT_FUNCTION,
		    (u_timer_handler)((t_timer_handler)sched_switch),
		    sched->quantum, TIMER_REPEAT_ENABLE,
		    &sched->machdep.timer) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function destroys the scheduler manager.
 *
 * we simply release our timer.
 */

t_error			ia32_sched_clean(void)
{
  SCHED_ENTER(sched);

  if (timer_release(sched->machdep.timer) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*                                                                 [cut] /k5 */
