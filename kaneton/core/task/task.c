/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo.../kaneton.STABLE/kaneton/core/task/task.c
 *
 * created       julien quintard   [fri jun 22 02:25:26 2007]
 * updated       julien quintard   [fri dec  3 16:21:14 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the task manager manages tasks.
 *
 * a task is  composed of the process control  block (task id, parent,
 * children...), a priority used  for the scheduling, an address space
 * and a set of waits.
 *
 * XXX adapt for cpu use.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

machine_include(task);

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the kernel manager.
 */

extern m_kernel*	_kernel;

/*
 * the init variable, filled by the bootloader, containing in this case
 * the list of segments to mark used.
 */

extern t_init*		_init;

/*
 * the scheduler manager.
 */

extern m_scheduler*	_scheduler;

/*
 * the identifier of the pre-reserved segment containing the system service
 * code.
 */

extern i_segment	_system;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the task manager variable.
 */

m_task*			_task = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function returns the current task.
 */

t_error			task_current(i_task*			tsk)
{
  i_thread		current;
  o_thread*		o;

  assert(tsk != NULL);

  if (_scheduler == NULL)
    {
      *tsk = _kernel->task;

      CORE_LEAVE();
    }

  if (scheduler_current(&current) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the currently scheduled thread");

  if (thread_get(current, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  *tsk = o->task;

  CORE_LEAVE();
}

/*
 * this function shows a precise task.
 */

t_error			task_show(i_task			id)
{
  o_task*		o;
  char*			state;

  if (task_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  switch (o->state)
    {
      case TASK_STATE_START:
	{
	  state = "started";

	  break;
	}
      case TASK_STATE_STOP:
	{
	  state = "stopped";

	  break;
	}
      case TASK_STATE_BLOCK:
	{
	  state = "blocked";

	  break;
	}
      case TASK_STATE_ZOMBIE:
	{
	  state = "zombie";

	  break;
	}
      case TASK_STATE_DEAD:
	{
	  state = "dead";

	  break;
	}
    default:
      CORE_ESCAPE("unknown task state '%u'",
		  o->state);
    }

  module_call(console, console_message,
	      '#', "  task %qu on cpu %qd is %s\n",
	      id, o->cpu, state);

  if (machine_call(task, task_show, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function dumps the tasks managed by the kernel.
 *
 * steps:
 *
 * 1) gets the set's size.
 * 2) for each task, show it.
 */

t_error			task_dump(void)
{
  t_state		state;
  o_task*		data;
  t_setsz		size;
  t_iterator		i;

  /*
   * 1)
   */

  if (set_size(_task->tasks, &size) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the size of the set of tasks");

  /*
   * 2)
   */

  module_call(console, console_message,
	      '#', "dumping %qu task(s):\n", size);

  set_foreach(SET_OPTION_FORWARD, _task->tasks, &i, state)
    {
      if (set_object(_task->tasks, i, (void**)&data) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the task object");

      if (task_show(data->id) != ERROR_OK)
	CORE_ESCAPE("unable to show the task");
    }

  CORE_LEAVE();
}

/*
 * this function reserves a task object.
 *
 * steps:
 *
 * 1) checks the validity of arguments.
 * 2) initializes the task object.
 * 3) reserves an identifier for the task object.
 * 4) get the parent task and add the created task to its children set.
 * 5) reserves the set of threads for the new task object.
 * 6) reserves the set of waits for the new task object.
 * 7) reserves the set of children for the new task object.
 * 8) create a message box for the task.
 * 9) adds the new task object in the task set.
 * 10) calls the machine-dependent code.
 * 11) create the default message types for the task.
 */

t_error			task_reserve(t_class			class,
				     t_behaviour		behav,
				     t_priority			prior,
				     i_task*			id)
{
  o_task		o;
  o_task*		parent;

  assert(id != NULL);

  /*
   * 1)
   */

  if ((class != TASK_CLASS_KERNEL) &&
      (class != TASK_CLASS_DRIVER) &&
      (class != TASK_CLASS_SERVICE) &&
      (class != TASK_CLASS_GUEST))
    CORE_ESCAPE("invalid class");

  if ((behav != TASK_BEHAVIOUR_KERNEL) &&
      (behav != TASK_BEHAVIOUR_REALTIME) &&
      (behav != TASK_BEHAVIOUR_INTERACTIVE) &&
      (behav != TASK_BEHAVIOUR_TIMESHARING) &&
      (behav != TASK_BEHAVIOUR_BACKGROUND))
    CORE_ESCAPE("invalid behaviour");

  switch (behav)
    {
      case TASK_BEHAVIOUR_KERNEL:
	{
	  if (prior < TASK_PRIORITY_KERNEL_LOW ||
	      prior > TASK_PRIORITY_KERNEL_HIGH)
	    CORE_ESCAPE("the given priority is inconsistent with the "
			"behaviour");

	  break;
	}
      case TASK_BEHAVIOUR_REALTIME:
	{
	  if (prior < TASK_PRIORITY_REALTIME_LOW ||
	      prior > TASK_PRIORITY_REALTIME_HIGH)
	    CORE_ESCAPE("the given priority is inconsistent with the "
			"behaviour");


	  break;
	}
      case TASK_BEHAVIOUR_INTERACTIVE:
	{
	  if (prior < TASK_PRIORITY_INTERACTIVE_LOW ||
	      prior > TASK_PRIORITY_INTERACTIVE_HIGH)
	    CORE_ESCAPE("the given priority is inconsistent with the "
			"behaviour");

	  break;
	}
      case TASK_BEHAVIOUR_TIMESHARING:
	{
	  if (prior < TASK_PRIORITY_TIMESHARING_LOW ||
	      prior > TASK_PRIORITY_TIMESHARING_HIGH)
	    CORE_ESCAPE("the given priority is inconsistent with the "
			"behaviour");

	  break;
	}
      case TASK_BEHAVIOUR_BACKGROUND:
	{
	  if (prior < TASK_PRIORITY_BACKGROUND_LOW ||
	      prior > TASK_PRIORITY_BACKGROUND_HIGH)
	    CORE_ESCAPE("the given priority is inconsistent with the "
			"behaviour");

	  break;
	}
      default:
	{
	  CORE_ESCAPE("unknown behaviour '%u'",
		      behav);
	}
    }

  /*
   * 2)
   */

  memset(&o, 0x0, sizeof(o_task));

  o.class = class;
  o.behaviour = behav;
  o.priority = prior;

  o.as = ID_UNUSED;
  o.threads = ID_UNUSED;
  o.waits = ID_UNUSED;

  if (_kernel->task != ID_UNUSED)
    {
      if (cpu_select(&o.cpu) != ERROR_OK)
	CORE_ESCAPE("unable to select a CPU on which to place the task");
    }
  else
    o.cpu = _init->bsp;

  o.state = TASK_STATE_STOP;

  /*
   * 3)
   */

  if (id_reserve(&_task->id, &o.id) != ERROR_OK)
    CORE_ESCAPE("unable to reserve an identifier for the task");

  /*
   * 4)
   */

  if (_kernel->task != ID_UNUSED)
    {
      if (task_current(&o.parent) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the currently scheduled task");

      if (task_get(o.parent, &parent) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the task object");

      if (set_add(parent->children, &o) != ERROR_OK)
	CORE_ESCAPE("unable to add the new task to the list of its parent's "
		    "children tasks");
    }
  else
    o.parent = ID_UNUSED;

  *id = o.id;

  /*
   * 5)
   */

  if (set_reserve(array,
		  SET_OPTION_SORT | SET_OPTION_ALLOC,
		  TASK_THREADS_INITSZ,
		  sizeof (i_thread),
		  &o.threads) != ERROR_OK)
    CORE_ESCAPE("unable to reserve a set of threads");

  /*
   * 6)
   */

  if (set_reserve(array,
		  SET_OPTION_ALLOC,
		  TASK_WAITS_INITSZ,
		  sizeof (i_task),
		  &o.waits) != ERROR_OK)
    CORE_ESCAPE("unable to reserve a set for the waiting tasks/threads");

  /*
   * 7)
   */

  if (set_reserve(ll,
		  SET_OPTION_SORT | SET_OPTION_ALLOC,
		  sizeof (i_task),
		  &o.children) != ERROR_OK)
    CORE_ESCAPE("unable to reserve a set for the children tasks");

  /*
   * 8)
   */

  if (set_reserve(bpt, SET_OPTION_SORT | SET_OPTION_ALLOC,
		  sizeof (o_message_type),
		  MESSAGE_BPT_NODESZ,
		  &o.messages) != ERROR_OK)
    CORE_ESCAPE("unable to reserve a set for the messages");

  /*
   * 9)
   */

  if (set_add(_task->tasks, &o) != ERROR_OK)
    CORE_ESCAPE("unable to add the object to the set of tasks");

  /*
   * 10)
   */

  if (machine_call(task, task_reserve, class, behav, prior, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 11)
   */

  if (o.id != _kernel->task)
    {
      if (message_register(o.id, MESSAGE_TYPE_INTERFACE,
			   sizeof (o_syscall)) != ERROR_OK)
	CORE_ESCAPE("unable to register the message channel for the "
		    "kernel interface");

      if (message_register(o.id, MESSAGE_TYPE_EVENT,
			   sizeof (o_event_message)) != ERROR_OK)
	CORE_ESCAPE("unable to register the message channel for notifying "
		    "events");

      if (message_register(o.id, MESSAGE_TYPE_TIMER,
			   sizeof (o_timer_message)) != ERROR_OK)
	CORE_ESCAPE("unable to register the message channel for triggering "
		    "timers");
    }

  CORE_LEAVE();
}

/*
 * this function releases a task.
 *
 * steps:
 *
 * 1) call the machine-dependent code.
 * 2) get the task object given its identifier.
 * 3) release the task's object identifier.
 * 4) release the task object's set of threads.
 * 5) release the task object's set of waits.
 * 6) release the task's list of messages.
 * 7) remove the task object from the task set.
 */

t_error			task_release(i_task			id)
{
  o_task*		o;

  /*
   * 1)
   */

  if (machine_call(task, task_release, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  if (task_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 3)
   */

  if (id_release(&_task->id, o->id) != ERROR_OK)
    CORE_ESCAPE("unable to release the task identifier");

  /*
   * 4)
   */

  if (o->as != ID_UNUSED && id != _kernel->task)
    {
      if (as_release(o->as) != ERROR_OK)
	CORE_ESCAPE("unable to release the task's address space");
    }

  /*
   * 5)
   */

  if (thread_flush(id) != ERROR_OK)
    CORE_ESCAPE("unable to flush the task's threads");

  if (set_release(o->threads) != ERROR_OK)
    CORE_ESCAPE("unable to release the set of threads");

  if (set_release(o->children) != ERROR_OK)
    CORE_ESCAPE("unable to release the set of children tasks");

  /*
   * 5)
   */

  if (set_release(o->waits) != ERROR_OK)
    CORE_ESCAPE("unable to release the set of waiting tasks/threads");

  /*
   * 6)
   */

  if (message_flush(id) != ERROR_OK)
    CORE_ESCAPE("unable to flush the messages");

  if (set_release(o->messages) != ERROR_OK)
    CORE_ESCAPE("unable to release the set of messages");

  /*
   * 7)
   */

  if (set_remove(_task->tasks, o->id) != ERROR_OK)
    CORE_ESCAPE("unable to remove the object from the set of tasks");

  CORE_LEAVE();
}

/*
 * this function updates the task's priority to "prior".
 *
 * steps:
 *
 * 1) get the task object.
 * 2) check the priority range using the behaviour.
 * 3) update the task priority.
 * 4) call the machine dependent code.
 * 5) update all the threads priorities in the scheduler.
 */

t_error			task_priority(i_task			id,
				      t_priority		prior)
{
  o_task*		o;
  t_iterator		i;
  t_state		state;

  /*
   * 1)
   */

  if (task_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  switch (o->behaviour)
    {
      case TASK_BEHAVIOUR_KERNEL:
	{
	  if (prior < TASK_PRIORITY_KERNEL_LOW ||
	      prior > TASK_PRIORITY_KERNEL_HIGH)
	    CORE_ESCAPE("the given priority is inconsistent with the "
			"behaviour");

	  break;
	}
      case TASK_BEHAVIOUR_REALTIME:
	{
	  if (prior < TASK_PRIORITY_REALTIME_LOW ||
	      prior > TASK_PRIORITY_REALTIME_HIGH)
	    CORE_ESCAPE("the given priority is inconsistent with the "
			"behaviour");

	  break;
	}
      case TASK_BEHAVIOUR_INTERACTIVE:
	{
	  if (prior < TASK_PRIORITY_INTERACTIVE_LOW ||
	      prior > TASK_PRIORITY_INTERACTIVE_HIGH)
	    CORE_ESCAPE("the given priority is inconsistent with the "
			"behaviour");

	  break;
	}
      case TASK_BEHAVIOUR_TIMESHARING:
	{
	  if (prior < TASK_PRIORITY_TIMESHARING_LOW ||
	      prior > TASK_PRIORITY_TIMESHARING_HIGH)
	    CORE_ESCAPE("the given priority is inconsistent with the "
			"behaviour");

	  break;
	}
      case TASK_BEHAVIOUR_BACKGROUND:
	{
	  if (prior < TASK_PRIORITY_BACKGROUND_LOW ||
	      prior > TASK_PRIORITY_BACKGROUND_HIGH)
	    CORE_ESCAPE("the given priority is inconsistent with the "
			"behaviour");

	  break;
	}
      default:
	{
	  CORE_ESCAPE("unknown behaviour '%u'",
		      o->behaviour);
	}
    }

  /*
   * 3)
   */

  o->priority = prior;

  /*
   * 4)
   */

  if (machine_call(task, task_priority, id, prior) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 5)
   */

  set_foreach(SET_OPTION_FORWARD, o->threads, &i, state)
    {
      i_thread*		th;
      o_thread*		oth;

      if (set_object(o->threads, i, (void**)&th) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the thread identifier");

      if (thread_get(*th, &oth) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the thread object");

      if ((o->state == TASK_STATE_START) &&
	  (oth->state == THREAD_STATE_START))
	{
	  if (scheduler_update(*th) != ERROR_OK)
	    CORE_ESCAPE("unable to update the thread scheduling state");
	}
    }

  CORE_LEAVE();
}

/*
 * this function marks the task as runnable.
 *
 * steps:
 *
 * 1) get the task object.
 * 2) set the new state.
 * 3) wakeup the waiting tasks.
 * 4) call the machine dependent code.
 * 5) start the threads.
 */

t_error			task_start(i_task			id)
{
  o_task*		o;
  t_iterator		i;
  t_state		st;

  /*
   * 1)
   */

  if (task_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  if (o->state == TASK_STATE_START)
    CORE_ESCAPE("a task cannot be started twice"); // XXX appliquer aux autres operations

  o->state = TASK_STATE_START;

  /*
   * 3)
   */
  /*
  set_foreach(SET_OPTION_FORWARD, o->waits, &i, st)
    {
      o_wait*		w;

      if (set_object(o->waits, i, (void**)&w) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the waiting entity");

      if (w->options & WAIT_START)
	{
	  if (task_run(w->u.task) != ERROR_OK)
	    CORE_ESCAPE("unable to run the waiting task");
	}

	// XXX remove from wait list
    }
  */
  /*
   * 4)
   */

  if (machine_call(task, task_start, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 5)
   */
  set_foreach(SET_OPTION_FORWARD, o->threads, &i, st)
    {
      i_thread*		th;
      o_thread*		oth;

      if (set_object(o->threads, i, (void**)&th) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the thread identifier");

      if (thread_get(*th, &oth) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the thread object");

      if (oth->state == THREAD_STATE_START)
	{
	  if (scheduler_add(oth->id) != ERROR_OK)
	    CORE_ESCAPE("unable to add the thread to the scheduler");
	}
    }

  CORE_LEAVE();
}

/*
 * this function marks the task as stopped.
 *
 * steps:
 *
 * 1) get the task object.
 * 2) set the new state.
 * 3) wakeup the waiting tasks.
 * 4) call the machine dependent code.
 * 5) start the threads.
 */

t_error			task_stop(i_task			id)
{
  t_state		state;
  o_task*		o;
  t_iterator		i;
  t_state		st;

  /*
   * 1)
   */

  if (task_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  state = o->state;

  /*
   * XXX
   */

  if (o->state == TASK_STATE_STOP)
    CORE_LEAVE();

  /*
   *
   */

  o->state = TASK_STATE_STOP;

  /*
   * 3)
   */
  /*
  set_foreach(SET_OPTION_FORWARD, o->waits, &i, st)
    {
      o_wait*		w;

      if (set_object(o->waits, i, (void**)&w) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the waiting entity");

      if (w->options & WAIT_STOP)
	{
	  if (task_run(w->u.task) != ERROR_OK)
	    CORE_ESCAPE("unable to run the task");
	}

	// XXX remove from wait list
    }
  */

  /*
   * 4)
   */

  if (machine_call(task, task_stop, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 5)
   */

  set_foreach(SET_OPTION_FORWARD, o->threads, &i, st)
    {
      i_thread*		th;
      o_thread*		oth;

      if (set_object(o->threads, i, (void**)&th) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the thread identifier");

      if (thread_get(*th, &oth) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the thread object");

      if ((state == TASK_STATE_START) &&
	  (oth->state == THREAD_STATE_START))
	{
	  if (scheduler_remove(oth->id) != ERROR_OK)
	    CORE_ESCAPE("unable to remove the thread from the scheduler");
	}
    }

  CORE_LEAVE();
}

/*
 * this function marks the task as blocked.
 *
 * steps:
 *
 * 1) get the task object.
 * 2) set the new state.
 * 3) wakeup the waiting tasks.
 * 4) call the machine dependent code.
 * 5) start the threads.
 */

t_error			task_block(i_task			id)
{
  t_state		state;
  o_task*		o;
  t_iterator		i;
  t_state		st;

  /*
   * 1)
   */

  if (task_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   *
   */

  state = o->state;

  /*
   * 2)
   */

  if (o->state == TASK_STATE_BLOCK)
    CORE_LEAVE();

  /*
   *
   */

  o->state = TASK_STATE_BLOCK;

  /*
   * 4)
   */

  if (machine_call(task, task_block, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 5)
   */

  set_foreach(SET_OPTION_FORWARD, o->threads, &i, st)
    {
      i_thread*		th;
      o_thread*		oth;

      if (set_object(o->threads, i, (void**)&th) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the thread identifier");

      if (thread_get(*th, &oth) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the thread object");

      if ((state == TASK_STATE_START) &&
	  (oth->state == THREAD_STATE_START))
	{
	  if (scheduler_remove(oth->id) != ERROR_OK)
	    CORE_ESCAPE("unable to remove the thread from the scheduler");
	}
    }

  CORE_LEAVE();
}

/*
 * this function marks the task as dead.
 *
 * steps:
 *
 * 1) get the task object.
 * 2) set the new state.
 * 3) wakeup the waiting tasks.
 * 4) call the machine dependent code.
 * 5) start the threads.
 */

t_error			task_exit(i_task			id,
				  t_value			value)
{
  o_task*		o;
  t_iterator		i;
  t_state		st;

  /*
   * 1)
   */

  if (task_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  if (o->state == TASK_STATE_ZOMBIE)
    CORE_LEAVE();

  o->state = TASK_STATE_ZOMBIE;

  /*
   * 3)
   */
  /*
  set_foreach(SET_OPTION_FORWARD, o->waits, &i, st)
    {
      o_wait*		w;

      if (set_object(o->waits, i, (void**)&w) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the waiting entity");

      if (w->options & WAIT_DEATH)
	{
	  if (task_run(w->u.task) != ERROR_OK)
	    CORE_ESCAPE("unable to run the task");
	}

      // XXX remove from wait list
    }
  */
  // XXX if list of waits empty -> task_release

  /*
   * 4)
   */

  if (machine_call(task, task_exit, id, value) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 5)
   */
  /*
  set_foreach(SET_OPTION_FORWARD, o->threads, &i, st)
    {
      i_thread*		th;

      if (set_object(o->threads, i, (void**)&th) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the thread");

      if (thread_stop(*th) != ERROR_OK)
	CORE_ESCAPE("unable to stop the thread");

	// exit every thread
    }
  */

  CORE_LEAVE();
}

/*
 * this function waits for state change.
 */

t_error			task_wait(i_task			id,
				  t_options			opts,
				  t_wait*			wait)
{
  /*
  o_task*		o;
  o_wait		w;

  assert(wait != NULL);

  if (opts & WAIT_ID)
    {
      if (task_get(id, &o) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the task object");

      if ((opts & WAIT_START) && o->state == TASK_STATE_RUN)
	CORE_LEAVE();

      if ((opts & WAIT_STOP) && o->state == TASK_STATE_STOP)
	CORE_LEAVE();

      if ((opts & WAIT_DEATH) && o->state == TASK_STATE_ZOMBIE)
	CORE_LEAVE();

      if (!(opts & WAIT_NOHANG))
	{
	  if (task_current(&w.u.task) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve the currently scheduled task");

	  w.options = opts;

	  if (set_add(o->waits, &w) != ERROR_OK)
	    CORE_ESCAPE("unable to add the task to the waiting list");

	  if (task_stop(w.u.task) != ERROR_OK)
	    CORE_ESCAPE("unable to stop the task");
	}
    }
  else
    {
    }

  if (wait != NULL)
    {
      // XXX
    }

  if (machine_call(task, task_wait, id, opts, wait) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_ESCAPE("unable to wait for the entity");
  */
}

/*
 * this function returns true if the task object exists.
 */

t_error			task_exist(i_task			id)
{
  if (set_exist(_task->tasks, id) != ERROR_TRUE)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function gets a task object from the task set.
 */

t_error			task_get(i_task				id,
				 o_task**			o)
{
  assert(o != NULL);

  if (set_get(_task->tasks, id, (void**)o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the object from the set of tasks");

  CORE_LEAVE();
}

/*
 * this functions initializes the task manager.
 *
 * this function takes care of initializing and builing the kernel task.
 *
 * steps:
 *
 * 1) allocate and initializes the task manager structure.
 * 2) initialize the identifier object to be able to generate
 *    the task identifiers.
 * 3) reserve the task set which will contain the tasks built later.
 * 4) reserve the kernel task and its address space.
 * 5) add the segments to the kernel address space.
 *    note that for the segment corresponding to the system service code, the
 *    segment identifier is saved in the system global variable so that the
 *    core can retrieve it, build a task, map the segment, and launch it.
 * 6) add the regions to the kernel address space.
 * 7) call the machine-dependent code.
 */

t_error			task_initialize(void)
{
  i_segment		segments[GLUE_INIT_SEGMENTS];
  i_region		useless;
  o_segment*		segment;
  o_region*		region;
  t_uint32		i;

  /*
   * 1)
   */

  if ((_task = malloc(sizeof(m_task))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the task manager's structure");

  memset(_task, 0x0, sizeof(m_task));

  /*
   * 2)
   */

  if (id_build(&_task->id) != ERROR_OK)
    CORE_ESCAPE("unable to build the identifier object");

  /*
   * 3)
   */

  if (set_reserve(ll, SET_OPTION_ALLOC | SET_OPTION_SORT,
		  sizeof(o_task), &_task->tasks) != ERROR_OK)
    CORE_ESCAPE("unable to reserve the set of tasks");

  /*
   * 4)
   */

  if (task_reserve(TASK_CLASS_KERNEL, TASK_BEHAVIOUR_KERNEL,
		   TASK_PRIORITY_KERNEL, &_kernel->task) != ERROR_OK)
    CORE_ESCAPE("unable to reserve the kernel task");

  if (as_reserve(_kernel->task, &_kernel->as) != ERROR_OK)
    CORE_ESCAPE("unable to reserve the kernel task's address space");

  if (task_start(_kernel->task) != ERROR_OK)
    CORE_ESCAPE("unable to start the kernel task");

  /*
   * 5)
   */

  for (i = 0; i < _init->nsegments; i++)
    {
      if ((segment = malloc(sizeof(o_segment))) == NULL)
	CORE_ESCAPE("unable to allocate memory for the segment object");

      segment->type = SEGMENT_TYPE_MEMORY;
      segment->address = _init->segments[i].address;
      segment->size = _init->segments[i].size;
      segment->permissions = _init->segments[i].permissions;

      assert(segment->size != 0);

      if (segment_inject(_kernel->as, segment, &segments[i]) != ERROR_OK)
	CORE_ESCAPE("unable to inject the segment object pre-reserved "
		    "by the boot loader");

      if (_init->scode == _init->segments[i].address)
	_system = segments[i];
    }

  /*
   * 6)
   */

  for (i = 0; i < _init->nregions; i++)
    {
      if ((region = malloc(sizeof(o_region))) == NULL)
	CORE_ESCAPE("unable to allocate memory for the region object");

      region->segment = segments[_init->regions[i].segment];
      region->address = _init->regions[i].address;
      region->offset = _init->regions[i].offset;
      region->size = _init->regions[i].size;
      region->options = _init->regions[i].options;

      assert(region->size != 0);

      if (region_inject(_kernel->as, region, &useless) != ERROR_OK)
	CORE_ESCAPE("unable to inject the region object pre-reserved "
		    "by the boot loader");
    }

  /*
   * 7)
   */

  if (machine_call(task, task_initialize) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function just reinitializes the task manager.
 *
 * steps:
 *
 * 1) call the machine-dependent code.
 * 2) release the task's set.
 * 3) destroy the id object.
 * 4) free the task manager structure's memory.
 */

t_error			task_clean(void)
{
  t_iterator		i;
  i_task		*data;

  /*
   * 1)
   */

  if (machine_call(task, task_clean) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  while (set_head(_task->tasks, &i) == ERROR_TRUE)
    {
      if (set_object(_task->tasks, i, (void**)&data) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the task identifier");

      if (task_release(*data) != ERROR_OK)
	CORE_ESCAPE("unable to release the task");
    }

  if (set_release(_task->tasks) != ERROR_OK)
    CORE_ESCAPE("unable to release the set of tasks");

  /*
   * 3)
   */

  if (id_destroy(&_task->id) != ERROR_OK)
    CORE_ESCAPE("unable to destroy the identifier object");

  /*
   * 4)
   */

  free(_task);

  CORE_LEAVE();
}
