/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/arch/ia32-virtual/sched.c
 *
 * created       matthieu bucchianeri   [sat jun  3 22:45:19 2006]
 * updated       julien quintard   [sat jul  8 02:30:37 2006]
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
 */

t_error			ia32_sched_switch(i_thread		thread)
{
  SCHED_ENTER(sched);

  /* XXX */

  printf("switching from %qd to %qd\n", sched->current, thread);

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

  if (timer_reserve(ktask, sched->quantum, TIMER_REPEAT_ENABLE,
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
