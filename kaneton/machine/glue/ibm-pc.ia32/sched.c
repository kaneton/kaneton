/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...kaneton/machine/glue/ibm-pc.ia32/sched.c
 *
 * created       matthieu bucchianeri   [sat jun  3 22:45:19 2006]
 * updated       matthieu bucchianeri   [sun jun 17 16:44:27 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

#include <architecture/architecture.h>

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
    glue_sched_quantum,
    glue_sched_yield,
    glue_sched_switch,
    NULL,
    NULL,
    NULL,
    glue_sched_init,
    glue_sched_clean
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function sets the scheduler quantum value.
 *
 * just update the timer delay.
 */

t_error			glue_sched_quantum(t_quantum		quantum)
{
  SCHED_ENTER(sched);

  if (timer_delay(sched->machdep.timer, quantum) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function calls switch in the good context.
 */

t_error			glue_sched_yield(i_cpu			cpuid)
{
  SCHED_ENTER(sched);

  // XXX

  SCHED_LEAVE(sched, ERROR_NONE);
}


/*
 * this function initialises the scheduler manager.
 *
 * initialise a new timer and fpu exception.
 */

t_error			glue_sched_init(void)
{
  SCHED_ENTER(sched);

  CLI();

  if (timer_reserve(EVENT_FUNCTION,
		    TIMER_HANDLER(sched_switch),
		    sched->quantum,
		    TIMER_REPEAT_ENABLE,
		    &sched->machdep.timer) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  STI();

  if (event_reserve(7, EVENT_FUNCTION,
		    EVENT_HANDLER(glue_sched_switch_extended)) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function destroys the scheduler manager.
 *
 * we simply release our timer.
 */

t_error			glue_sched_clean(void)
{
  SCHED_ENTER(sched);

  if (timer_release(sched->machdep.timer) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  if (event_release(7) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * interrupt handler called when an extended context switching is needed.
 */

void			glue_sched_switch_extended(i_event	id)
{
  i_cpu			cpuid;
  t_cpu_sched*		ent;

  if (cpu_current(&cpuid) != ERROR_NONE)
    return;

  if (set_get(sched->cpus, cpuid, (void**)&ent) != ERROR_NONE)
    return;

  if (ia32_extended_context_switch(sched->machdep.mmx_context,
				   ent->current) != ERROR_NONE)
    return;

  sched->machdep.mmx_context = ent->current;
}

/*
 * classical context switching.
 */

t_error			glue_sched_switch(i_thread		elected)
{
  i_cpu			cpuid;
  t_cpu_sched*		ent;

  SCHED_ENTER(sched);

  if (cpu_current(&cpuid) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  if (set_get(sched->cpus, cpuid, (void**)&ent) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  if (ia32_context_switch(ent->current, elected) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  SCHED_LEAVE(sched, ERROR_NONE);
}
