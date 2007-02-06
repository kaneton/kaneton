/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/sched/sched.c
 *
 * created       matthieu bucchianeri   [sat jun  3 22:36:59 2006]
 * updated       matthieu bucchianeri   [tue feb  6 22:49:35 2007]
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
 * calling  of sched_switch  and cpu_balance.  for example,  these two
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

#include <klibc.h>
#include <kaneton.h>

machdep_include(sched);

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * scheduler manager variable.
 */

m_sched*		sched = NULL;

/*                                                                 [cut] k3 */

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
 *
 * steps:
 *
 * 1) dump the current thread.
 * 2) dump the scheduler active queues.
 * 2) dump the scheduler expired queues.
 */

t_error			sched_dump(void)
{
  t_iterator		i;
  t_iterator		iq;
  t_iterator		ic;
  t_state		st;
  t_state		stq;
  t_state		stc;
  i_set*		queue;
  t_scheduled*		thread;
  t_prior		prio;
  t_cpu_sched*		ent;

  SCHED_ENTER(sched);

  set_foreach(SET_OPT_FORWARD, sched->cpus, &ic, stc)
    {
      if (set_object(sched->cpus, ic, (void**)&ent) != ERROR_NONE)
	SCHED_LEAVE(sched, ERROR_UNKNOWN);

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
	    SCHED_LEAVE(sched, ERROR_UNKNOWN);

	  set_foreach(SET_OPT_FORWARD, *queue, &iq, stq)
	    {
	      if (set_object(*queue, iq, (void**)&thread) != ERROR_NONE)
		SCHED_LEAVE(sched, ERROR_UNKNOWN);

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
	    SCHED_LEAVE(sched, ERROR_UNKNOWN);

	  set_foreach(SET_OPT_FORWARD, *queue, &iq, stq)
	    {
	      if (set_object(*queue, iq, (void**)&thread) != ERROR_NONE)
		SCHED_LEAVE(sched, ERROR_UNKNOWN);

	      printf(" %qd", thread->thread);
	    }
	}

      printf("\n");

    }

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function sets the scheduler quantum value.
 *
 * steps:
 *
 * 1) set the new quantum value.
 * 2) update all thread timeslices to take account of new granularity.
 * 3) call the architecture dependent code.
 */

t_error			sched_quantum(t_quantum			quantum)
{
  t_iterator		i;
  t_iterator		ic;
  t_state		st;
  t_state		stc;
  t_iterator		iq;
  t_state		stq;
  i_set*		queue;
  t_scheduled*		entity;
  t_cpu_sched*		ent;

  SCHED_ENTER(sched);

  /*
   * 1)
   */

  sched->quantum = quantum;

  /*
   * 2)
   */

  set_foreach(SET_OPT_FORWARD, sched->cpus, &ic, stc)
    {
      if (set_object(sched->cpus, i, (void**)&ent) != ERROR_NONE)
	SCHED_LEAVE(sched, ERROR_UNKNOWN);

      ent->timeslice = SCALE_TIMESLICE(ent->timeslice);

      set_foreach(SET_OPT_FORWARD, ent->active, &i, st)
	{
	  if (set_object(ent->active, i, (void**)&queue) != ERROR_NONE)
	    SCHED_LEAVE(sched, ERROR_UNKNOWN);

	  set_foreach(SET_OPT_FORWARD, *queue, &iq, stq)
	    {
	      if (set_object(*queue, iq, (void**)&entity) != ERROR_NONE)
		SCHED_LEAVE(sched, ERROR_UNKNOWN);

	      entity->timeslice = SCALE_TIMESLICE(entity->timeslice);
	    }
	}

      set_foreach(SET_OPT_FORWARD, ent->expired, &i, st)
	{
	  if (set_object(ent->expired, i, (void**)&queue) != ERROR_NONE)
	    SCHED_LEAVE(sched, ERROR_UNKNOWN);

	  set_foreach(SET_OPT_FORWARD, *queue, &iq, stq)
	    {
	      if (set_object(*queue, iq, (void**)&entity) != ERROR_NONE)
		SCHED_LEAVE(sched, ERROR_UNKNOWN);

	      entity->timeslice = SCALE_TIMESLICE(entity->timeslice);
	    }
	}
    }

  /*
   * 2)
   */

  if (machdep_call(sched, sched_quantum, quantum) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function permits the current task to relinquish the processor
 * voluntary.
 *
 * steps:
 *
 * 1) we force  the current thread's  priority to the minimum,  so the
 *    switch function  will force switching  to another active  thread
 *    if possible.
 * 2) call  the dependent code (calling switch with the good cpu).
 */

