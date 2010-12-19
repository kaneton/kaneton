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
 * updated       julien quintard   [sun dec 19 18:09:23 2010]
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

#include <architecture/architecture.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the kernel manager.
 */

extern m_kernel*	_kernel;

/*
 * the scheduler manager.
 */

extern m_scheduler*	_scheduler;

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
    NULL,
    glue_scheduler_stop,
    glue_scheduler_quantum,
    glue_scheduler_yield,
    NULL,
    NULL,
    NULL,
    NULL,
    glue_scheduler_initialize,
    glue_scheduler_clean
  };

/*
 * ---------- prototype -------------------------------------------------------
 */

extern void		glue_scheduler_idle ();

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
 * XXX
 */

t_error			glue_scheduler_dump(void)
{
  module_call(console, message,
	      '#',
	      "  machine: timer(%qu)\n",
	      _scheduler->machine.timer);

  MACHINE_LEAVE();
}

/*
 * this function manually triggers the timer interrupt in order to
 * induce an immediate scheduler election.
 */

t_error			glue_scheduler_stop(i_cpu		cpu)
{
  o_scheduler*		scheduler;

  /*
   * XXX
   */

  if (scheduler_current(&scheduler) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the current scheduler");

  /*
   * XXX
   */

  if (cpu == scheduler->cpu)
    {
      if (scheduler_yield() != ERROR_OK)
	MACHINE_ESCAPE("unable to yield the execution");
    }

  MACHINE_LEAVE();
}

/*
 * this function represents the timer handler which performs the
 * election and context switch.
 */

void			glue_scheduler_switch_handler(void)
{
  o_scheduler*		scheduler;
  i_thread		current;
  i_thread		future;

  /*
   * XXX
   */

  assert(scheduler_current(&scheduler) == ERROR_OK);

  current = scheduler->thread;

  /*
   * XXX
   */

  assert(scheduler_elect() == ERROR_OK);

  future = scheduler->thread;

  /*
   * XXX
   */

  assert(ia32_context_switch(current, future) == ERROR_OK);
}

/*
 * this function sets the scheduler quantum value.
 *
 * note that since the quantum has been updated, the scheduler timer
 * must be adujsted
 */

t_error			glue_scheduler_quantum(t_quantum	quantum)
{
  /*
   * XXX
   */

  if (timer_update(_scheduler->machine.timer, quantum) != ERROR_OK)
    MACHINE_ESCAPE("unable to adjust the timer's delay to the "
		   "scheduler's quantum");

  MACHINE_LEAVE();
}

/*
 * this function yields the execution by manually triggering the
 * timer hardware interrupt, hence inducing a scheduler election.
 */

t_error			glue_scheduler_yield(void)
{
  /*
   * XXX
   */

  asm volatile ("int $32");

  MACHINE_LEAVE();
}

/*
 * this function initializes the scheduler manager's glue.
 *
 * XXX
 */

t_error			glue_scheduler_initialize(void)
{
  s_thread_context	ctx;
  s_stack		stack;
  o_thread*		o;

  /*
   * XXX
   */

  if (timer_reserve(TIMER_TYPE_FUNCTION,
		    TIMER_ROUTINE(glue_scheduler_switch_handler),
		    TIMER_DATA(NULL),
		    _scheduler->quantum,
		    TIMER_OPTION_REPEAT,
		    &_scheduler->machine.timer) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve the timer");

  /*
   * XXX
   */

  if (event_reserve(7, EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(glue_scheduler_switch_extended),
		    0) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve the event assocated with the timer "
		   "hardware interrupt");

  /*
   * XXX
   */

  // XXX ici on fait en sorte que la priorite ne soit pas la plus faible pour
  // que le yield fonctionne mais qu'elle soit assez basse tout de meme.
  //
  // on pourrait faire une macro opur la priorite avec une fonction math.
  if (thread_reserve(_kernel->task,
		     THREAD_PRIORITY_LOW + 10,
		     &_scheduler->idle) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve the idle thread");

  /*
   * XXX
   */

  stack.base = 0;
  stack.size = ___kaneton$pagesz;

  if (thread_stack(_scheduler->idle, stack) != ERROR_OK)
    MACHINE_ESCAPE("unable to set the stack for the thread");

  /*
   * XXX
   */

  if (thread_get(_scheduler->idle, &o) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the thread object");

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = (t_vaddr)glue_scheduler_idle;

  if (thread_load(_scheduler->idle, ctx) != ERROR_OK)
    MACHINE_ESCAPE("unable to load the thread context");

  /*
   * XXX
   */

  if (thread_start(_scheduler->idle) != ERROR_OK)
    MACHINE_ESCAPE("unable to set the thread as running");

  MACHINE_LEAVE();
}

/*
 * this function cleans the scheduler manager's glue.
 *
 * XXX
 */

t_error			glue_scheduler_clean(void)
{
  /*
   * XXX
   */

  if (timer_release(_scheduler->machine.timer) != ERROR_OK)
    MACHINE_ESCAPE("unable to release the timer");

  /*
   * XXX
   */

  if (event_release(7) != ERROR_OK)
    MACHINE_ESCAPE("unable to release the timer event");

  MACHINE_LEAVE();
}
