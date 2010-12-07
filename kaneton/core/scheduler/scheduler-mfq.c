/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...E/kaneton/core/scheduler/scheduler-mfq.c
 *
 * created       matthieu bucchianeri   [sat jun  3 22:36:59 2006]
 * updated       julien quintard   [sun dec  5 15:20:17 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this implementation is a multilevel feedback queues scheduler (like
 * the GNU/Linux one).
 *
 * future implementation  will   include  dynamic  task  priorization.
 *
 * following  explanation stand  for  one processor.  the mechanism  is
 * multiplied by the number of processors on the system.
 *
 * there are  two lists : the  active and the expired  list.  each one
 * contains an equal  number of queues (FIFO). the  first queue if for
 * the highest priority threads and the last for the lowest priority.
 *
 * a thread priority is computed based on its parent task priority and
 * the thread's priority itself. a thread's execution timeslice is the
 * time a  thread can be executed.  it is computed  with the priority:
 * higher priority threads obtain an higher execution timeslice.
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

#if (SCHEDULER_ALGORITHM == SCHEDULER_ALGORITHM_MFQ)

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

machine_include(scheduler);

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the kernel manager.
 */

extern m_kernel*	_kernel;

/*
 * cpu manager.
 */

extern m_cpu*		_cpu;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * scheduler manager variable.
 */

m_scheduler*		_scheduler = NULL;

int _XXX = 0;

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


  o_scheduler*		scheduler;
  o_scheduled*		entity;
  t_priority		priority;
  i_set*		queue;
  t_iterator		i;
  t_iterator		iq;
  t_iterator		ic;
  t_state		st;
  t_state		stq;
  t_state		stc;

  /*						      [endblock::dump::vars] */

  /*							       [block::dump] */

  set_foreach(SET_OPTION_FORWARD, _scheduler->cpus, &ic, stc)
    {
      if (scheduler_get(&scheduler) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the current CPU's scheduler");

      module_call(console, message,
		  '#', "cpu %qd\n", scheduler->cpu);

      /*
       * 1)
       */

      module_call(console, message,
		  '#', "  current: %qu (%u, %u ms)\n",
		  scheduler->thread,
		  scheduler->priority,
		  scheduler->timeslice);

      /*
       * 2)
       */

      module_call(console, message,
		  '#', "  queues:\n");

      module_call(console, message,
		  '#', "    active:\n");

      priority = 0;
      set_foreach(SET_OPTION_FORWARD, scheduler->active, &i, st)
	{
	  if (set_object(scheduler->active, i, (void**)&queue) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve the active queue with "
			"priority %u from the scheduler",
			priority);

	  set_foreach(SET_OPTION_FORWARD, *queue, &iq, stq)
	    {
	      if (set_object(*queue, iq, (void**)&entity) != ERROR_OK)
		CORE_ESCAPE("unable to retrieve the thread identifier "
			    "from the queue");

	      module_call(console, message,
			  '#', "      %qu (%u, %u ms)\n",
			  entity->thread,
			  priority,
			  entity->timeslice);
	    }

	  priority++;
	}

      /*
       * 3)
       */

      module_call(console, message,
		  '#', "    expired:\n");

      priority = 0;
      set_foreach(SET_OPTION_FORWARD, scheduler->expired, &i, st)
	{
	  if (set_object(scheduler->expired, i, (void**)&queue) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve the expired queue with "
			"priority %u from the scheduler",
			priority);

	  set_foreach(SET_OPTION_FORWARD, *queue, &iq, stq)
	    {
	      if (set_object(*queue, iq, (void**)&entity) != ERROR_OK)
		CORE_ESCAPE("unable to retrieve the thread identifier "
			    "from the queue");

	      module_call(console, message,
			  '#', "      %qu (%u, %u ms)\n",
			  entity->thread,
			  priority,
			  entity->timeslice);
	    }

	  priority++;
	}
    }

  /*							    [endblock::dump] */

  CORE_LEAVE();
}

/*
 * this function starts the scheduler.
 *
 * steps:
 *
 * 1) retrieve the current scheduler.
 * 2) change the scheduler's state.
 * 3) call the machine.
 */

t_error			scheduler_start(void)
{
  o_scheduler*		scheduler;

  /*
   * 1)
   */

  if (scheduler_get(&scheduler) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the current CPU's scheduler");

  /*
   * 2)
   */

  scheduler->state = SCHEDULER_STATE_START;

  /*
   * 3)
   */

  if (machine_call(scheduler, scheduler_start) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function stops the scheduler.
 *
 * steps:
 *
 * 1) retrieve the current scheduler.
 * 2) change the scheduler's state.
 * 3) call the machine.
 */

t_error			scheduler_stop(void)
{
  o_scheduler*		scheduler;

  /*
   * 1)
   */

  if (scheduler_get(&scheduler) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the current CPU's scheduler");

  /*
   * 2)
   */

  scheduler->state = SCHEDULER_STATE_STOP;

  /*
   * 3)
   */

  if (machine_call(scheduler, scheduler_stop) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
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

t_error			scheduler_quantum(t_quantum		quantum)
{
  /*						      [block::quantum::vars] */

  o_scheduler*		scheduler;
  o_scheduled*		entity;
  i_set*		queue;
  t_state		st;
  t_iterator		i;
  t_state		stc;
  t_iterator		ic;
  t_state		stq;
  t_iterator		iq;

  /*						   [endblock::quantum::vars] */

  /*							    [block::quantum] */

  assert(quantum != 0);

  /*
   * 1)
   */

  _scheduler->quantum = quantum;

  /*
   * 2)
   */

  set_foreach(SET_OPTION_FORWARD, _scheduler->cpus, &ic, stc)
    {
      if (scheduler_get(&scheduler) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the current CPU's scheduler");

      scheduler->timeslice = SCHEDULER_SCALE_TIMESLICE(scheduler->timeslice);

      set_foreach(SET_OPTION_FORWARD, scheduler->active, &i, st)
	{
	  if (set_object(scheduler->active, i, (void**)&queue) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve one of the active queues"
			"from the scheduler");

	  set_foreach(SET_OPTION_FORWARD, *queue, &iq, stq)
	    {
	      if (set_object(*queue, iq, (void**)&entity) != ERROR_OK)
		CORE_ESCAPE("unable to retrieve the thread identifier "
			    "from the queue");

	      entity->timeslice = SCHEDULER_SCALE_TIMESLICE(entity->timeslice);
	    }
	}

      set_foreach(SET_OPTION_FORWARD, scheduler->expired, &i, st)
	{
	  if (set_object(scheduler->expired, i, (void**)&queue) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve one of the expired queues"
			"from the scheduler");

	  set_foreach(SET_OPTION_FORWARD, *queue, &iq, stq)
	    {
	      if (set_object(*queue, iq, (void**)&entity) != ERROR_OK)
		CORE_ESCAPE("unable to retrieve the thread identifier "
			    "from the queue");

	      entity->timeslice = SCHEDULER_SCALE_TIMESLICE(entity->timeslice);
	    }
	}
    }

  /*
   * 2)
   */

  if (machine_call(scheduler, scheduler_quantum, quantum) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*							 [endblock::quantum] */

  CORE_LEAVE();
}

/*
 * this function permits the current task to relinquish the processor
 * voluntary.
 *							[block::yield::comment]
 *
 * steps:
 *
 * 1) XXX
 * 2) prio minimum pour etre sur qu'il ne soit jamais repris (au pire idle sera pris a la place) + timeslice = quantum pour qu'il passe a 0 au prochain click et donc qu'il passe en expired (donc aucune chance qu'il soit repris meme si idle est en expired)
 *						     [endblock::yield::comment]
 */

t_error			scheduler_yield(void)
{
  /*							[block::yield::vars] */

  o_scheduler*		scheduler;

  /*						     [endblock::yield::vars] */

  /*							      [block::yield] */

  /*
   * 1)
   */

  if (scheduler_get(&scheduler) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the current CPU's scheduler");

  /*
   * 2)
   */

  scheduler->priority = SCHEDULER_PRIORITY_LOW;
  scheduler->timeslice = _scheduler->quantum;

  /*
   * 3)
   */

  if (machine_call(scheduler, scheduler_yield) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*							   [endblock::yield] */

  CORE_LEAVE();
}

/*
 * this  function returns  the  identifier of  the currently  executed
 * thread.
 *
 */

t_error			scheduler_current(i_thread*			thread)
{
  /*						      [block::current::vars] */

  o_scheduler*		scheduler;

  /*						   [endblock::current::vars] */

  /*							    [block::current] */

  assert(thread != NULL);

  if (scheduler_get(&scheduler) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the current CPU's scheduler");

  *thread = scheduler->thread;

  /*							 [endblock::current] */

  CORE_LEAVE();
}

/*
 * this function elects the future thread to execute.
 *						       [block::switch::comment]
 *
 * steps:
 *
 * XXX
 *
 *						    [endblock::switch::comment]
 */

t_error			scheduler_elect(void)
{
  /*						       [block::switch::vars] */

  i_thread		current_thread = ID_UNUSED;
  t_priority		current_priority = (t_priority)-1;
  t_timeslice		current_timeslice = (t_timeslice)-1;
  i_thread		future_thread = ID_UNUSED;
  t_priority		future_priority = (t_priority)-1;
  t_timeslice		future_timeslice = (t_timeslice)-1;
  o_scheduler*		scheduler;
  t_priority		priority;
  t_boolean		elected;
  i_set*		queue;
  t_boolean		tried;
  i_cpu			cpu;
  t_state		st;
  t_iterator		i;
  o_task*		t;
  o_thread*		o;

  /*						    [endblock::switch::vars] */

  /*							     [block::switch] */

  /*
   * 1)
   */

  if (scheduler_get(&scheduler) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the current CPU's scheduler");

  if (thread_get(scheduler->thread, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the given thread object");

  if (task_get(o->task, &t) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread's task object");

  /*
   * 2)
   */

  scheduler->timeslice -= _scheduler->quantum;

  /*
   * 3)
   */

  current_thread = scheduler->thread;
  current_timeslice = scheduler->timeslice;
  current_priority = scheduler->priority;

  /*
   * 2)
   */

  if ((t->state == TASK_STATE_START) &&
      (o->state == THREAD_STATE_START) &&
      (current_timeslice == 0))
    {
      o_scheduled	entity;

      entity.thread = current_thread;
      entity.timeslice = SCHEDULER_COMPUTE_TIMESLICE(current_thread);
      current_priority = SCHEDULER_COMPUTE_PRIORITY(current_thread);

      priority = 0;
      set_foreach(SET_OPTION_FORWARD, scheduler->expired, &i, st)
	{
	  if (priority == current_priority)
	    {
	      if (set_object(scheduler->expired,
			     i,
			     (void**)&queue) != ERROR_OK)
		CORE_ESCAPE("unable to retrieve one of the queues"
			    "from the scheduler");

	      if (set_insert(*queue, &entity) != ERROR_OK)
		CORE_ESCAPE("unable to insert the thread identifier "
			    "to the queue");

	      // XXX
	      //printf("[ELECT] saved %qu (%u, %u ms) in expired queue\n",
	      //current_thread, current_priority, current_timeslice);
	      // XXX

	      break;
	    }

	  priority++;
	}
    }

  /*
   * 3)
   */

  elected = BOOLEAN_FALSE;
  tried = BOOLEAN_FALSE;

 try:
  priority = 0;
  set_foreach(SET_OPTION_FORWARD, scheduler->active, &i, st)
    {
      t_iterator	iterator;
      o_scheduled*	highest;
      o_task*		tt;
      o_thread*		oo;

      // XXX si le thread courant n'a pas expire mais qu'il reste le plus
      // prioritaire, ne rien change et le laisse s'executer.
      if ((current_timeslice != 0) &&
	  (priority > current_priority))
	{
	  if (thread_get(current_thread, &oo) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve the thread object");

	  if (task_get(oo->task, &tt) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve the thread's task object");

	  if ((tt->state != TASK_STATE_START) &&
	      (oo->state != THREAD_STATE_START))
	    continue;

	  elected = BOOLEAN_TRUE;

	  future_thread = current_thread;
	  future_priority = current_priority;
	  future_timeslice = current_timeslice;

	  // XXX
	  //printf("[ELECT] elected the current thread\n");
	  // XXX
	}
      else
	{
	  if (set_object(scheduler->active, i, (void**)&queue) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve one of the queues"
			"from the scheduler");

	  if (set_empty(*queue) == ERROR_TRUE)
	    {
	      priority++;
	      continue;
	    }

	  if (set_tail(*queue, &iterator) != ERROR_TRUE)
	    CORE_ESCAPE("unable to locate the the thread with the "
			"highest priority");

	  if (set_object(*queue, iterator, (void**)&highest) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve the thread");

	  if (thread_get(highest->thread, &oo) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve the thread object");

	  if (task_get(oo->task, &tt) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve the thread's task object");

	  if ((tt->state != TASK_STATE_START) &&
	      (oo->state != THREAD_STATE_START))
	    continue;

	  elected = BOOLEAN_TRUE;

	  future_thread = highest->thread;
	  future_priority = priority;
	  future_timeslice = highest->timeslice;

	  if (set_delete(*queue, iterator) != ERROR_OK)
	    CORE_ESCAPE("unable to delete the elected thread from the queue");

	  // XXX
	  //printf("[ELECT] elected the highest active thread %qu\n",
	  //future_thread);
	  // XXX

	  if ((t->state == TASK_STATE_START) &&
	      (o->state == THREAD_STATE_START) &&
	      (current_timeslice != 0)) /* XXX to avoid the thread to be saved twice since if timeslice == 0 it has been saved when entering the elect() function */
	    {
	      o_scheduled	entity;

	      entity.thread = current_thread;
	      entity.timeslice = current_timeslice;
	      current_priority = current_priority;

	      priority = 0;
	      set_foreach(SET_OPTION_FORWARD, scheduler->active, &i, st)
		{
		  if (priority == current_priority)
		    {
		      if (set_object(scheduler->active,
				     i,
				     (void**)&queue) != ERROR_OK)
			CORE_ESCAPE("unable to retrieve one of the queues"
				    "from the scheduler");

		      if (set_insert(*queue, &entity) != ERROR_OK)
			CORE_ESCAPE("unable to insert the thread identifier "
				    "to the queue");

		      // XXX
		      //printf("[ELECT] saved %qu (%u, %u ms) in active queue\n",
		      //current_thread, current_priority,
		      //current_timeslice);
		      // XXX

		      break;
		    }

		  priority++;
		}
	    }
	}

      break;
    }

  /*
   * 4)
   */

  if (elected == BOOLEAN_FALSE)
    {
      i_set		list;

      if (tried == BOOLEAN_TRUE)
	{
	  // XXX
	  //printf("[ELECT] idle\n");
	  // XXX

	  future_thread = _scheduler->idle;
	  future_priority = SCHEDULER_COMPUTE_PRIORITY(future_thread);
	  future_timeslice = _scheduler->quantum;
	}
      else
	{
	  // XXX
	  //printf("[ELECT] switch lists\n");
	  // XXX

	  list = scheduler->active;

	  scheduler->active = scheduler->expired;
	  scheduler->expired = list;

	  tried = BOOLEAN_TRUE;

	  goto try;
	}
    }

  /*
   * 6)
   */

  switch (scheduler->state)
    {
    case SCHEDULER_STATE_START:
      {
	break;
      }
    case SCHEDULER_STATE_STOP:
      {
	o_scheduled	entity;

	entity.thread = future_thread;
	entity.timeslice = future_timeslice;

	priority = 0;
	set_foreach(SET_OPTION_FORWARD, scheduler->active, &i, st)
	  {
	    if (priority == future_priority)
	      {
		if (set_object(scheduler->active,
			       i,
			       (void**)&queue) != ERROR_OK)
		  CORE_ESCAPE("unable to retrieve one of the queues"
			      "from the scheduler");

		if (set_insert(*queue, &entity) != ERROR_OK)
		  CORE_ESCAPE("unable to insert the the thread identifier "
			      "to the queue");

		break;
	      }

	    priority++;
	  }

	future_thread = _kernel->thread;
	future_priority = 0;
	future_timeslice = _scheduler->quantum;

	break;
      }
    }

  /*
   * 7)
   */

  if (machine_call(scheduler, scheduler_elect) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 8)
   */

  scheduler->thread = future_thread;
  scheduler->priority = future_priority;
  scheduler->timeslice = future_timeslice;

  /*
   * 9)
   */

  if (cpu_current(&cpu) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the current CPU object");

  if (cpu_statistics(cpu, _scheduler->quantum) != ERROR_OK)
    CORE_ESCAPE("unable to update the current CPU's statistics");

  /*							  [endblock::switch] */

  CORE_LEAVE();
}

/*
 * this function returns the scheduler object for the current CPU.
 *
 * steps:
 *
 * XXX
 */

t_error			scheduler_get(o_scheduler**		scheduler)
{
  i_cpu			cpu;
  
  /*
   * 1)
   */

  if (cpu_current(&cpu) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the current CPU object");

  if (set_get(_scheduler->cpus, cpu, (void**)scheduler) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the scheduler from the set");

  CORE_LEAVE();
}

/*
 * this function adds a thread to the scheduler.
 *							  [block::add::comment]
 *
 * steps:
 *
 * 1) compute the thread's priority.
 * 2) add the thread to the right queue.
 * 4) call the machine dependent code.
 *
 *						       [endblock::add::comment]
 */

t_error			scheduler_add(i_thread			id)
{
  /*							  [block::add::vars] */

  o_scheduler*		scheduler;
  t_priority		priority;
  o_scheduled		entity;
  i_set*		queue;
  t_state		st;
  t_iterator		i;
  t_priority		p;


  /*						       [endblock::add::vars] */

  /*								[block::add] */

  /*
   * 1)
   */

  // XXX recuperer le scheduler du CPU associe avec le thread

  if (scheduler_get(&scheduler) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the current CPU's scheduler");

  /*
   * 3)
   */

  priority = SCHEDULER_COMPUTE_PRIORITY(id);

  p = 0;
  set_foreach(SET_OPTION_FORWARD, scheduler->active, &i, st)
    {
      if (p == priority)
	{
	  entity.thread = id;
	  entity.timeslice = SCHEDULER_COMPUTE_TIMESLICE(entity.thread);

	  if (set_object(scheduler->active, i, (void**)&queue) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve on of the queues"
			"from the scheduler");

	  if (set_insert(*queue, &entity) != ERROR_OK)
	    CORE_ESCAPE("unable to insert the thread identifier "
			"to the queue");

	  break;
	}

      p++;
    }

  /*
   * 4)
   */

  if (machine_call(scheduler, scheduler_add, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*							     [endblock::add] */

  CORE_LEAVE();
}

/*
 * this function removes a thread from the scheduler.
 *						       [block::remove::comment]
 *
 * steps:
 *
 * 1) XXX
 *						    [endblock::remove::comment]
 */

t_error			scheduler_remove(i_thread		id)
{
  /*						       [block::remove::vars] */

  o_scheduler*		scheduler;
  t_priority		priority;
  i_set*		queue;
  t_state		st;
  t_iterator		i;
  t_priority		p;

  /*						    [endblock::remove::vars] */

  /*							     [block::remove] */

  /*
   * 1)
   */

  // XXX il faut recuperer le scheduler qui correspond a la tache du thread.

  if (scheduler_get(&scheduler) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the current CPU's scheduler");

  /*
   * 3)
   */

  priority = SCHEDULER_COMPUTE_PRIORITY(id);

  /*
   * 4)
   */

  if (scheduler->thread == id)
    goto removed;

  p = 0;
  set_foreach(SET_OPTION_FORWARD, scheduler->active, &i, st)
    {
      if (p == priority)
        {
	  if (set_object(scheduler->active, i, (void**)&queue) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve on of the queues"
			     "from the scheduler");

	  if (set_exist(*queue, id) == ERROR_FALSE)
	    continue;

	  if (set_remove(*queue, id) != ERROR_OK)
	    CORE_ESCAPE("unable to remove the thread from the set");

	  goto removed;
	}

      p++;
    }

  p = 0;
  set_foreach(SET_OPTION_FORWARD, scheduler->expired, &i, st)
    {
      if (p == priority)
	{
	  if (set_object(scheduler->expired, i, (void**)&queue) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve on of the queues"
			"from the scheduler");

	  if (set_exist(*queue, id) == ERROR_FALSE)
	    continue;

	  if (set_remove(*queue, id) != ERROR_OK)
	    CORE_ESCAPE("unable to remove the thread from the set");

	  goto removed;
	}

      p++;
    }

  CORE_ESCAPE("the thread to remove has not been found in any of "
	      "the scheduler's queues");

 removed:

  /*
   * 5)
   */

  if (machine_call(scheduler, scheduler_remove, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 6)
   */

  if (scheduler->thread == id)
    {
      if (scheduler_yield() != ERROR_OK)
	CORE_ESCAPE("unable to yield the execution");
    }

  /*							  [endblock::remove] */

  CORE_LEAVE();
}

/*
 * this function updates a thread into the scheduler, for example because its
 * priority has changed.
 *						       [block::update::comment]
 *
 * steps:
 *
 * 1) remove the thread from the scheduler and adds it back.
 * 2) call the machine dependent code.
 *
 *						    [endblock::update::comment]
 */

t_error			scheduler_update(i_thread			thread)
{
  /*							     [block::update] */

  /*
   * 1)
   */

  // XXX le mettre en stop puis en run -> ca ne marchera surement pas
  // d'autant que si c'est le thread courant, le remove va faire
  // un yield et donc on ne sera plus jamais schedule.
  //
  // il faut simplement faire un yield pour forcer a ce qu'un autre thread
  // soit choisi vu qu'on passera a une petit prio et timeslice=0.
  //
  // une fois le thread replace en liste, il aura sa nouvelle priroite
  // niquel!

  if (scheduler_yield() != ERROR_OK)
    CORE_ESCAPE("unable to yield the execution");

  /*
   * 2)
   */

  if (machine_call(scheduler, scheduler_update, thread) != ERROR_OK)
    CORE_ESCAPE("an erro occured in the machine");

  /*							  [endblock::update] */

  CORE_LEAVE();
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

  o_scheduler*		scheduler;
  i_set			queue;
  t_setsz		ncpus;
  t_iterator		it;
  t_state		st;
  o_cpu*		o;
  t_uint32		i;

  /*
   * 1)
   */

  if ((_scheduler = malloc(sizeof(m_scheduler))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the scheduler "
		"manager structure");

  memset(_scheduler, 0x0, sizeof(m_scheduler));

  _scheduler->quantum = SCHEDULER_QUANTUM;
  _scheduler->cpus = ID_UNUSED;

  /*
   * 2)
   */

  if (set_size(_cpu->cpus, &ncpus) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the number of active CPUs");

  if (set_reserve(array, SET_OPTION_ALLOC, ncpus, sizeof(o_scheduler),
		  &_scheduler->cpus) != ERROR_OK)
    CORE_ESCAPE("unable to reserve a set for the CPUs' schedulers");

  set_foreach(SET_OPTION_FORWARD, _cpu->cpus, &it, st)
    {
      o_scheduler	scheduler;

      if (set_object(_cpu->cpus, it, (void**)&o) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the CPU object");

      scheduler.cpu = o->id;
      scheduler.thread = ID_UNUSED;
      scheduler.timeslice = _scheduler->quantum;
      scheduler.priority = SCHEDULER_N_PRIORITY_QUEUE;
      scheduler.active = ID_UNUSED;
      scheduler.expired = ID_UNUSED;
      scheduler.state = SCHEDULER_STATE_STOP;

      if (set_reserve(array, SET_OPTION_ALLOC, SCHEDULER_N_PRIORITY_QUEUE + 1,
		      sizeof(i_set), &scheduler.active) != ERROR_OK)
	CORE_ESCAPE("unable to reserve a set for the active queues");

      for (i = 0; i <= SCHEDULER_N_PRIORITY_QUEUE; i++)
	{
	  if (set_reserve(ll,
			  SET_OPTION_ALLOC,
			  sizeof(o_scheduled),
			  &queue) != ERROR_OK)
	    CORE_ESCAPE("unable to reserve the active queue of priority "
			"%u",
			i);

	  if (set_insert(scheduler.active, &queue) != ERROR_OK)
	    CORE_ESCAPE("unable to insert the active queue of priority "
			"%u in the list of active queues",
			i);
	}

      if (set_reserve(array,
		      SET_OPTION_ALLOC,
		      SCHEDULER_N_PRIORITY_QUEUE + 1,
		      sizeof(i_set),
		      &scheduler.expired) != ERROR_OK)
	CORE_ESCAPE("unable to reserve a set for the expired queues");

      for (i = 0; i <= SCHEDULER_N_PRIORITY_QUEUE; i++)
	{
	  if (set_reserve(ll,
			  SET_OPTION_ALLOC,
			  sizeof(o_scheduled),
			  &queue) != ERROR_OK)
	    CORE_ESCAPE("unable to reserve the expired queue of priority "
			"%u",
			i);

	  if (set_insert(scheduler.expired, &queue) != ERROR_OK)
	    CORE_ESCAPE("unable to insert the expired queue of priority "
			"%u in the list of expired queues", i);
	}

      if (set_append(_scheduler->cpus, &scheduler) != ERROR_OK)
	CORE_ESCAPE("unable to append the CPU's scheduler to the set");
    }

  /*
   * 3)
   */

  if (scheduler_get(&scheduler) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the current CPU's scheduler");

  scheduler->thread = _kernel->thread;

  /*
   * 4)
   */

  if (machine_call(scheduler, scheduler_initialize) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*						      [endblock::initialize] */

  CORE_LEAVE();
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

  o_scheduler*		scheduler;
  i_set*		queue;
  t_state		stc;
  t_iterator		ic;
  t_state		st;
  t_iterator		i;


  /*
   * 1)
   */

  if (machine_call(scheduler, scheduler_clean) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  set_foreach(SET_OPTION_FORWARD, _scheduler->cpus, &ic, stc)
    {
      if (set_object(_scheduler->cpus, ic, (void**)&scheduler) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the scheduler object");

      set_foreach(SET_OPTION_FORWARD, scheduler->active, &i, st)
	{
	  if (set_object(scheduler->active, i, (void**)&queue) != ERROR_OK)
	    CORE_ESCAPE("unable to retrive an active queue");

	  if (set_release(*queue) != ERROR_OK)
	    CORE_ESCAPE("unable to release the set associated with "
			"the active queue");
	}

      if (set_release(scheduler->active) != ERROR_OK)
	CORE_ESCAPE("unable to release the set of active queues");

      set_foreach(SET_OPTION_FORWARD, scheduler->expired, &i, st)
	{
	  if (set_object(scheduler->expired, i, (void**)&queue) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve an expired queue");

	  if (set_release(*queue) != ERROR_OK)
	    CORE_ESCAPE("unable to release the set associated with "
			"the expired queue");
	}

      if (set_release(scheduler->expired) != ERROR_OK)
	CORE_ESCAPE("unable to release the set of expired queues");
    }

  /*
   * 3)
   */

  free(_scheduler);

  /*							   [endblock::clean] */

  CORE_LEAVE();
}

#endif
