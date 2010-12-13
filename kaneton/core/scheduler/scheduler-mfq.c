/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/scheduler/scheduler-mfq.c
 *
 * created       matthieu bucchianeri   [sat jun  3 22:36:59 2006]
 * updated       julien quintard   [sun dec 12 21:51:50 2010]
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

/*
 * ---------- algorithm -------------------------------------------------------
 *
 * this implementation is referred to as a multilevel feedback queues
 * scheduler.
 *
 * the following explanation stands for a single processor. the mechanism
 * identical for every scheduler of every processor.
 *
 * a scheduler maintains two lists: the active and the expired list. each
 * one contains an equal number of queues (FIFO). the first queue contains
 * the highest priority threads and the last the lowest priority threads.
 *
 * a thread priority is computed based on its parent task priority and
 * the thread's priority itself. the thread's execution timeslice is the
 * time a  thread can be executed. it is computed with the priority:
 * higher priority threads obtain a higher execution timeslice.
 *
 * the quantum corresponds to the interval when the scheduler is called.
 * so the execution timeslices has a granularity equal to this quantum.
 *
 * when the scheduler is called, it verifies that the currently executing
 * thread's timeslice has not expired. if there is no more time for the
 * current thread, it is placed in the expired list, into the queue
 * corresponding to its priority. then, the scheduler looks for the highest
 * priority active thread. it can be the current thread, then no preemption
 * is performed. if multiple threads have the highest priority, then they
 * are taken on a first-come first-served basis.
 *
 * when the active queues become empty, the active and expired list are
 * swapped in order for the scheduler to continue picking thread.
 *
 * note that the machine is responsible for context switching and calling
 * the scheduler_elect() and cpu_balance() functions. for example, these
 * two functions can be called by a timer.
 *
 * finally, should no active or expired thread be found, the scheduler
 * picks the idle thread in order to be sure something is executing.
 *
 * noteworthy is that the idle thread is considered as a normal thread i.e
 * the thread is reserved with a priority---slightly higher than the lowest
 * one---and maintained at all times in the scheduler queues, moving from
 * the active to the expired set. this design decision has an impact on the
 * performance since the idle thread is regularly scheduled though other
 * threads are waiting. however, this is required in order to ensure some
 * functionalities such as forcing a context switch whenever scheduler_yield()
 * is called for instance.
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

extern m_kernel*	_kernel;

/*
 * the cpu manager.
 */

extern m_cpu*		_cpu;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the scheduler manager.
 */

m_scheduler*		_scheduler = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function displays information regarding the given scheduler.
 *
 * steps:
 *
 * 1) retrieve the scheduler object.
 * 2) compute the state string.
 * 3) display the scheduler's attributes.
 * 4) call the machine.
 */

t_error			scheduler_show(i_cpu			id,
				       mt_margin		margin)
{
  o_scheduler*		scheduler;
  char*			state;
  t_priority		priority;
  s_iterator		i;
  t_state		s;

  /*
   * 1)
   */

  if (scheduler_get(id, &scheduler) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the scheduler object");

  /*
   * 2)
   */

  switch (scheduler->state)
    {
    case SCHEDULER_STATE_START:
      {
	state = "start";

	break;
      }
    case SCHEDULER_STATE_STOP:
      {
	state = "stop";

	break;
      }
    default:
      CORE_ESCAPE("unknown scheduler state '%u'",
		  scheduler->state);
    }

  /*
   * 3)
   */

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "scheduler: cpu(%qu) thread(%qu) timeslice(%u) priority(%u) "
	      "state(%s)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      scheduler->cpu,
	      scheduler->thread,
	      scheduler->timeslice,
	      scheduler->priority,
	      state);

  /*
   * 4)
   */

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "  active:\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin));

  priority = 0;
  set_foreach(SET_OPTION_FORWARD, scheduler->active, &i, s)
    {
      i_set*		queue;
      s_iterator	j;
      t_state		t;

      /*
       * a)
       */

      if (set_object(scheduler->active, i, (void**)&queue) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the active queue with "
		    "priority %u from the scheduler",
		    priority);

      /*
       * b)
       */

      set_foreach(SET_OPTION_FORWARD, *queue, &j, t)
	{
	  o_scheduler_candidate	*candidate;

	  /*
	   * i)
	   */

	  if (set_object(*queue, j, (void**)&candidate) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve the thread candidate "
			"from the queue");

	  /*
	   * ii)
	   */

	  module_call(console, message,
		      '#',
		      MODULE_CONSOLE_MARGIN_FORMAT
		      "    thread(%qu) priority(%u) timeslice(%ums)\n",
		      MODULE_CONSOLE_MARGIN_VALUE(margin),
		      candidate->thread,
		      priority,
		      candidate->timeslice);
	}

      /*
       * c)
       */

      priority++;
    }

  /*
   * 5)
   */

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "  expired:\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin));

  priority = 0;
  set_foreach(SET_OPTION_FORWARD, scheduler->expired, &i, s)
    {
      i_set*		queue;
      s_iterator	j;
      t_state		t;

      /*
       * a)
       */

      if (set_object(scheduler->expired, i, (void**)&queue) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the expired queue with "
		    "priority %u from the scheduler",
		    priority);

      /*
       * b)
       */

      set_foreach(SET_OPTION_FORWARD, *queue, &j, t)
	{
	  o_scheduler_candidate	*candidate;

	  /*
	   * i)
	   */

	  if (set_object(*queue, j, (void**)&candidate) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve the thread candidate "
			"from the queue");

	  /*
	   * ii)
	   */

	  module_call(console, message,
		      '#',
		      MODULE_CONSOLE_MARGIN_FORMAT
		      "    thread(%qu) priority(%u) timeslice(%ums)\n",
		      MODULE_CONSOLE_MARGIN_VALUE(margin),
		      candidate->thread,
		      priority,
		      candidate->timeslice);
	}

      /*
       * c)
       */

      priority++;
    }

  /*
   * 6)
   */

  if (machine_call(scheduler, show, id, margin) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function dumps the scheduler manager.
 *
 *							 [block::dump::comment]
 *
 * steps:
 *
 * 1) display general information on the scheduler manager.
 * 2) go through the schedulers and show them.
 *   a) retrieve the scheduler object.
 *   b) show the scheduler object.
 * 3) call the machine.
 *						      [endblock::dump::comment]
 */