t_error			sched_yield(i_cpu			cpuid)
{
  t_cpu_sched*		ent;

  SCHED_ENTER(sched);

  /*
   * 1)
   */

  if (set_get(sched->cpus, cpuid, (void**)&ent) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  ent->prio = SCHED_N_PRIORITY_QUEUE + 1;

  /*
   * 2)
   */

  if (machdep_call(sched, sched_yield, cpuid) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this  function returns  the  identifier of  the currently  executed
 * thread.
 */

t_error			sched_current(i_thread*			thread)
{
  i_cpu			cpuid;
  t_cpu_sched*		ent;

  SCHED_ENTER(sched);

  if (cpu_current(&cpuid) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  if (set_get(sched->cpus, cpuid, (void**)&ent) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  *thread = ent->current;

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function switches execution to the specified thread.
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
 */

t_error			sched_switch(void)
{
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
  i_set			list;
  i_cpu			cpuid;
  t_cpu_sched*		ent;

  SCHED_ENTER(sched);

  if (cpu_current(&cpuid) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  if (set_get(sched->cpus, cpuid, (void**)&ent) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  elected = ent->current;

  /*
   * 1)
   */

  ent->timeslice -= sched->quantum;

  /*
   * 2)
   */

  if (ent->timeslice == 0)
    {
      entity.thread = ent->current;
      entity.timeslice = COMPUTE_TIMESLICE(ent->current);

      prio = COMPUTE_PRIORITY(ent->current);

      p = 0;
      set_foreach(SET_OPT_FORWARD, ent->expired, &i, st)
	{
	  if (set_object(ent->expired, i, (void**)&queue) != ERROR_NONE)
	    SCHED_LEAVE(sched, ERROR_UNKNOWN);

	  if (prio == p)
	    {
	      if (set_push(*queue, &entity) != ERROR_NONE)
		SCHED_LEAVE(sched, ERROR_UNKNOWN);

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
      if (set_object(ent->active, i, (void**)&queue) != ERROR_NONE)
	SCHED_LEAVE(sched, ERROR_UNKNOWN);

      if (p > ent->prio && ent->timeslice)
	{
	  nonempty = 1;
	  elected = ent->current;
	  elected_prio = ent->prio;
	  elected_timeslice = ent->timeslice;
	  break;
	}

      if (set_pick(*queue, (void**)&highest) == ERROR_NONE)
	{
	  nonempty = 1;
	  elected = highest->thread;
	  elected_prio = p;
	  elected_timeslice = highest->timeslice;

	  if (set_pop(*queue) != ERROR_NONE)
	    SCHED_LEAVE(sched, ERROR_UNKNOWN);

	  break;
	}

      p++;
    }

  /*
   * 4)
   */

  if (!nonempty)
    {
      list = ent->active;
      ent->active = ent->expired;
      ent->expired = list;

      goto try;
    }

  /*
   * 5)
   */

  if (ent->timeslice != 0 && elected != ent->current)
    {
      entity.thread = ent->current;
      entity.timeslice = ent->timeslice;

      prio = COMPUTE_PRIORITY(ent->current);

      p = 0;
      set_foreach(SET_OPT_FORWARD, ent->active, &i, st)
	{
	  if (set_object(ent->active, i, (void**)&queue) != ERROR_NONE)
	    SCHED_LEAVE(sched, ERROR_UNKNOWN);

	  if (prio == p)
	    {
	      if (set_push(*queue, &entity) != ERROR_NONE)
		SCHED_LEAVE(sched, ERROR_UNKNOWN);

	      break;
	    }

	  p++;
	}
    }

  /*
   * 6)
   */

  if (machdep_call(sched, sched_switch, elected) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  ent->current = elected;
  ent->prio = elected_prio;
  ent->timeslice = elected_timeslice;

  /*
   * 7)
   */

  if (cpu_stats(cpuid, sched->quantum) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);


  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function adds a thread to the scheduler.
 *
 * steps:
 *
 * 1) compute the thread's priority.
 * 2) add the thread to the right queue.
 * 3) preempt current thread if added thread has an higher priority.
 * 4) call the machine dependent code.
 */

t_error			sched_add(i_thread			thread)
{
  t_iterator		i;
  t_state		st;
  i_set*		queue;
  t_prior		prio;
  t_prior		p;
  t_scheduled		entity;
  o_thread*		othread;
  o_task*		otask;
  t_cpu_sched*		ent;

  SCHED_ENTER(sched);

  /*
   * 1)
   */

  prio = COMPUTE_PRIORITY(thread);

  /*
   * 2)
   */

  if (thread_get(thread, &othread) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  if (task_get(othread->taskid, &otask) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  if (set_get(sched->cpus, otask->cpuid, (void**)&ent) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  p = 0;
  set_foreach(SET_OPT_FORWARD, ent->active, &i, st)
    {
      if (set_object(ent->active, i, (void**)&queue) != ERROR_NONE)
	SCHED_LEAVE(sched, ERROR_UNKNOWN);

      if (prio == p)
	{
	  entity.thread = thread;
	  entity.timeslice = COMPUTE_TIMESLICE(thread);

	  if (set_push(*queue, &entity) != ERROR_NONE)
	    SCHED_LEAVE(sched, ERROR_UNKNOWN);

	  break;
	}

      p++;
    }

  /*
   * 3)
   */

  if (ent->timeslice > sched->quantum &&
      prio < COMPUTE_PRIORITY(ent->current))
    SCHED_LEAVE(sched, sched_yield(ent->cpuid));

  /*
   * 4)
   */

  if (machdep_call(sched, sched_add, thread) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function removes a thread from the scheduler.
 *
 * steps:
 *
 * 1) if the  currently scheduled thread  is being removed,  then stop
 *    its execution.
 * 2) call architecture dependent code.
 * 3) compute the thread priority.
 * 4) look in both active and expired lists to remove the thread.
 */

t_error			sched_remove(i_thread			thread)
{
  t_iterator		i;
  t_state		st;
  i_set*		queue;
  t_prior		prio;
  t_prior		p;
  int			removed = 0;
  o_thread*		othread;
  o_task*		otask;
  t_cpu_sched*		ent;

  SCHED_ENTER(sched);

  /*
   * 1)
   */

  if (thread_get(thread, &othread) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  if (task_get(othread->taskid, &otask) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  if (set_get(sched->cpus, otask->cpuid, (void**)&ent) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  if (ent->current == thread)
    if (sched_yield(ent->cpuid) != ERROR_NONE)
      SCHED_LEAVE(sched, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (machdep_call(sched, sched_remove, thread) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  /*
   * 3)
   */

  prio = COMPUTE_PRIORITY(thread);

  /*
   * 4)
   */

  p = 0;
  set_foreach(SET_OPT_FORWARD, ent->active, &i, st)
    {
      if (set_object(ent->active, i, (void**)&queue) != ERROR_NONE)
	SCHED_LEAVE(sched, ERROR_UNKNOWN);

      if (prio == p)
	{
	  if (set_remove(*queue, thread) == ERROR_NONE)
	    removed = 1;
	  break;
	}
    }

  if (!removed)
    {
      p = 0;
      set_foreach(SET_OPT_FORWARD, ent->active, &i, st)
	{
	  if (set_object(ent->active, i, (void**)&queue) != ERROR_NONE)
	    SCHED_LEAVE(sched, ERROR_UNKNOWN);

	  if (prio == p)
	    {
	      if (set_remove(*queue, thread) != ERROR_NONE)
		SCHED_LEAVE(sched, ERROR_UNKNOWN);
	      break;
	    }
	}
    }

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*
 * this function updates a thread into the scheduler.

 *
 * steps:
 *
 * 1) XXX optimize this !!!
 * 2) call the machine dependent code.
 */

t_error			sched_update(i_thread			thread)
{
  SCHED_ENTER(sched);

  /*
   * 1)
   */

  if (sched_remove(thread) != ERROR_NONE ||
      sched_add(thread) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (machdep_call(sched, sched_update, thread) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  SCHED_LEAVE(sched, ERROR_NONE);
}

/*                                                                [cut] /k3 */

/*
 * this function initialises the scheduler manager.
 *
 * steps:
 *
 * 1) allocate and initialises the scheduler manager structure.
 * 2) try to reserve a statistics object.
 * 3) create the thread lists.
 * 4) create the kernel thread.
 * 5) call the machine-dependent code.
 */

t_error			sched_init(void)
{
  int			i;
  t_iterator		it;
  t_state		st;
  i_set			queue;
  i_thread		kthread;
  t_cpu_sched		ent;
  t_cpu_sched*		ent2;
  i_cpu			cpuid;
  o_cpu*		o;
  t_setsz		ncpus;

  /*
   * 1)
   */

  if ((sched = malloc(sizeof(m_sched))) == NULL)
    {
      cons_msg('!', "sched: cannot allocate memory for the scheduler "
	       "manager structure\n");

      return (ERROR_UNKNOWN);
    }

  memset(sched, 0x0, sizeof(m_sched));

  sched->quantum = SCHED_QUANTUM_INIT;
  sched->cpus = ID_UNUSED;

  /*
   * 2)
   */

  STATS_RESERVE("sched", &sched->stats);

  /*
   * 3)
   */

  if (set_size(cpu->cpus, &ncpus) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (set_reserve(array, SET_OPT_ALLOC, ncpus, sizeof(t_cpu_sched),
		  &sched->cpus) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  set_foreach(SET_OPT_FORWARD, cpu->cpus, &it, st)
    {
      if (set_object(cpu->cpus, it, (void**)&o) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      ent.cpuid = o->cpuid;
      ent.current = ID_UNUSED;
      ent.timeslice = sched->quantum;
      ent.prio = SCHED_N_PRIORITY_QUEUE;
      ent.active = ID_UNUSED;
      ent.expired = ID_UNUSED;

      if (set_reserve(array, SET_OPT_ALLOC, SCHED_N_PRIORITY_QUEUE + 1,
		      sizeof(i_set), &ent.active) !=
	  ERROR_NONE)
	return (ERROR_UNKNOWN);

      for (i = 0; i <= SCHED_N_PRIORITY_QUEUE; i++)
	{
	  if (set_reserve(pipe, SET_OPT_ALLOC, sizeof(t_scheduled), &queue) !=
	      ERROR_NONE)
	    return (ERROR_UNKNOWN);

	  if (set_insert(ent.active, &queue) != ERROR_NONE)
	    return (ERROR_UNKNOWN);
	}

      if (set_reserve(array, SET_OPT_ALLOC, SCHED_N_PRIORITY_QUEUE + 1,
		      sizeof(i_set), &ent.expired) !=
	  ERROR_NONE)
	return (ERROR_UNKNOWN);

      for (i = 0; i <= SCHED_N_PRIORITY_QUEUE; i++)
	{
	  if (set_reserve(pipe, SET_OPT_ALLOC, sizeof(t_scheduled), &queue) !=
	      ERROR_NONE)
	    return (ERROR_UNKNOWN);

	  if (set_insert(ent.expired, &queue) != ERROR_NONE)
	    return (ERROR_UNKNOWN);
	}

      if (set_append(sched->cpus, &ent) != ERROR_NONE)
	return (ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  if (thread_reserve(ktask, THREAD_PRIOR, &kthread) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (cpu_current(&cpuid) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  if (set_get(sched->cpus, cpuid, (void**)&ent2) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  ent2->current = kthread;

  /*
   * 5)
   */

  if (machdep_call(sched, sched_init) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}

/*
 * this function just reinitialises the scheduler manager.
 *
 * steps:
 *
 * 1) call the machine-dependent code.
 * 2) release the lists and the queues.
 * 3) release the stats object.
 * 4) free the scheduler manager structure's memory.
 */

t_error			sched_clean(void)
{
  t_iterator		it;
  t_state		st;
  t_iterator		it2;
  t_state		st2;
  i_set*		queue;
  t_cpu_sched*		ent;

  /*
   * 1)
   */

  if (machdep_call(sched, sched_clean) != ERROR_NONE)
    SCHED_LEAVE(sched, ERROR_UNKNOWN);

  /*
   * 2)
   */

  set_foreach(SET_OPT_FORWARD, sched->cpus, &it2, st2)
    {
      if (set_object(sched->cpus, it2, (void**)&ent) != ERROR_NONE)
	SCHED_LEAVE(sched, ERROR_UNKNOWN);

      set_foreach(SET_OPT_FORWARD, ent->active, &it, st)
	{
	  if (set_object(ent->active, it, (void**)&queue) != ERROR_NONE)
	    SCHED_LEAVE(sched, ERROR_UNKNOWN);

	  if (set_release(*queue) != ERROR_NONE)
	    SCHED_LEAVE(sched, ERROR_UNKNOWN);
	}

      if (set_release(ent->active) != ERROR_NONE)
	SCHED_LEAVE(sched, ERROR_UNKNOWN);

      set_foreach(SET_OPT_FORWARD, ent->expired, &it, st)
	{
	  if (set_object(ent->expired, it, (void**)&queue) != ERROR_NONE)
	    SCHED_LEAVE(sched, ERROR_UNKNOWN);

	  if (set_release(*queue) != ERROR_NONE)
	    SCHED_LEAVE(sched, ERROR_UNKNOWN);
	}

      if (set_release(ent->expired) != ERROR_NONE)
	SCHED_LEAVE(sched, ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  STATS_RELEASE(sched->stats);

  /*
   * 4)
   */

  free(sched);

  return (ERROR_NONE);
}

/*                                                                 [cut] k3 */

/*
 * -------- tests -------------------------------------------------------------
 *
 * To be removed...
 */

static volatile t_uint64 c1 = 0;
static volatile t_uint64 c2 = 0;
static volatile t_uint64 c3 = 0;
static volatile t_uint64 c4 = 0;

void _fun1(void)
{
  while (1)
    {
      c1++;
    }
}

void _fun2(void)
{
  while (1)
    {
      c2++;
    }
}

void _fun3(void)
{
  while (1)
    {
      c3++;
    }
}

void _fun4(void)
{
  while (1)
    {
      c4++;
    }
}

i_task sched_test_add_thread(void *func, t_prior p)
{
  static i_task		tsk = ID_UNUSED;
  i_as			as;
  o_thread*		o;
  t_thread_context	ctx;
  i_thread		thr;
  t_stack		stack;

  if (tsk == ID_UNUSED)
    {
      if (task_reserve(TASK_CLASS_PROGRAM,
		       TASK_BEHAV_INTERACTIVE,
		       TASK_PRIOR_INTERACTIVE,
		       &tsk) != ERROR_NONE)
	{
	  cons_msg('!', "cannot reserve task\n");
	  while (1);
	}

      if (as_reserve(tsk, &as) != ERROR_NONE)
	{
	  cons_msg('!', "cannot reserve as\n");
	  while (1);
	}

      if (io_grant(0x60, tsk, 1) != ERROR_NONE)
	{
	  cons_msg('!', "cannot grant I/O\n");
	  while (1);
	}
    }

  if (thread_reserve(tsk, p, &thr) != ERROR_NONE)
    {
      cons_msg('!', "cannot reserve thread\n");
      while (1);
    }

  stack.base = 0;
  stack.size = THREAD_MIN_STACKSZ;
  if (thread_stack(thr, stack) != ERROR_NONE)
    {
      cons_msg('!', "cannot set stack\n");
      while (1);
    }

  if (thread_get(thr, &o) != ERROR_NONE)
    {
      cons_msg('!', "cannot get thread\n");
      while (1);
    }

  /*
   * XXX
   *
   * faire une fonction dependante de l'archi
   * pour trouver le sommet de la pile.
   */

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = (t_uint32)func;

  if (thread_load(thr, ctx) != ERROR_NONE)
    {
      cons_msg('!', "cannot load context\n");
      while (1);
    }

  return tsk;
}

void sched_test(void)
{
#if 1
  i_task t;

  sched_test_add_thread(_fun1, 10);
  sched_test_add_thread(_fun2, 100);
  sched_test_add_thread(_fun3, 150);
  t = sched_test_add_thread(_fun4, 250);

  if (task_state(t, SCHED_STATE_RUN) != ERROR_NONE)
    {
      cons_msg('!', "cannot start !\n");

      while (1);
    }

  sched_dump();

  printf("\n\n\n");

  while(1)
    {
      printf("\r%12qu%12qu%12qu%12qu                      ",
	     c1, c2, c3, c4);
    }
#endif
#if 0
  t_uint32 dumps[2][10];
  t_uint32* regs = dumps[0];
  int first = 1;
  while (1)
    {
      asm volatile("pusha");

      asm volatile("movl %%eax, %0" : "=g" (regs[0]));
      asm volatile("movl %%ebx, %0" : "=g" (regs[1]));
      asm volatile("movl %%ecx, %0" : "=g" (regs[2]));
      asm volatile("movl %%edx, %0" : "=g" (regs[3]));
      asm volatile("movl %%esi, %0" : "=g" (regs[4]));
      asm volatile("movl %%edi, %0" : "=g" (regs[5]));
      asm volatile("movl %%esp, %0" : "=g" (regs[6]));
      asm volatile("movl %%ebp, %0" : "=g" (regs[7]));
      regs[8] = *(t_uint32*)(regs[6]);
      regs[9] = *(t_uint32*)(regs[6] + 4);

      if (0 && dumps[0][0] != dumps[1][0])
	printf("eax differ %d %d\n", dumps[0][0], dumps[1][0]);
      if (!first && (dumps[0][1] != dumps[1][1]))
	printf("ebx differ %d %d\n", dumps[0][1], dumps[1][1]);
      if (!first && (dumps[0][2] != dumps[1][2]))
	printf("ecx differ %d %d\n", dumps[0][2], dumps[1][2]);
      if (!first && (dumps[0][3] != dumps[1][3]))
	printf("edx differ %d %d\n", dumps[0][3], dumps[1][3]);
      if (!first && (dumps[0][4] != dumps[1][4]))
	printf("esi differ %d %d\n", dumps[0][4], dumps[1][4]);
      if (!first && (dumps[0][5] != dumps[1][5]))
	printf("edi differ %d %d\n", dumps[0][5], dumps[1][5]);
      if (!first && (dumps[0][6] != dumps[1][6]))
	printf("esp differ %d %d\n", dumps[0][6], dumps[1][6]);
      if (!first && (dumps[0][7] != dumps[1][7]))
	printf("ebp differ %d %d\n", dumps[0][7], dumps[1][7]);
      if (!first && (dumps[0][8] != dumps[1][8]))
	printf("top stack differ %d %d\n", dumps[0][8], dumps[1][8]);
      if (!first && (dumps[0][9] != dumps[1][9]))
	printf("top stack (2) differ %d %d\n", dumps[0][9], dumps[1][9]);

      if (regs == dumps[0])
	regs = dumps[1];
      else
	regs = dumps[0];
      asm volatile("popa");

      first = 0;
    }
#endif
}

/*                                                                [cut] /k3 */
