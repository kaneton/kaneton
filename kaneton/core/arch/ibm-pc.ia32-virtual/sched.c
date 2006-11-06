/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/arch/ibm-pc.ia32-virtual/sched.c
 *
 * created       matthieu bucchianeri   [sat jun  3 22:45:19 2006]
 * updated       matthieu bucchianeri   [sat nov  4 18:22:07 2006]
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
    ia32_sched_yield,
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
 * this function calls switch in the good context.
 */

t_error			ia32_sched_yield(i_cpu			cpuid)
{
  SCHED_ENTER(sched);

  /* XXX int to sched_switch */

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function switches execution to the specified thread.
 *
 * steps:
 *
 * 1) check if a switch is necessary.
 * 2) store back the executing context into the executing thread.
 * 3) set current context as the elected thread one.
 * 4) update the I/O permissions bit map.
 * 5) set the TS flag so any use of FPU, MMX or SSE instruction will
 *    generate an exception.
 */

t_error			ia32_sched_switch(i_thread		elected)
{
  o_thread*		from;
  o_thread*		to;
  o_task*		task;
  i_cpu			cpuid;
  t_cpu_sched*		ent;

  SCHED_ENTER(sched);

  if (!context)
    {
      cons_msg('!', "unable to switch context in this state\n");

      while (1)
	;
    }

  /*
   * 1)
   */

  if (cpu_current(&cpuid) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  if (set_get(sched->cpus, cpuid, (void**)&ent) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  if (ent->current == elected)
      SCHED_LEAVE(sched, ERROR_NONE);

  /*
   * 2)
   */

  if (ent->current != ID_UNUSED)
    {
      if (thread_get(ent->current, &from) != ERROR_NONE)
	SCHED_LEAVE(sched, ERROR_UNKNOWN);

      context_copy(&from->machdep.context, context);
    }

  /*
   * 3)
   */

  if (thread_get(elected, &to) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  context_copy(context, &to->machdep.context);

  /*
   * 4)
   */

  if (task_get(to->taskid, &task) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  memcpy((t_uint8*)thread->machdep.tss + thread->machdep.tss->io,
	 &task->machdep.iomap,
	 8192);

  /*
   * 5)
   */

  STS();

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function is used to switch FPU, MMX and SSE context.
 *
 * steps:
 *
 * 1) get the currently executing thread.
 * 2) SSE case
 *  a) save the SSE & FPU context into the previous thread context.
 *  b) restore the SSE & FPU context of the executing thread.
 * 3) FPU or MMX case
 *  a) save the FPU/MMX context into the previous thread context.
 *  b) restore the FPU/MMX context of the executing thread.
 * 4) clears the TS bit in CR0.
 * 5) store the current thread identifier to save back its context later.
 */

void			ia32_sched_switch_mmx(t_id			id)
{
  i_cpu			cpuid;
  t_cpu_sched*		ent;
  i_thread		thread;
  o_thread*		o;
  o_thread*		old;

  /*
   * 1)
   */

  if (cpu_current(&cpuid) != ERROR_NONE)
    return;

  if (set_get(sched->cpus, cpuid, (void**)&ent) != ERROR_NONE)
    return;

  thread = ent->current;

  if (thread_get(thread, &o) != ERROR_NONE)
    return;

  if (thread_get(sched->machdep.mmx_context, &old) != ERROR_NONE)
    return;

  if (cpucaps & IA32_CAPS_SSE)
    {
      /*
       * 2)
       */

      /*
       * a)
       */

      FXSAVE(old->machdep.u.sse);

      /*
       * b)
       */

      FXRSTOR(o->machdep.u.sse);
    }
  else
    {
      /*
       * 3)
       */

      /*
       * a)
       */

      FSAVE(old->machdep.u.x87);

      /*
       * b)
       */

      FRSTOR(o->machdep.u.x87);
    }

  /*
   * 4)
   */

  CLTS();

  /*
   * 5)
   */

  sched->machdep.mmx_context = thread;
}

/*
 * this function initialises the scheduler manager.
 *
 * initialise a new timer.
 */

t_error			ia32_sched_init(void)
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
