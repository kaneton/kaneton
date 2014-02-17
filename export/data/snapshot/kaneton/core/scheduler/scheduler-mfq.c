/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/Down...n/kaneton/core/scheduler/scheduler-mfq.c
 *
 * created       matthieu bucchianeri   [sat jun  3 22:36:59 2006]
 * updated       julien quintard   [mon apr 11 13:23:09 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the scheduler manager provides functionalities for managing the
 * execution of the tasks and their threads on the possibly multiple CPUs
 * of the computer.
 *
 * note that tasks are not scheduled. indeed, the active entity is the
 * thread. therefore, adding a thread to the scheduler makes it a candidate
 * for future election.
 *
 * the core function is the scheduler_elect() function which chooses
 * the next thread to execute, though the current thread may be selected
 * to continue its execution.
 */

#if (SCHEDULER_ALGORITHM == SCHEDULER_ALGORITHM_MFQ)

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * include the machine-specific definitions required by the core.
 */

machine_include(scheduler);

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the kernel manager.
 */

extern m_kernel		_kernel;

/*
 * the cpu manager.
 */

extern m_cpu		_cpu;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the scheduler manager.
 */

m_scheduler		_scheduler;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function starts the given scheduler.
 *
 * steps:
 *
 * 1) retrieve the scheduler object.
 * 2) change the scheduler's state.
 * 3) call the machine.
 */

t_status			scheduler_start(i_cpu			id)
{
  o_scheduler*		scheduler;

  /*
   * 1)
   */

  if (scheduler_get(id, &scheduler) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the scheduler object");

  /*
   * 2)
   */

  scheduler->state = SCHEDULER_STATE_START;

  /*
   * 3)
   */

  if (machine_call(scheduler, start, id) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function stops the given scheduler.
 *
 * steps:
 *
 * 1) retrieve the scheduler object.
 * 2) change the scheduler's state.
 * 3) call the machine.
 */

