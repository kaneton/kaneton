/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/cry...neton/kaneton/core/scheduler/scheduler.c
 *
 * created       matthieu bucchianeri   [sat jun  3 22:36:59 2006]
 * updated       matthieu bucchianeri   [wed jan  9 10:53:57 2008]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this implementation is a multilevel feedback queues scheduler (like
 * the GNU/Linux one).
 *
 * future implementation  will   include  dynamic  task  priorization.
 *
 * following  explanation stand  for  one processor.  the mecanism  is
 * multiplied by the number of processors on the system.
 *
 * there are  two lists : the  active and the expired  list.  each one
 * contains an equal  number of queues (FIFO). the  first queue if for
 * the highest priority threads and the last for the lowest priority.
 *
 * a thread priority is computed based on its parent task priority and
 * the thread's priority itself. a thread's execution timeslice is the
 * time a  thread can be executed.  it is computed  with the priority:
 * higher priotity threads obtain an higher execution timeslice.
 *
 * the  quantum corresponds  to  the interval  when  the scheduler  is
 * called.  so the execution  timeslices have  a granularity  equal to
 * this quantum.
 *
 * when the scheduler is called, it checks for the currently executing
 * thread  timeslice expiration.   if there  is no  more time  for the
 * current thread,  it is placed in  the expired list,  into the queue
 * corresponding to  its priority. then,  the scheduler looks  for the
 * highest priority active thread. it  can be the current thread, then
 * no  preemption  is  done.   if  multiple thread  have  the  highest
 * priority, then  they are taken round-robin. the  selected thread is
 * now scheduled for immediate execution.
 *
 * when the  active queues become  empty, the active and  expired list
 * are swapped so the scheduler can continue its operation.
 *
 * the machine dependent part is responsible for context switching and
 * calling  of scheduler_switch  and cpu_balance.  for example,  these two
 * functions can be called by a timer.
 *
 * this implementation  is based on explanations  around the GNU/Linux
 * 2.6 branch "O(1)" scheduler.   a very good document describing this
 * scheduler   and  the  current   GNU/Linux  implementation   can  be
 * downloaded at http://josh.trancesoftware.com/linux.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

machine_include(scheduler);

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * scheduler manager variable.
 */

m_scheduler*		scheduler = NULL;

i_thread		kthread = ID_UNUSED;

/*
 * kernel task id.
 */

extern i_task		ktask;

/*
 * cpu manager.
 */

extern m_cpu*		cpu;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * dump the scheduler state.
 *							 [block::dump::comment]
 *
 * steps:
 *
 * 1) dump the current thread.
 * 2) dump the scheduler active queues.
 * 2) dump the scheduler expired queues.
 *
 *						      [endblock::dump::comment]
 */

t_error			scheduler_dump(void)
{
  /*							 [block::dump::vars] */

  t_iterator		i;
  t_iterator		iq;
  t_iterator		ic;
  t_state		st;
  t_state		stq;
  t_state		stc;
  i_set*		queue;
  t_scheduled*		thread;
  t_prior		prio;
  o_scheduler*		ent;

  /*						      [endblock::dump::vars] */

  SCHEDULER_ENTER(scheduler);

  /*							       [block::dump] */

  set_foreach(SET_OPT_FORWARD, scheduler->cpus, &ic, stc)
    {
      if (set_object(scheduler->cpus, ic, (void**)&ent) != ERROR_NONE)
	SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

      cons_msg('#', "cpu %qd\n", ent->cpuid);

      /*
       * 1)
       */

      cons_msg('#', " scheduler current thread: %qd (%d, %d ms)\n",
	       ent->current, ent->prio, ent->timeslice);

      /*
       * 2)
       */

      cons_msg('#', " scheduler active queues:");

      prio = 0;
      set_foreach(SET_OPT_FORWARD, ent->active, &i, st)
	{
	  if (set_object(ent->active, i, (void**)&queue) != ERROR_NONE)
	    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

	  set_foreach(SET_OPT_FORWARD, *queue, &iq, stq)
	    {
	      if (set_object(*queue, iq, (void**)&thread) != ERROR_NONE)
		SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

	      printf(" %qd (%d, %d ms)", thread->thread, prio,
		     thread->timeslice);
	    }

	  prio++;
	}

      printf("\n");

      /*
       * 3)
       */

      cons_msg('#', " scheduler expired queues:");

      set_foreach(SET_OPT_FORWARD, ent->expired, &i, st)
	{
	  if (set_object(ent->expired, i, (void**)&queue) != ERROR_NONE)
	    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

	  set_foreach(SET_OPT_FORWARD, *queue, &iq, stq)
	    {
	      if (set_object(*queue, iq, (void**)&thread) != ERROR_NONE)
		SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

	      printf(" %qd", thread->thread);
	    }
	}

      printf("\n");

    }
  /*							    [endblock::dump] */

  SCHEDULER_LEAVE(scheduler, ERROR_NONE);
}

