/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ton/machine/glue/ibm-pc.ia32/scheduler.c
 *
 * created       matthieu bucchianeri   [sat jun  3 22:45:19 2006]
 * updated       julien quintard   [fri jun 22 18:33:55 2007]
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

extern m_scheduler*	scheduler;

extern m_thread*	thread;

extern i_task		ktask;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the address space manager dispatch.
 */

d_scheduler			scheduler_dispatch =
  {
    glue_scheduler_quantum,
    glue_scheduler_yield,
    glue_scheduler_switch,
    NULL,
    NULL,
    NULL,
    glue_scheduler_initialize,
    glue_scheduler_clean
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function sets the scheduler quantum value.
 *
 * just update the timer delay.
 */

t_error			glue_scheduler_quantum(t_quantum	quantum)
{
  SCHEDULER_ENTER(scheduler);

  if (timer_delay(scheduler->machdep.timer, quantum) != ERROR_NONE)
    SCHEDULER_LEAVE(sched, ERROR_UNKNOWN);

  SCHEDULER_LEAVE(scheduler, ERROR_NONE);
}

/*
 * this function calls switch in the good context.
 */

t_error			glue_scheduler_yield(i_cpu			cpuid)
{
  SCHEDULER_ENTER(scheduler);

  // XXX

  SCHEDULER_LEAVE(scheduler, ERROR_NONE);
}


/*
 * this function initializes the scheduler manager.
 *
 * initialize a new timer and fpu exception.
 */

t_error			glue_scheduler_initialize(void)
{
  SCHEDULER_ENTER(scheduler);

  if (timer_reserve(EVENT_FUNCTION,
		    TIMER_HANDLER(scheduler_switch),
		    scheduler->quantum,
		    TIMER_REPEAT_ENABLE,
		    &scheduler->machdep.timer) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  if (event_reserve(7, EVENT_FUNCTION,
		    EVENT_HANDLER(glue_scheduler_switch_extended)) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  SCHEDULER_LEAVE(scheduler, ERROR_NONE);
}

/*
 * this function destroys the scheduler manager.
 *
 * we simply release our timer.
 */

t_error			glue_scheduler_clean(void)
{
  SCHEDULER_ENTER(scheduler);

  if (timer_release(scheduler->machdep.timer) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  if (event_release(7) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  SCHEDULER_LEAVE(scheduler, ERROR_NONE);
}

/*
 * interrupt handler called when an extended context switching is needed.
 */

void			glue_scheduler_switch_extended(i_event	id)
{
  i_cpu			cpuid;
  t_cpu_sched*		ent;

  if (cpu_current(&cpuid) != ERROR_NONE)
    return;

  if (set_get(scheduler->cpus, cpuid, (void**)&ent) != ERROR_NONE)
    return;

  if (ia32_extended_context_switch(scheduler->machdep.mmx_context,
				   ent->current) != ERROR_NONE)
    return;

  scheduler->machdep.mmx_context = ent->current;
}

/*
 * classical context switching.
 */

t_error			glue_scheduler_switch(i_thread		elected)
{
  i_cpu			cpuid;
  t_cpu_sched*		ent;

  SCHEDULER_ENTER(scheduler);

  if (cpu_current(&cpuid) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  if (set_get(scheduler->cpus, cpuid, (void**)&ent) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  if (ia32_context_switch(ent->current, elected) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  SCHEDULER_LEAVE(scheduler, ERROR_NONE);
}