t_error			scheduler_dump(void)
{
  /*							 [block::dump::vars] */

  o_scheduler*		scheduler;
  t_setsz		size;
  s_iterator		i;
  t_state		s;

  /*						      [endblock::dump::vars] */

  /*							       [block::dump] */

  /*
   * 1)
   */

  if (set_size(_scheduler->schedulers, &size) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the size of the set of schedulers");

  /*
   * 2)
   */

  module_call(console, message,
	      '#',
	      "scheduler manager: quantum(%u) idle(%qu) #schedulers(%qu)\n",
	      _scheduler->quantum,
	      _scheduler->idle,
	      size);

  /*
   * 3)
   */

  module_call(console, message,
	      '#',
	      "  schedulers:\n");

  set_foreach(SET_OPTION_FORWARD, _scheduler->schedulers, &i, s)
    {
      /*
       * a)
       */

      if (set_object(_scheduler->schedulers,
		     i,
		     (void**)&scheduler) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the scheduler object");

      /*
       * b)
       */

      if (scheduler_show(scheduler->cpu,
			 2 * MODULE_CONSOLE_MARGIN_SHIFT) != ERROR_OK)
	CORE_ESCAPE("unable to show the scheduler");
    }

  /*
   * 4)
   */

  if (machine_call(scheduler, dump) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*							    [endblock::dump] */

  CORE_LEAVE();
}

/*
 * this function starts the given scheduler.
 *
 * steps:
 *
 * 1) retrieve the scheduler object.
 * 2) change the scheduler's state.
 * 3) call the machine.
 */

t_error			scheduler_start(i_cpu			id)
{
  o_scheduler*		scheduler;

  /*
   * 1)
   */

  if (scheduler_get(id, &scheduler) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the scheduler object");

  /*
   * 2)
   */

  scheduler->state = SCHEDULER_STATE_START;

  /*
   * 3)
   */

  if (machine_call(scheduler, start, id) != ERROR_OK)
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

t_error			scheduler_stop(i_cpu			id)
{
  o_scheduler*		scheduler;

  /*
   * 1)
   */

  if (scheduler_get(id, &scheduler) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the scheduler object");

  /*
   * 2)
   */

  scheduler->state = SCHEDULER_STATE_STOP;

  /*
   * 3)
   */

  if (machine_call(scheduler, stop, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function modifies the quantum.
 *
 *						      [block::quantum::comment]
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) update the quantum.
 * 2) go through the CPUs' schedulers.
 *   a) retrieve the scheduler object.
 *   b) re-compute the currently scheduled thread's timeslice.
 *   c) go through the scheduler's active queues.
 *     i) retrieve the queue object.
 *     ii) go through the thread of this queue.
 *       #1) retrieve the thread object.
 *       #2) re-compute the thread's timeslice.
 *   d) go through the scheduler's expired queues.
 *     i) retrieve the queue object.
 *     ii) go through the thread of this queue.
 *       #1) retrieve the thread object.
 *       #2) re-compute the thread's timeslice.
 * 3) call the machine.
 *
 *						   [endblock::quantum::comment]
 */