/*
 * this function sets the scheduler quantum value.
 *						      [block::quantum::comment]
 *
 * steps:
 *
 * 1) set the new quantum value.
 * 2) update all thread timeslices to take account of new granularity.
 * 3) call the architecture dependent code.
 *
 *						   [endblock::quantum::comment]
 */

t_error			scheduler_quantum(t_quantum			quantum)
{
  /*						      [block::quantum::vars] */

  t_iterator		i;
  t_iterator		ic;
  t_state		st;
  t_state		stc;
  t_iterator		iq;
  t_state		stq;
  i_set*		queue;
  t_scheduled*		entity;
  o_scheduler*		ent;

  /*						   [endblock::quantum::vars] */

  SCHEDULER_ENTER(scheduler);

  /*							    [block::quantum] */

  ASSERT(quantum != 0);

  /*
   * 1)
   */

  scheduler->quantum = quantum;

  /*
   * 2)
   */

  set_foreach(SET_OPT_FORWARD, scheduler->cpus, &ic, stc)
    {
      if (set_object(scheduler->cpus, i, (void**)&ent) != ERROR_NONE)
	SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

      ent->timeslice = SCHEDULER_SCALE_TIMESLICE(ent->timeslice);

      set_foreach(SET_OPT_FORWARD, ent->active, &i, st)
	{
	  if (set_object(ent->active, i, (void**)&queue) != ERROR_NONE)
	    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

	  set_foreach(SET_OPT_FORWARD, *queue, &iq, stq)
	    {
	      if (set_object(*queue, iq, (void**)&entity) != ERROR_NONE)
		SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

	      entity->timeslice = SCHEDULER_SCALE_TIMESLICE(entity->timeslice);
	    }
	}

      set_foreach(SET_OPT_FORWARD, ent->expired, &i, st)
	{
	  if (set_object(ent->expired, i, (void**)&queue) != ERROR_NONE)
	    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

	  set_foreach(SET_OPT_FORWARD, *queue, &iq, stq)
	    {
	      if (set_object(*queue, iq, (void**)&entity) != ERROR_NONE)
		SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

	      entity->timeslice = SCHEDULER_SCALE_TIMESLICE(entity->timeslice);
	    }
	}
    }

  /*
   * 2)
   */

  if (machine_call(scheduler, scheduler_quantum, quantum) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  /*							 [endblock::quantum] */

  SCHEDULER_LEAVE(scheduler, ERROR_NONE);
}

/*
 * this function permits the current task to relinquish the processor
 * voluntary.
 *							[block::yield::comment]
 *
 * steps:
 *
 * 1) we force  the current thread's  priority to the minimum,  so the
 *    switch function  will force switching  to another active  thread
 *    if possible.
 * 2) call  the dependent code (calling switch with the good cpu).
 *
 *						     [endblock::yield::comment]
 */

t_error			scheduler_yield(i_cpu			cpuid)
{
  /*							[block::yield::vars] */

  o_scheduler*		ent;

  /*						     [endblock::yield::vars] */

  SCHEDULER_ENTER(scheduler);

  /*							      [block::yield] */

  /*
   * 1)
   */

  if (set_get(scheduler->cpus, cpuid, (void**)&ent) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  ent->yield = 1;

  if (scheduler_switch() != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (machine_call(scheduler, scheduler_yield, cpuid) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  /*							   [endblock::yield] */

  SCHEDULER_LEAVE(scheduler, ERROR_NONE);
}

/*
 * this  function returns  the  identifier of  the currently  executed
 * thread.
 *
 */

t_error			scheduler_current(i_thread*			thread)
{
  /*						      [block::current::vars] */

  i_cpu			cpuid;
  o_scheduler*		ent;

  /*						   [endblock::current::vars] */

  SCHEDULER_ENTER(scheduler);

  /*							    [block::current] */

  ASSERT(thread != NULL);

  if (cpu_current(&cpuid) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  if (set_get(scheduler->cpus, cpuid, (void**)&ent) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  *thread = ent->current;

  /*							 [endblock::current] */

  SCHEDULER_LEAVE(scheduler, ERROR_NONE);
}

/*
 * this function switches execution to the specified thread.
 *						       [block::switch::comment]
 *
 * steps:
 *
 * 1) update the current thread timeslice.
 * 2) if the thread timeslice is expired, push it into the expired queues.
 * 3) look for the higher priority task.
 * 4) if there are no active threads, swap the expired and active queues.
 * 5) if  we are changing  current thread  while the  executing thread
 *    timeslice is not expired, push it back to the active list.
 * 6) call the machine dependent code that performs the context switching.
 * 7) update the scheduler internal informations.
 *
 *
 *						    [endblock::switch::comment]
 */

t_error			scheduler_switch(void)
{
  /*						       [block::switch::vars] */

  t_iterator		i;
  t_state		st;
  t_scheduled		entity;
  t_scheduled*		highest;
  t_prior		prio;
  t_prior		p;
  i_set*		queue;
  i_thread		elected;
  t_prior		elected_prio = (t_prior)-1;
  t_timeslice		elected_timeslice = (t_timeslice)-1;
  int			nonempty = 0;
  int			second_round = 0;
  i_set			list;
  i_cpu			cpuid;
  o_scheduler*		ent;

  /*						    [endblock::switch::vars] */

  SCHEDULER_ENTER(scheduler);

  /*							     [block::switch] */

  if (cpu_current(&cpuid) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  if (VIEW_SIGNAL("scheduler", cpuid, VIEW_SIGNAL_SCHEDULING) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  if (set_get(scheduler->cpus, cpuid, (void**)&ent) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  elected = ent->current;

  /*
   * 1)
   */

  ent->timeslice -= scheduler->quantum;

  /*
   * 2)
   */

  if (elected != kthread && elected != scheduler->idle &&
      ent->timeslice == 0 && ent->current != -1)
    {
      entity.thread = ent->current;
      entity.timeslice = SCHEDULER_COMPUTE_TIMESLICE(ent->current);

      prio = SCHEDULER_COMPUTE_PRIORITY(ent->current);

      p = 0;
      set_foreach(SET_OPT_FORWARD, ent->expired, &i, st)
	{
	  if (prio == p)
	    {
	      if (set_object(ent->expired, i, (void**)&queue) != ERROR_NONE)
		SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

	      if (set_push(*queue, &entity) != ERROR_NONE)
		SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

	      break;
	    }

	  p++;
	}
    }

  /*
   * 3)
   */

 try:
  p = 0;
  set_foreach(SET_OPT_FORWARD, ent->active, &i, st)
    {
      if (p > ent->prio && ent->timeslice && !ent->dead)
	{
	  nonempty = 1;
	  elected = ent->current;
	  elected_prio = ent->prio;
	  elected_timeslice = ent->timeslice;
	  break;
	}

      if (set_object(ent->active, i, (void**)&queue) != ERROR_NONE)
	SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

      if (set_pick(*queue, (void**)&highest) == ERROR_NONE)
	{
	  nonempty = 1;
	  elected = highest->thread;
	  elected_prio = p;
	  elected_timeslice = highest->timeslice;

	  if (set_pop(*queue) != ERROR_NONE)
	    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

	  break;
	}

      p++;
    }

  /*
   * 4)
   */

  if (!nonempty)
    {
      if (ent->yield && ent->timeslice)
	{
	  elected = ent->current;
	  elected_prio = ent->prio;
	  elected_timeslice = ent->timeslice;

	  goto ok;
	}

      if (second_round)
	{
	  elected = scheduler->idle;
	  elected_prio = 0;
	  elected_timeslice = scheduler->quantum;

	  goto ok;
	}

      list = ent->active;
      ent->active = ent->expired;
      ent->expired = list;
      second_round = 1;

      goto try;
    }

  if (ent->dead && ent->current == elected)
    {
      elected = scheduler->idle;
      elected_prio = 0;
      elected_timeslice = scheduler->quantum;
    }

 ok:

  /*
   * 5)
   */

  if (ent->current != kthread && ent->current != scheduler->idle &&
      ent->timeslice != 0 && elected != ent->current)
    {
      entity.thread = ent->current;
      entity.timeslice = ent->timeslice;

      prio = SCHEDULER_COMPUTE_PRIORITY(ent->current);

      p = 0;
      set_foreach(SET_OPT_FORWARD, ent->active, &i, st)
	{
	  if (prio == p)
	    {
	      if (set_object(ent->active, i, (void**)&queue) != ERROR_NONE)
		SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

	      if (set_push(*queue, &entity) != ERROR_NONE)
		SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

	      break;
	    }

	  p++;
	}
    }

  /*
   * 6)
   */

  if (machine_call(scheduler, scheduler_switch, elected) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  ent->current = elected;
  ent->prio = elected_prio;
  ent->timeslice = elected_timeslice;
  ent->yield = 0;
  ent->dead = 0;

  /*
   * 7)
   */

  if (cpu_stats(cpuid, scheduler->quantum) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  if (VIEW_SIGNAL("thread", elected, VIEW_SIGNAL_PREEMPT) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  /*							  [endblock::switch] */

  SCHEDULER_LEAVE(scheduler, ERROR_NONE);
}

/*
 * this function adds a thread to the scheduler.
 *							  [block::add::comment]
 *
 * steps:
 *
 * 1) compute the thread's priority.
 * 2) add the thread to the right queue.
 * 3) preempt current thread if added thread has an higher priority.
 * 4) call the machine dependent code.
 *
 *						       [endblock::add::comment]
 */

t_error			scheduler_add(i_thread			thread)
{
  /*							  [block::add::vars] */

  t_iterator		i;
  t_state		st;
  i_set*		queue;
  t_prior		prio;
  t_prior		p;
  t_scheduled		entity;
  o_thread*		othread;
  o_task*		otask;
  o_scheduler*		ent;

  /*						       [endblock::add::vars] */

  SCHEDULER_ENTER(scheduler);

  /*								[block::add] */

  /*
   * 1)
   */

  prio = SCHEDULER_COMPUTE_PRIORITY(thread);

  /*
   * 2)
   */

  if (thread_get(thread, &othread) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  if (task_get(othread->taskid, &otask) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  if (set_get(scheduler->cpus, otask->cpuid, (void**)&ent) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  p = 0;
  set_foreach(SET_OPT_FORWARD, ent->active, &i, st)
    {
      if (prio == p)
	{
	  entity.thread = thread;
	  entity.timeslice = SCHEDULER_COMPUTE_TIMESLICE(thread);

	  if (set_object(ent->active, i, (void**)&queue) != ERROR_NONE)
	    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

	  if (set_push(*queue, &entity) != ERROR_NONE)
	    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

	  break;
	}

      p++;
    }

  /*
   * 3)
   */

  if (ent->timeslice > scheduler->quantum &&
      prio < SCHEDULER_COMPUTE_PRIORITY(ent->current))
    SCHEDULER_LEAVE(scheduler, scheduler_yield(ent->cpuid));

  /*
   * 4)
   */

  if (machine_call(scheduler, scheduler_add, thread) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  /*							     [endblock::add] */

  SCHEDULER_LEAVE(scheduler, ERROR_NONE);
}

/*
 * this function removes a thread from the scheduler.
 *						       [block::remove::comment]
 *
 * steps:
 *
 * 1) if the  currently scheduled thread  is being removed,  then stop
 *    its execution.
 * 2) call architecture dependent code.
 * 3) compute the thread priority.
 * 4) look in both active and expired lists to remove the thread.
 *
 *						    [endblock::remove::comment]
 */

t_error			scheduler_remove(i_thread			thread)
{
  /*						       [block::remove::vars] */

  t_iterator		i;
  t_state		st;
  i_set*		queue;
  t_prior		prio;
  t_prior		p;
  int			removed = 0;
  o_thread*		othread;
  o_task*		otask;
  o_scheduler*		ent;

  /*						    [endblock::remove::vars] */

  SCHEDULER_ENTER(scheduler);

  /*							     [block::remove] */

  /*
   * 1)
   */

  if (thread_get(thread, &othread) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  if (task_get(othread->taskid, &otask) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  if (set_get(scheduler->cpus, otask->cpuid, (void**)&ent) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  /* XXX extend to MP: check all sched_ents */
  if (ent->current == thread)
    {
      ent->timeslice = scheduler->quantum;
      ent->dead = 1;

      if (scheduler_switch() != ERROR_NONE)
	SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

      if (set_get(scheduler->cpus, otask->cpuid, (void**)&ent) != ERROR_NONE)
	SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

      ASSERT(ent->current != thread);
    }

  /*
   * 2)
   */

  if (machine_call(scheduler, scheduler_remove, thread) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  /*
   * 3)
   */

  prio = SCHEDULER_COMPUTE_PRIORITY(thread);

  /*
   * 4)
   */

  p = 0;
  set_foreach(SET_OPT_FORWARD, ent->active, &i, st)
    {
      if (prio == p)
        {
	  if (set_object(ent->active, i, (void**)&queue) != ERROR_NONE)
	    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

	  if (set_remove(*queue, thread) == ERROR_NONE)
	    removed = 1;
	  break;
	}
      p++;
    }

  if (!removed)
    {
      p = 0;
      set_foreach(SET_OPT_FORWARD, ent->expired, &i, st)
	{
	  if (prio == p)
	    {
	      if (set_object(ent->expired, i, (void**)&queue) != ERROR_NONE)
		SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

	      if (set_remove(*queue, thread) != ERROR_NONE)
		SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);
	      break;
	    }
	  p++;
	}
    }

  /*							  [endblock::remove] */

  SCHEDULER_LEAVE(scheduler, ERROR_NONE);
}

/*
 * this function updates a thread into the scheduler.
 *						       [block::update::comment]
 *
 * steps:
 *
 * 1) XXX optimize this !!!
 * 2) call the machine dependent code.
 *
 *						    [endblock::update::comment]
 */

t_error			scheduler_update(i_thread			thread)
{
  SCHEDULER_ENTER(scheduler);

  /*							     [block::update] */

  /*
   * 1)
   */

  if (scheduler_remove(thread) != ERROR_NONE ||
      scheduler_add(thread) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (machine_call(scheduler, scheduler_update, thread) != ERROR_NONE)
    SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);

  /*							  [endblock::update] */

  SCHEDULER_LEAVE(scheduler, ERROR_NONE);
}

/*
 * this function initializes the scheduler manager.
 *						   [block::initialize::comment]
 *
 * steps:
 *
 * 1) allocate and initializes the scheduler manager structure.
 * 2) create the thread lists.
 * 3) create the kernel thread.
 * 4) call the machine-dependent code.
 *
 *						[endblock::initialize::comment]
 */

t_error			scheduler_initialize(void)
{
  /*							 [block::initialize] */

  int			i;
  t_iterator		it;
  t_state		st;
  i_set			queue;
  o_scheduler		ent;
  o_scheduler*		ent2;
  i_cpu			cpuid;
  o_cpu*		o;
  o_thread*		oth;
  t_setsz		ncpus;

  /*
   * 1)
   */

  if ((scheduler = malloc(sizeof(m_scheduler))) == NULL)
    {
      cons_msg('!', "scheduler: cannot allocate memory for the scheduler "
	       "manager structure\n");

      return (ERROR_UNKNOWN);
    }

  memset(scheduler, 0x0, sizeof(m_scheduler));

  scheduler->quantum = SCHEDULER_QUANTUM_INIT;
  scheduler->cpus = ID_UNUSED;

  /*
   * 2)
   */

  if (set_size(cpu->cpus, &ncpus) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (set_reserve(array, SET_OPT_ALLOC, ncpus, sizeof(o_scheduler),
		  &scheduler->cpus) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  set_foreach(SET_OPT_FORWARD, cpu->cpus, &it, st)
    {
      if (set_object(cpu->cpus, it, (void**)&o) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      ent.cpuid = o->cpuid;
      ent.current = ID_UNUSED;
      ent.timeslice = scheduler->quantum;
      ent.prio = SCHEDULER_N_PRIORITY_QUEUE;
      ent.active = ID_UNUSED;
      ent.expired = ID_UNUSED;
      ent.yield = 0;
      ent.dead = 0;

      if (set_reserve(array, SET_OPT_ALLOC, SCHEDULER_N_PRIORITY_QUEUE + 1,
		      sizeof(i_set), &ent.active) !=
	  ERROR_NONE)
	return (ERROR_UNKNOWN);

      for (i = 0; i <= SCHEDULER_N_PRIORITY_QUEUE; i++)
	{
	  if (set_reserve(pipe, SET_OPT_ALLOC, sizeof(t_scheduled), &queue) !=
	      ERROR_NONE)
	    return (ERROR_UNKNOWN);

	  if (set_insert(ent.active, &queue) != ERROR_NONE)
	    return (ERROR_UNKNOWN);
	}

      if (set_reserve(array, SET_OPT_ALLOC, SCHEDULER_N_PRIORITY_QUEUE + 1,
		      sizeof(i_set), &ent.expired) !=
	  ERROR_NONE)
	return (ERROR_UNKNOWN);

      for (i = 0; i <= SCHEDULER_N_PRIORITY_QUEUE; i++)
	{
	  if (set_reserve(pipe, SET_OPT_ALLOC, sizeof(t_scheduled), &queue) !=
	      ERROR_NONE)
	    return (ERROR_UNKNOWN);

	  if (set_insert(ent.expired, &queue) != ERROR_NONE)
	    return (ERROR_UNKNOWN);
	}

      if (set_append(scheduler->cpus, &ent) != ERROR_NONE)
	return (ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  if (thread_reserve(ktask, THREAD_PRIOR, &kthread) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (thread_get(kthread, &oth) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  oth->sched = SCHEDULER_STATE_RUN;

  if (cpu_current(&cpuid) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (set_get(scheduler->cpus, cpuid, (void**)&ent2) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  ent2->current = kthread;

  /*
   * 4)
   */

  if (machine_call(scheduler, scheduler_initialize) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*						      [endblock::initialize] */

  return (ERROR_NONE);
}

/*
 * this function just reinitializes the scheduler manager.
 *							[block::clean::comment]
 *
 * steps:
 *
 * 1) call the machine-dependent code.
 * 2) release the lists and the queues.
 * 3) free the scheduler manager structure's memory.
 *
 *						     [endblock::clean::comment]
 */

t_error			scheduler_clean(void)
{
  /*							      [block::clean] */

  t_iterator		it;
  t_state		st;
  t_iterator		it2;
  t_state		st2;
  i_set*		queue;
  o_scheduler*		ent;

  /*
   * 1)
   */

  if (machine_call(scheduler, scheduler_clean) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  set_foreach(SET_OPT_FORWARD, scheduler->cpus, &it2, st2)
    {
      if (set_object(scheduler->cpus, it2, (void**)&ent) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      set_foreach(SET_OPT_FORWARD, ent->active, &it, st)
	{
	  if (set_object(ent->active, it, (void**)&queue) != ERROR_NONE)
	    return (ERROR_UNKNOWN);

	  if (set_release(*queue) != ERROR_NONE)
	    return (ERROR_UNKNOWN);
	}

      if (set_release(ent->active) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      set_foreach(SET_OPT_FORWARD, ent->expired, &it, st)
	{
	  if (set_object(ent->expired, it, (void**)&queue) != ERROR_NONE)
	    return (ERROR_UNKNOWN);

	  if (set_release(*queue) != ERROR_NONE)
	    return (ERROR_UNKNOWN);
	}

      if (set_release(ent->expired) != ERROR_NONE)
	return (ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  free(scheduler);

  /*							   [endblock::clean] */

  return (ERROR_NONE);
}
