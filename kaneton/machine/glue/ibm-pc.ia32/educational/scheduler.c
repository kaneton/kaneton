/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/cry...glue/ibm-pc.ia32/educational/scheduler.c
 *
 * created       matthieu bucchianeri   [sat jun  3 22:45:19 2006]
 * updated       matthieu bucchianeri   [wed jan  9 15:05:33 2008]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_scheduler*	scheduler;

extern m_thread*	thread;

extern i_task		ktask;

extern void		glue_scheduler_idle ();

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the address space manager dispatch.
 */

d_scheduler			scheduler_dispatch =
  {
    glue_scheduler_quantum,
    NULL,
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
 * the idle thread assembly.
 */

asm (".text				\n"
     "glue_scheduler_idle:		\n"
     "1:				\n"
     "	hlt				\n"
     "	jmp 1b				\n"
     ".text				\n");

/*
 * timer handler that calls scheduler_switch().
 */

static void		glue_scheduler_switch_handler(void)
{
  assert(scheduler_switch() == ERROR_NONE);
}

/*
 * this function sets the scheduler quantum value.
 *
 * just update the timer delay.
 *
 */

t_error			glue_scheduler_quantum(t_quantum	quantum)
{
  SCHEDULER_ENTER(scheduler);

  if (timer_delay(scheduler->machdep.timer, quantum) != ERROR_NONE)
    SCHEDULER_LEAVE(sched, ERROR_UNKNOWN);

  SCHEDULER_LEAVE(scheduler, ERROR_NONE);
}

/*
 * this function initializes the scheduler manager.
 *
 * initialize a new timer and fpu exception.
 *
 */

t_error			glue_scheduler_initialize(void)
{
  t_thread_context	ctx;
  t_stack		stack;
  o_thread*		o;

  SCHEDULER_ENTER(scheduler);

  if (timer_reserve(TIMER_FUNCTION,
		    TIMER_HANDLER(glue_scheduler_switch_handler),
		    0,
		    scheduler->quantum,
		    TIMER_REPEAT_ENABLE,
		    &scheduler->machdep.timer) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  if (event_reserve(7, EVENT_FUNCTION,
		    EVENT_HANDLER(glue_scheduler_switch_extended),
		    0) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  if (thread_reserve(ktask, THREAD_PRIOR, &scheduler->idle) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  stack.base = 0;
  stack.size = PAGESZ;

  if (thread_stack(scheduler->idle, stack) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (thread_get(scheduler->idle, &o) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = (t_vaddr)glue_scheduler_idle;

  if (thread_load(scheduler->idle, ctx) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  SCHEDULER_LEAVE(scheduler, ERROR_NONE);
}

/*
 * this function destroys the scheduler manager.
 *
 * we simply release our timer.
 *
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
  o_scheduler*		ent;

  if (cpu_current(&cpuid) != ERROR_NONE)
    return;

  if (set_get(scheduler->cpus, cpuid, (void**)&ent) != ERROR_NONE)
    return;

  if (ia32_extended_context_switch(ent->machdep.mmx_context,
				   ent->current) != ERROR_NONE)
    return;

  ent->machdep.mmx_context = ent->current;
}

/*
 * classical context switching.
 *
 */

t_error			glue_scheduler_switch(i_thread		elected)
{
  i_cpu			cpuid;
  o_scheduler*		ent;

  SCHEDULER_ENTER(scheduler);

  if (cpu_current(&cpuid) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  if (set_get(scheduler->cpus, cpuid, (void**)&ent) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  if (ia32_context_switch(ent->current, elected) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  SCHEDULER_LEAVE(scheduler, ERROR_NONE);
}