t_error			scheduler_quantum(t_quantum		quantum)
{
  /*						      [block::quantum::vars] */

  s_iterator		i;
  t_state		s;

  /*						   [endblock::quantum::vars] */

  /*							    [block::quantum] */

  /*
   * 0)
   */

  if (quantum == 0)

    CORE_ESCAPE("unable to update the quantum to zero");

  /*
   * 1)
   */

  _scheduler->quantum = quantum;

  /*
   * 2)
   */

  set_foreach(SET_OPTION_FORWARD, _scheduler->schedulers, &i, s)
    {
      o_scheduler*		scheduler;
      s_iterator		j;
      t_state			t;

      /*
       * a)
       */

      if (set_object(_scheduler->schedulers,
		     i,
		     (void**)&scheduler) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the scheduler from the set");

      /*
       * b)
       */

      scheduler->timeslice = SCHEDULER_SCALE(scheduler->timeslice);

      /*
       * c)
       */

      set_foreach(SET_OPTION_FORWARD, scheduler->active, &j, t)
	{
	  i_set*		queue;
	  s_iterator		k;
	  t_state		u;

	  /*
	   * i)
	   */

	  if (set_object(scheduler->active, j, (void**)&queue) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve one of the active queues"
			"from the scheduler");

	  /*
	   * ii)
	   */

	  set_foreach(SET_OPTION_FORWARD, *queue, &k, u)
	    {
	      o_scheduler_candidate*	candidate;

	      /*
	       * #1)
	       */

	      if (set_object(*queue, k, (void**)&candidate) != ERROR_OK)
		CORE_ESCAPE("unable to retrieve the thread identifier "
			    "from the queue");

	      /*
	       * #2)
	       */

	      candidate->timeslice = SCHEDULER_SCALE(candidate->timeslice);
	    }
	}

      /*
       * d)
       */

      set_foreach(SET_OPTION_FORWARD, scheduler->expired, &j, t)
	{
	  i_set*		queue;
	  s_iterator		k;
	  t_state		u;

	  /*
	   * i)
	   */

	  if (set_object(scheduler->expired, j, (void**)&queue) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve one of the expired queues"
			"from the scheduler");

	  /*
	   * ii)
	   */

	  set_foreach(SET_OPTION_FORWARD, *queue, &k, u)
	    {
	      o_scheduler_candidate*	candidate;

	      /*
	       * #1)
	       */

	      if (set_object(*queue, k, (void**)&candidate) != ERROR_OK)
		CORE_ESCAPE("unable to retrieve the thread identifier "
			    "from the queue");

	      /*
	       * #2)
	       */

	      candidate->timeslice = SCHEDULER_SCALE(candidate->timeslice);
	    }
	}
    }

  /*
   * 3)
   */

  if (machine_call(scheduler, quantum, quantum) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*							 [endblock::quantum] */

  CORE_LEAVE();
}

/*
 * this function enables the current thread to voluntarily relinquish its
 * execution, hence permitting another thread to be scheduled immediately
 * on this CPU.
 *
 * note that this function has been designed in order to ensure a context
 * switch.
 *
 *							[block::yield::comment]
 *
 * steps:
 *
 * 1) retrieve the current scheduler.
 * 2) set the current thread's priority to the lowest possible and its
 *    timeslice to the quantum. this way, the next call to scheduler_elect(),
 *    which will probably occur at the next tick, will have no choice but to
 *    elect another thread since the current thread timeslice will fall
 *    to zero: quantum - quantum = zero. besides, setting the thread's priority
 *    to low ensures that another thread will be picked from the expired
 *    set should no active thread be waiting for execution. in the worst
 *    case scenario in which this thread is the only one, the idle thread
 *    will be picked because the idle thread has a priority slightly higher
 *    than the lowest.
 *    the design choice consisting in maintaining the idle thread in the
 *    scheduler queues at all times has been made for this very reason i.e
 *    in order to ensure that a thread can be scheduled with a higher
 *    priority than the lowest one.
 *    note that changing the priority is not permanent since the
 *    scheduler_elect() recomputes the priorities of expired threads.
 * 3) call the machine.
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

  if (scheduler_current(&scheduler) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the current CPU's scheduler");

  /*
   * 2)
   */

  scheduler->priority = SCHEDULER_PRIORITY_LOW;
  scheduler->timeslice = _scheduler->quantum;

  /*
   * 3)
   */

  if (machine_call(scheduler, yield) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*							   [endblock::yield] */

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
 * should both active and expired sets be empty, the function elects
 * the special idle thread in order to give the CPU something to do.
 *
 * finally, this function may detect that the scheduler has been stopped.
 * should this occur, the elected thread is saved and the original kernel
 * thread is specially scheduled, hence returning to its initial state.
 *
 *						       [block::switch::comment]
 *
 * steps:
 *
 * 1) retrieve the current scheduler, task and thread objects.
 * 2) decrease the currently scheduled thread's timeslice by a quantum.
 * 3) save the current thread identifier, timeslice and priority.
 * 4) if the current thread's timeslice has reached zero, save it in
 *    its proper priority-based queue...
 *   a) build the appropriate scheduler candidate based on the current thread.
 *   b) compute the new priority should the task or thread priorites have
 *      changed.
 *   c) add the candidate to the queue assiociated with its priority.
 * 5) initialize the election process.
 * 6) go through the set of active queues.
 *   a) try to locate an eligeable thread depending on the current queue's
 *      priority.
 *     A) the current queue's priority is lower than the current thread's
 *        and the current thread still has some timeslices. therefore, the
 *        current thread seem to be the active thread with the highest
 *        priority, hence should be elected.
 *        besides, the thread's task and the thread itself are running
 *        and are therefore perfectly eligeable.
 *       i) set the future thread's attribute as being the current one's.
 *       ii) a thread has been elected.
 *       iii) therefore the election process can be stopped by exiting the
 *            loop through the active queues.
 *     B) otherwise, search for another thread.
 *       i) retrieve the current queue.
 *       ii) if the queue is empty, move on to the next one.
 *       iii) retrieve from the queue the identifier of the thread with
 *            seniority.
 *       iv) retrieve the thread candidate.
 *       v) retrieve both the thread and task objects corresponding to
 *          this candidate.
 *       vi) if either the task or the thread is not running, ignore this
 *           candidate. note that the thread is not removed since, if the
 *           thread is still in the queue, it probably means that it is
 *           being removed right now but an election occured in between.
 *       vii) set the future thread's attribute as being the found candidate.
 *       viii) a thread has been elected.
 *       ix) remove the thread candidate from its queue.
 *       x) if the currently scheduled thread and its task are running
 *           and the task has not been saved in its queue previously
 *           (in 4) i.e the thread still has remaining timeslices.
 *         #1) build a candidate for the currently running thread based
 *             on its thread identifier and its remaining timeslices.
 *         #2) save the candidate in the queue associated with its current
 *             priority.
 *       xi) at this point the election process is over: exit the loop.
 * 7) if not thread has been elected...
 *   a) either this is the second time an election has been tried, in which
 *      case the idle thread is elected since this situation means that
 *      no threads are left in the queues.
 *   b) or this is the first time and only the active set has been explored.
 *     i) swap the active and expired sets.
 *     ii) save the fact that a swap has been performed, hence that the
 *         next time the function will end up in this situation, both
 *         active and expired sets will have been explored.
 *     iii) try again with the expired set.
 * 8) now that a thread has been elected, check the scheduler's state.
 *   A) if the scheduler is still running, keep it that way.
 *   B) if the scheduler has been stopped...
 *     a) build a candidate for the elected thread since the system is about
 *        to return to its initial state.
 *     b) save the candidate in its priority queue.
 *     c) elect the kernel thread as the elected one in order to jump
 *        back to the initial state i.e before the schedulers were
 *        started.
 * 9) call the machine.
 * 10) install the elected thread as the scheduler's current thread.
 * 11) retrieve the current CPU object and update its statistics.
 *
 *						    [endblock::switch::comment]
 */