t_status			scheduler_stop(i_cpu			id)
{
  o_scheduler*		scheduler;

  /*
   * 1)
   */

  if (scheduler_get(id, &scheduler) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the scheduler object");

  /*
   * 2)
   */

  scheduler->state = SCHEDULER_STATE_STOP;

  /*
   * 3)
   */

  if (machine_call(scheduler, stop, id) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function modifies the quantum.
 */

t_status			scheduler_quantum(t_quantum		quantum)
{
  /* FIXME[code to complete] */

  CORE_LEAVE();
}

/*
 * this function enables the current thread to voluntarily relinquish its
 * execution, hence permitting another thread to be scheduled immediately
 * on this CPU.
 */

t_status			scheduler_yield(void)
{
  /* FIXME[code to complete] */

  CORE_LEAVE();
}

/*
 * this function elects the future thread to execute, taking care to
 * save the currently executing one, should both the task and thread be
 * still in a running state.
 *
 * note that this function may elect the already running thread if
 * (i) it has not expired and (ii) there is no thread with a higher priority
 * in the active list.
 *
 * finally, this function may detect that the scheduler has been stopped.
 * should this occur, the elected thread is saved and the original kernel
 * thread is specially scheduled, hence returning to its initial state.
 */

t_status			scheduler_elect(void)
{
  /* FIXME[code to complete] */

  CORE_LEAVE();
}

/*
 * this function adds a thread to the scheduler.
 */

t_status			scheduler_add(i_thread			id)
{
  /* FIXME[code to complete] */

  CORE_LEAVE();
}

/*
 * this function removes a thread from the scheduler.
 */

t_status			scheduler_remove(i_thread		id)
{
  /* FIXME[code to complete] */

  CORE_LEAVE();
}

/*
 * this function updates a thread from the scheduler after its priority
 * has changed.
 *
 * note that the easiest way could seem to be to remove then add the
 * thread, in which case it would be added to its proper queue depending
 * on its new priority. unfortunately this solution would make the thread
 * lose its remaining timeslices. besides, if the thread to remove is the
 * currently scheduled thread, removing it would incurr a yield. in this
 * case, the thread would no longer be scheduled and would therefore not
 * have the chance to add itself back to the scheduler.
 */

t_status			scheduler_update(i_thread		id)
{
  /* FIXME[code to complete] */

  CORE_LEAVE();
}

/*
 * this function returns true if the scheduler for the given CPU identifier
 * exists.
 */

t_status			scheduler_exist(i_cpu			id)
{
  if (set_exist(_scheduler.schedulers, id) != TRUE)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function retrives the scheduler for the given CPU.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the object from the set of schedulers.
 */

t_status			scheduler_get(i_cpu			id,
				      o_scheduler**		object)
{
  /*
   * 0)
   */

  if (object == NULL)
    CORE_ESCAPE("the 'object' argument is null");

  /*
   * 1)
   */

  if (set_get(_scheduler.schedulers, id, (void**)object) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the object from the set of schedulers");

  CORE_LEAVE();
}

/*
 * this function returns the scheduler object for the current CPU.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the current CPU identifier.
 * 2) retrieve the current scheduler object.
 */

t_status			scheduler_current(o_scheduler**		scheduler)
{
  i_cpu			cpu;

  /*
   * 0)
   */

  if (scheduler == NULL)
    CORE_ESCAPE("the 'scheduler' argument is null");

  /*
   * 1)
   */

  if (cpu_current(&cpu) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the current CPU object");

  /*
   * 2)
   */

  if (set_get(_scheduler.schedulers, cpu, (void**)scheduler) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the scheduler from the set");

  CORE_LEAVE();
}

/*
 * this function initializes the scheduler manager.
 *
 * steps:
 *
 * 1) display a message.
 * 2) initialize the manager's structure.
 * 3) initialize the quantum.
 * 4) retrieve the number of CPUs.
 * 5) reserve the set of schedulers.
 * 6) go through the CPUs.
 *   a) retrieve the CPU object.
 *   b) build the scheduler object.
 *   g) add the scheduler to the set of schedulers.
 * 7) retrieve the currently running scheduler.
 * 8) set the scheduler's current thread as being the kernel thread.
 * 9) call the machine.
 */

t_status			scheduler_initialize(void)
{
  o_scheduler*		scheduler;
  t_setsz		ncpus;
  s_iterator		it;
  t_state		st;
  o_cpu*		o;

  /*
   * 1)
   */

  module_call(console, message,
	      '+', "initializing the scheduler manager\n");

  /*
   * 2)
   */

  memset(&_scheduler, 0x0, sizeof (m_scheduler));

  /*
   * 3)
   */

  _scheduler.quantum = SCHEDULER_QUANTUM;

  /*
   * 4)
   */

  if (set_size(_cpu.cpus, &ncpus) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the number of active CPUs");

  /*
   * 5)
   */

  if (set_reserve(array,
		  SET_OPTION_ALLOCATE,
		  ncpus,
		  sizeof (o_scheduler),
		  &_scheduler.schedulers) != STATUS_OK)
    CORE_ESCAPE("unable to reserve a set for the schedulers");

  /*
   * 6)
   */

  set_foreach(SET_OPTION_FORWARD, _cpu.cpus, &it, st)
    {
      o_scheduler	scheduler;

      /*
       * a)
       */

      if (set_object(_cpu.cpus, it, (void**)&o) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the CPU object");

      /*
       * b)
       */

      scheduler.cpu = o->id;
      scheduler.thread = ID_UNUSED;
      scheduler.timeslice = _scheduler.quantum;
      scheduler.priority = 0;
      scheduler.state = SCHEDULER_STATE_STOP;

      /*
       * g)
       */

      if (set_append(_scheduler.schedulers, &scheduler) != STATUS_OK)
	CORE_ESCAPE("unable to append the CPU's scheduler to the set");
    }

  /*
   * 7)
   */

  if (scheduler_current(&scheduler) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the current CPU's scheduler");

  /*
   * 8)
   */

  scheduler->thread = _kernel.thread;

  /*
   * 9)
   */

  if (machine_call(scheduler, initialize) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function just reinitializes the scheduler manager.
 *
 * steps:
 *
 * 1) display a message.
 * 2) call the machine.
 */

t_status			scheduler_clean(void)
{
  /*
   * 1)
   */

  module_call(console, message,
	      '+', "cleaning the scheduler manager\n");

  /*
   * 2)
   */

  if (machine_call(scheduler, clean) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

#endif