t_error			scheduler_elect(void)
{
  /*						       [block::switch::vars] */

  i_thread		current_thread;
  t_priority		current_priority;
  t_timeslice		current_timeslice;
  i_thread		future_thread;
  t_priority		future_priority;
  t_timeslice		future_timeslice;
  o_scheduler*		scheduler;
  t_priority		priority;
  t_boolean		elected;
  t_boolean		tried;
  o_thread*		thread;
  o_task*		task;
  i_cpu			cpu;
  s_iterator		i;
  t_state		s;

  /*						    [endblock::switch::vars] */

  /*							     [block::switch] */

  /*
   * 1)
   */

  if (scheduler_current(&scheduler) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the current CPU's scheduler");

  if (thread_get(scheduler->thread, &thread) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the given thread object");

  if (task_get(thread->task, &task) != ERROR_OK)
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
   * 4)
   */

  if ((task->state == TASK_STATE_START) &&
      (thread->state == THREAD_STATE_START) &&
      (current_timeslice == 0))
    {
      o_scheduler_candidate	candidate;

      /*
       * a)
       */

      candidate.thread = current_thread;
      candidate.timeslice = SCHEDULER_TIMESLICE(candidate.thread);

      /*
       * b)
       */

      current_priority = SCHEDULER_PRIORITY(current_thread);

      /*
       * c)
       */

      SCHEDULER_QUEUE(scheduler->expired, candidate, current_priority);
    }

  /*
   * 5)
   */

  elected = BOOLEAN_FALSE;
  tried = BOOLEAN_FALSE;

  /*
   * 6)
   */

 try:

  priority = 0;

  set_foreach(SET_OPTION_FORWARD, scheduler->active, &i, s)
    {
      /*
       * a)
       */

      if ((current_timeslice != 0) &&
	  (priority > current_priority) &&
	  (task->state == TASK_STATE_START) &&
	  (thread->state == THREAD_STATE_START))
	{
	  /*
	   * A)
	   */

	  /*
	   * i)
	   */

	  future_thread = current_thread;
	  future_priority = current_priority;
	  future_timeslice = current_timeslice;

	  /*
	   * ii)
	   */

	  elected = BOOLEAN_TRUE;

	  /*
	   * iii)
	   */

	  break;
	}
      else
	{
	  o_scheduler_candidate*candidate;
	  s_iterator		iterator;
	  i_set*		queue;
	  o_task*		a;
	  o_thread*		h;

	  /*
	   * B)
	   */

	  /*
	   * i)
	   */

	  if (set_object(scheduler->active, i, (void**)&queue) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve one of the queues"
			"from the scheduler");

	  /*
	   * ii)
	   */

	  if (set_empty(*queue) == ERROR_TRUE)
	    {
	      priority++;

	      continue;
	    }

	  /*
	   * iii)
	   */

	  if (set_tail(*queue, &iterator) != ERROR_TRUE)
	    CORE_ESCAPE("unable to locate the the thread with the "
			"highest seniority");

	  /*
	   * iv)
	   */

	  if (set_object(*queue, iterator, (void**)&candidate) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve the thread");

	  /*
	   * v)
	   */

	  if (thread_get(candidate->thread, &h) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve the thread object");

	  if (task_get(h->task, &a) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve the thread's task object");

	  /*
	   * vi)
	   */

	  if ((a->state != TASK_STATE_START) ||
	      (h->state != THREAD_STATE_START))
	    continue;

	  /*
	   * vii)
	   */

	  future_thread = candidate->thread;
	  future_priority = priority;
	  future_timeslice = candidate->timeslice;

	  /*
	   * viii)
	   */

	  elected = BOOLEAN_TRUE;

	  /*
	   * ix)
	   */

	  if (set_delete(*queue, iterator) != ERROR_OK)
	    CORE_ESCAPE("unable to delete the elected thread from the queue");

	  /*
	   * x)
	   */

	  if ((task->state == TASK_STATE_START) &&
	      (thread->state == THREAD_STATE_START) &&
	      (current_timeslice != 0))
	    {
	      o_scheduler_candidate	candidate;

	      /*
	       * #1)
	       */

	      candidate.thread = current_thread;
	      candidate.timeslice = current_timeslice;

	      /*
	       * #2)
	       */

	      SCHEDULER_QUEUE(scheduler->active, candidate, current_priority);
	    }

	  /*
	   * xi)
	   */

	  break;
	}
    }

  /*
   * 7)
   */

  if (elected == BOOLEAN_FALSE)
    {
      i_set		list;

      /*
       * a)
       */

      if (tried == BOOLEAN_TRUE)
	{
	  /*
	   * A)
	   */

	  /*
	   * i)
	   */

	  future_thread = _scheduler->idle;
	  future_priority = SCHEDULER_PRIORITY(future_thread);
	  future_timeslice = _scheduler->quantum;
	}
      else
	{
	  /*
	   * B)
	   */

	  /*
	   * i)
	   */

	  list = scheduler->active;

	  scheduler->active = scheduler->expired;

	  scheduler->expired = list;

	  /*
	   * ii)
	   */

	  tried = BOOLEAN_TRUE;

	  /*
	   * iii)
	   */

	  goto try;
	}
    }

  /*
   * 8)
   */

  switch (scheduler->state)
    {
    case SCHEDULER_STATE_START:
      {
	/*
	 * A)
	 */

	break;
      }
    case SCHEDULER_STATE_STOP:
      {
	/*
	 * B)
	 */

	o_scheduler_candidate	candidate;

	/*
	 * a)
	 */

	candidate.thread = future_thread;
	candidate.timeslice = future_timeslice;

	/*
	 * b)
	 */

	SCHEDULER_QUEUE(scheduler->active, candidate, future_priority);

	/*
	 * c)
	 */

	future_thread = _kernel->thread;
	future_priority = 0;
	future_timeslice = _scheduler->quantum;

	break;
      }
    default:
      CORE_ESCAPE("unknown scheduler state '%u'",
		  scheduler->state);
    }

  /*
   * 9)
   */

  if (machine_call(scheduler, elect) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 10)
   */

  scheduler->thread = future_thread;
  scheduler->priority = future_priority;
  scheduler->timeslice = future_timeslice;

  /*
   * 11)
   */

  if (cpu_current(&cpu) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the current CPU object");

  if (cpu_update(cpu, _scheduler->quantum) != ERROR_OK)
    CORE_ESCAPE("unable to update the current CPU's statistics");

  /*							  [endblock::switch] */

  CORE_LEAVE();
}

/*
 * this function adds a thread to its task's scheduler.
 *							  [block::add::comment]
 *
 * steps:
 *
 * 1) retrieve the thread, its task and the task's scheduler objects.
 * 2) build a candidate for the thread.
 * 3) compute the thread's priority and add the candidate to its queue.
 * 4) call the machine.
 *
 *						       [endblock::add::comment]
 */

t_error			scheduler_add(i_thread			id)
{
  /*							  [block::add::vars] */

  o_thread*		thread;
  o_task*		task;
  o_scheduler*		scheduler;
  t_priority		priority;
  o_scheduler_candidate	candidate;

  /*						       [endblock::add::vars] */

  /*								[block::add] */

  /*
   * 1)
   */

  if (thread_get(id, &thread) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  if (task_get(thread->task, &task) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  if (scheduler_get(task->cpu, &scheduler) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the scheduler object");

  /*
   * 2)
   */

  candidate.thread = id;
  candidate.timeslice = SCHEDULER_TIMESLICE(candidate.thread);

  /*
   * 3)
   */

  priority = SCHEDULER_PRIORITY(thread->id);

  SCHEDULER_QUEUE(scheduler->active, candidate, priority);

  /*
   * 4)
   */

  if (machine_call(scheduler, add, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*							     [endblock::add] */

  CORE_LEAVE();
}

/*
 * this function removes a thread from its task's scheduler.
 *
 *						       [block::remove::comment]
 *
 * steps:
 *
 * 1) call the machine.
 * 2) retrieve the thread, task and scheduler objects.
 * 3) if the thread to remove is the one currently executed by the scheduler,
 *    ignore the phase consisting in remove the thread from the scheduler's
 *    queues since it is not in any queues (i.e move to the step 5).
 * 4) remove the thread from one of the scheduler's queues.
 * 5) if the thread to remove is the one currently being executed, yield
 *    the execution in order to trigger a context switch, hence leading
 *    to the removal of the current thread since, assuming the thread is
 *    not in a running state, the scheduler will never re-inject it in
 *    its queues.
 *
 *						    [endblock::remove::comment]
 */

t_error			scheduler_remove(i_thread		id)
{
  /*						       [block::remove::vars] */

  o_thread*		thread;
  o_task*		task;
  o_scheduler*		scheduler;
  t_priority		priority;
  t_priority		p;

  /*						    [endblock::remove::vars] */

  /*							     [block::remove] */

  /*
   * 1)
   */

  if (machine_call(scheduler, remove, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  if (thread_get(id, &thread) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  if (task_get(thread->task, &task) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  if (scheduler_get(task->cpu, &scheduler) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the scheduler object");

  /*
   * 3)
   */

  if (scheduler->thread == id)
    goto removed;

  /*
   * 4)
   */

  SCHEDULER_UNQUEUE(scheduler, id, removed);

 removed:

  /*
   * 5)
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
 *
 *						       [block::update::comment]
 *
 * steps:
 *
 * 1) retrieve the thread, task and scheduler objects.
 * 2) update the scheduler's queues depending on the thread identifier.
 *   A) if the thread to update is the current thread...
 *     a) this means the thread is not saved in the scheduler queue's.
 *        therefore its current priority can simply be updated directly
 *        along with its timeslice.
 *   B) otherwise, the thread must be somewhere in the queues.
 *     a) go through the active queues.
 *       i) retrieve the queue object.
 *       ii) go through the thread candidates belonging to this queue.
 *         #1) retrieve the thread candidate object.
 *         #2) if the candidate is not the thread looking for, continue
 *             with the next thread candidate from this queue.
 #         #3) compute the new thread's priority.
 *         #4) if the new thread's priority is the same as the current
 *             queue's priority, the thread is at its right place; hence
 *             stop here.
 *         #5) otherwise, the thread must be moved to a different active
 *             queue: update the candidate's timeslice.
 *         #6) add the thread to the proper queue i.e the one corresponding
 *             to its new priority.
 *         #7) remove the thread from this queue.
 *         #8) the process is complete.
 *     b) go through the expired queues.
 *       i) retrieve the queue object.
 *       ii) go through the thread candidates belonging to this queue.
 *         #1) retrieve the thread candidate object.
 *         #2) if the candidate is not the thread looking for, continue
 *             with the next thread candidate from this queue.
 #         #3) compute the new thread's priority.
 *         #4) if the new thread's priority is the same as the current
 *             queue's priority, the thread is at its right place; hence
 *             stop here.
 *         #5) otherwise, the thread must be moved to a different active
 *             queue: update the candidate's timeslice.
 *         #6) add the thread to the proper queue i.e the one corresponding
 *             to its new priority.
 *         #7) remove the thread from this queue.
 *         #8) the process is complete.
 * 3) if the function reaches this point, the thread has not been found: an
 *    error is therefore returned.
 * 4) call the machine.
 *
 *						    [endblock::update::comment]
 */

t_error			scheduler_update(i_thread		id)
{
  /*							     [block::update] */

  o_thread*		thread;
  o_task*		task;
  o_scheduler*		scheduler;
  t_priority		p;

  /*
   * 1)
   */

  if (thread_get(id, &thread) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  if (task_get(thread->task, &task) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  if (scheduler_get(task->cpu, &scheduler) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the scheduler object");

  /*
   * 2)
   */

  if (scheduler->thread == id)
    {
      /*
       * A)
       */

      /*
       * a)
       */

      scheduler->priority = SCHEDULER_PRIORITY(id);
      scheduler->timeslice = SCHEDULER_SCALE(scheduler->timeslice);
    }
  else
    {
      /*
       * B)
       */

      s_iterator	i;
      t_state		s;

      /*
       * a)
       */

      p = 0;

      set_foreach(SET_OPTION_FORWARD, scheduler->active, &i, s)
	{
	  i_set*	queue;
	  s_iterator	j;
	  t_state	t;

	  /*
	   * i)
	   */

	  if (set_object(scheduler->active,
			 i,
			 (void**)&queue) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve the queue from the "
			"active set");

	  /*
	   * ii)
	   */

	  set_foreach(SET_OPTION_FORWARD, *queue, &j, t)
	    {
	      o_scheduler_candidate	*candidate;
	      t_priority		priority;

	      /*
	       * #1)
	       */

	      if (set_object(*queue, j, (void**)&candidate) != ERROR_OK)
		CORE_ESCAPE("unable to retrieve the thread candidate "
			    "from the queue");

	      /*
	       * #2)
	       */

	      if (id != candidate->thread)
		continue;

	      /*
	       * #3)
	       */

	      priority = SCHEDULER_PRIORITY(id);

	      /*
	       * #4)
	       */

	      if (p == priority)
		goto updated;

	      /*
	       * #5)
	       */

	      candidate->timeslice = SCHEDULER_SCALE(candidate->timeslice);

	      /*
	       * #6)
	       */

	      SCHEDULER_QUEUE(scheduler->active, candidate, priority);

	      /*
	       * #7)
	       */

	      if (set_remove(*queue, id) != ERROR_OK)
		CORE_ESCAPE("unable to remove the thread from the set");

	      /*
	       * #8)
	       */

	      goto updated;
	    }

	  /*
	   * iii)
	   */

	  p++;
	}

      /*
       * b)
       */

      p = 0;

      set_foreach(SET_OPTION_FORWARD, scheduler->expired, &i, s)
	{
	  i_set*	queue;
	  s_iterator	j;
	  t_state	t;

	  /*
	   * i)
	   */

	  if (set_object(scheduler->expired,
			 i,
			 (void**)&queue) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve the queue from the "
			"expired set");

	  /*
	   * ii)
	   */

	  set_foreach(SET_OPTION_FORWARD, *queue, &j, t)
	    {
	      o_scheduler_candidate	*candidate;
	      t_priority		priority;

	      /*
	       * #1)
	       */

	      if (set_object(*queue, j, (void**)&candidate) != ERROR_OK)
		CORE_ESCAPE("unable to retrieve the thread candidate "
			    "from the queue");

	      /*
	       * #2)
	       */

	      if (id != candidate->thread)
		continue;

	      /*
	       * #3)
	       */

	      priority = SCHEDULER_PRIORITY(id);

	      /*
	       * #4)
	       */

	      if (p == priority)
		goto updated;

	      /*
	       * #5)
	       */

	      candidate->timeslice = SCHEDULER_SCALE(candidate->timeslice);

	      /*
	       * #6)
	       */

	      SCHEDULER_QUEUE(scheduler->expired, candidate, priority);

	      /*
	       * #7)
	       */

	      if (set_remove(*queue, id) != ERROR_OK)
		CORE_ESCAPE("unable to remove the thread from the set");

	      /*
	       * #8)
	       */

	      goto updated;
	    }

	  /*
	   * iii)
	   */

	  p++;
	}
    }

  /*
   * 3)
   */

  CORE_ESCAPE("the thread to update has not been found in the scheduler");

 updated:

  /*
   * 4)
   */

  if (machine_call(scheduler, update, id) != ERROR_OK)
    CORE_ESCAPE("an erro occured in the machine");

  /*							  [endblock::update] */

  CORE_LEAVE();
}

/*
 * this function returns true if the scheduler for the given CPU identifier
 * exists.
 */

t_error			scheduler_exist(i_cpu			id)
{
  if (set_exist(_scheduler->schedulers, id) != ERROR_TRUE)
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

t_error			scheduler_get(i_cpu			id,
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

  if (set_get(_scheduler->schedulers, id, (void**)object) != ERROR_OK)
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

t_error			scheduler_current(o_scheduler**		scheduler)
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

  if (cpu_current(&cpu) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the current CPU object");

  /*
   * 2)
   */

  if (set_get(_scheduler->schedulers, cpu, (void**)scheduler) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the scheduler from the set");

  CORE_LEAVE();
}

/*
 * this function initializes the scheduler manager.
 *
 *						   [block::initialize::comment]
 *
 * steps:
 *
 * 1) allocate and initialize the manager's structure.
 * 2) initialize the quantum.
 * 3) retrieve the number of CPUs.
 * 4) reserve the set of schedulers.
 * 5) go through the CPUs.
 *   a) retrieve the CPU object.
 *   b) build the scheduler object.
 *   c) reserve a set of active queues.
 *   d) for every priority...
 *     i) reserve a set of the queue of a specific priority.
 *     ii) insert the reserved queue in the set of active queues.
 *   e) reserve a set of expired queues.
 *   f) for every priority...
 *     i) reserve a set of the queue of a specific priority.
 *     ii) insert the reserved queue in the set of active queues.
 *   g) add the scheduler to the set of schedulers.
 * 6) retrieve the currently running scheduler.
 * 7) set the scheduler's current thread as being the kernel thread.
 * 8) call the machine.
 *
 *						[endblock::initialize::comment]
 */

t_error			scheduler_initialize(void)
{
  /*							 [block::initialize] */

  o_scheduler*		scheduler;
  i_set			queue;
  t_setsz		ncpus;
  s_iterator		it;
  t_state		st;
  o_cpu*		o;
  t_uint32		i;

  /*
   * 1)
   */

  if ((_scheduler = malloc(sizeof (m_scheduler))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the scheduler "
		"manager structure");

  memset(_scheduler, 0x0, sizeof (m_scheduler));

  /*
   * 2)
   */

  _scheduler->quantum = SCHEDULER_QUANTUM;

  /*
   * 3)
   */

  if (set_size(_cpu->cpus, &ncpus) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the number of active CPUs");

  /*
   * 4)
   */

  if (set_reserve(array,
		  SET_OPTION_ALLOCATE,
		  ncpus,
		  sizeof (o_scheduler),
		  &_scheduler->schedulers) != ERROR_OK)
    CORE_ESCAPE("unable to reserve a set for the schedulers");

  /*
   * 5)
   */

  set_foreach(SET_OPTION_FORWARD, _cpu->cpus, &it, st)
    {
      o_scheduler	scheduler;

      /*
       * a)
       */

      if (set_object(_cpu->cpus, it, (void**)&o) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the CPU object");

      /*
       * b)
       */

      scheduler.cpu = o->id;
      scheduler.thread = ID_UNUSED;
      scheduler.timeslice = _scheduler->quantum;
      scheduler.priority = SCHEDULER_NPRIORITIES;
      scheduler.active = ID_UNUSED;
      scheduler.expired = ID_UNUSED;
      scheduler.state = SCHEDULER_STATE_STOP;

      /*
       * c)
       */

      if (set_reserve(array,
		      SET_OPTION_ALLOCATE,
		      SCHEDULER_NPRIORITIES + 1,
		      sizeof (i_set),
		      &scheduler.active) != ERROR_OK)
	CORE_ESCAPE("unable to reserve a set for the active queues");

      /*
       * d)
       */

      for (i = 0; i <= SCHEDULER_NPRIORITIES; i++)
	{
	  /*
	   * i)
	   */

	  if (set_reserve(ll,
			  SET_OPTION_ALLOCATE,
			  sizeof (o_scheduler_candidate),
			  &queue) != ERROR_OK)
	    CORE_ESCAPE("unable to reserve the active queue of priority "
			"%u",
			i);

	  /*
	   * ii)
	   */

	  if (set_insert(scheduler.active, &queue) != ERROR_OK)
	    CORE_ESCAPE("unable to insert the active queue of priority "
			"%u in the list of active queues",
			i);
	}

      /*
       * e)
       */

      if (set_reserve(array,
		      SET_OPTION_ALLOCATE,
		      SCHEDULER_NPRIORITIES + 1,
		      sizeof (i_set),
		      &scheduler.expired) != ERROR_OK)
	CORE_ESCAPE("unable to reserve a set for the expired queues");

      /*
       * f)
       */

      for (i = 0; i <= SCHEDULER_NPRIORITIES; i++)
	{
	  /*
	   * i)
	   */

	  if (set_reserve(ll,
			  SET_OPTION_ALLOCATE,
			  sizeof (o_scheduler_candidate),
			  &queue) != ERROR_OK)
	    CORE_ESCAPE("unable to reserve the expired queue of priority "
			"%u",
			i);

	  /*
	   * ii)
	   */

	  if (set_insert(scheduler.expired, &queue) != ERROR_OK)
	    CORE_ESCAPE("unable to insert the expired queue of priority "
			"%u in the list of expired queues", i);
	}

      /*
       * g)
       */

      if (set_append(_scheduler->schedulers, &scheduler) != ERROR_OK)
	CORE_ESCAPE("unable to append the CPU's scheduler to the set");
    }

  /*
   * 6)
   */

  if (scheduler_current(&scheduler) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the current CPU's scheduler");

  /*
   * 7)
   */

  scheduler->thread = _kernel->thread;

  /*
   * 8)
   */

  if (machine_call(scheduler, initialize) != ERROR_OK)
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
 * 1) call the machine.
 * 2) for every scheduler...
 *   a) retrieve the scheduler object.
 *   b) for every active queue.
 *     i) retrieve the queue object.
 *     ii) release the set corresponding to the queue.
 *   c) release the set of active queues.
 *   d) for every expired queue.
 *     i) retrieve the queue object.
 *     ii) release the set corresponding to the queue.
 *   e) release the set of expired queues.
 * 3) free the manager's structure.
 *
 *						     [endblock::clean::comment]
 */

t_error			scheduler_clean(void)
{
  /*							      [block::clean] */

  o_scheduler*		scheduler;
  i_set*		queue;
  t_state		s;
  s_iterator		i;
  t_state		t;
  s_iterator		j;

  /*
   * 1)
   */

  if (machine_call(scheduler, clean) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  set_foreach(SET_OPTION_FORWARD, _scheduler->schedulers, &i, s)
    {
      /*
       * a)
       */

      if (set_object(_scheduler->schedulers,
		     i,
		     (void**)&scheduler) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the scheduler object");

      /*
       * b)
       */

      set_foreach(SET_OPTION_FORWARD, scheduler->active, &j, t)
	{
	  /*
	   * i)
	   */

	  if (set_object(scheduler->active, i, (void**)&queue) != ERROR_OK)
	    CORE_ESCAPE("unable to retrive an active queue");

	  /*
	   * ii)
	   */

	  if (set_release(*queue) != ERROR_OK)
	    CORE_ESCAPE("unable to release the set associated with "
			"the active queue");
	}

      /*
       * c)
       */

      if (set_release(scheduler->active) != ERROR_OK)
	CORE_ESCAPE("unable to release the set of active queues");

      /*
       * d)
       */

      set_foreach(SET_OPTION_FORWARD, scheduler->expired, &j, t)
	{
	  /*
	   * i)
	   */

	  if (set_object(scheduler->expired, i, (void**)&queue) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve an expired queue");

	  /*
	   * ii)
	   */

	  if (set_release(*queue) != ERROR_OK)
	    CORE_ESCAPE("unable to release the set associated with "
			"the expired queue");
	}

      /*
       * e)
       */

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
