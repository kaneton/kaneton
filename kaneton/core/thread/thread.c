/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/thread/thread.c
 *
 * created       renaud voltz   [tue apr  4 03:02:57 2006]
 * updated       julian pidancet   [fri apr 13 03:06:06 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the thread manager provides functionalities for creating, destorying
 * and controlling the active scheduling entites known as _thread_s.
 *
 * a thread contains an execution context composed of several machine-dependent
 * registers which capture the state of a state so that the thread's
 * execution can be resumed later.
 *
 * note that every task possesses its own stack i.e a personal memory area
 * which is used to store local information such as arguments and local
 * variables.
 *
 * the thread manager, in addition to creating and destroying threads,
 * provides functions for controlling the state of these objects. thus,
 * one may want to start a thread so that its code start executing,
 * block a thread until some event wakes it up or even exit a thread,
 * hence actually destroying the object and return a value to other threads
 * which may have been waiting for the thread to do so.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * include the machine-specific definitions required by the core.
 */

machine_include(thread);

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the kernel manager.
 */

extern m_kernel*	_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the thread manager.
 */

m_thread*		_thread = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function shows a given thread's attributes.
 *
 * steps:
 *
 * 1) retrieve the thread object.
 * 2) build the state string.
 * 3) display the object's attributes.
 * 4) if the thread is currently waiting, display its waiting state.
 *   a) build the state string.
 *   b) display the waiting state.
 * 5) call the machine.
 */

t_error			thread_show(i_thread			threadid,
				    mt_margin			margin)
{
  char*			state;
  o_thread*		o;

  /*
   * 1)
   */

  if (thread_get(threadid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  /*
   * 2)
   */

  switch (o->state)
    {
    case THREAD_STATE_START:
      {
	state = "start";

	break;
      }
    case THREAD_STATE_STOP:
      {
	state = "stop";

	break;
      }
    case THREAD_STATE_BLOCK:
      {
	state = "block";

	break;
      }
    case THREAD_STATE_ZOMBIE:
      {
	state = "zombie";

	break;
      }
    case THREAD_STATE_DEAD:
      {
	state = "dead";

	break;
      }
    default:
      CORE_ESCAPE("unknown thread state '%u'",
		  o->state);
    }

  /*
   * 3)
   */

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "thread: id(%qd) priority(%u) state(%s) waits(%qd) "
	      "value(%d) stack(0x%x) stacksz(0x%x) task(%qd)",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      o->id,
	      o->priority,
	      state,
	      o->waits,
	      o->value,
	      o->stack,
	      o->stacksz,
	      o->task);

  /*
   * 4)
   */

  if (o->wait.state != WAIT_STATE_NONE)
    {
      /*
       * a)
       */

      switch (o->wait.state)
	{
	case THREAD_STATE_START:
	  {
	    state = "start";

	    break;
	  }
	case THREAD_STATE_STOP:
	  {
	    state = "stop";

	    break;
	  }
	case THREAD_STATE_BLOCK:
	  {
	    state = "block";

	    break;
	  }
	case THREAD_STATE_ZOMBIE:
	  {
	    state = "zombie";

	    break;
	  }
	case THREAD_STATE_DEAD:
	  {
	    state = "dead";

	    break;
	  }
	default:
	  CORE_ESCAPE("unknown thread state '%u'",
		      o->state);
	}

      /*
       * b)
       */

      module_call(console, print,
		  " wait(%s)\n",
		  o->wait.state);
    }
  else
    module_call(console, print, "\n");

  /*
   * 5)
   */

  if (machine_call(thread, show, threadid, margin) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function dumps the thread manager.
 *
 * steps:
 *
 * 1) display general information on the manager.
 * 2) show the identifier object.
 * 3) retrieve the size of the set of threads.
 * 4) display information on the set and show every thread.
 * 5) retrieve the size of the set of dead threads.
 * 6) display information and show every dead thread identifier.
 * 7) call the machine.
 */

t_error			thread_dump(void)
{
  t_setsz		size;
  s_iterator		i;
  t_state		s;

  /*
   * 1)
   */

  module_call(console, message,
	      '#', "thread manager: threads(%qd)\n",
	      _thread->threads);

  /*
   * 2)
   */

  if (id_show(&_thread->id, MODULE_CONSOLE_MARGIN_SHIFT) != ERROR_OK)
    CORE_ESCAPE("unable to show the identifier object");

  /*
   * 3)
   */

  if (set_size(_thread->threads, &size) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the size of the set of threads");

  /*
   * 4)
   */

  module_call(console, message,
	      '#', "  threads: id(%qd) size(%qd)\n",
	      _thread->threads,
	      size);

  set_foreach(SET_OPTION_FORWARD, _thread->threads, &i, s)
    {
      o_thread*		o;

      if (set_object(_thread->threads, i, (void**)&o) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the thread identifier");

      if (thread_show(o->id,
		      2 * MODULE_CONSOLE_MARGIN_SHIFT) != ERROR_OK)
	CORE_ESCAPE("unable to show the thread");
    }

  /*
   * 5)
   */

  if (set_size(_thread->morgue.field, &size) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the size of the set of dead threads");

  /*
   * 6)
   */

  module_call(console, message,
	      '#', "  morgue: field(%qd) timer(%qd)\n",
	      _thread->morgue.field,
	      _thread->morgue.timer);

  module_call(console, message,
	      '#', "    field: id(%qd) size(%qd)\n",
	      _thread->morgue.field,
	      size);

  set_foreach(SET_OPTION_FORWARD, _thread->morgue.field, &i, s)
    {
      i_thread*		id;

      if (set_object(_thread->morgue.field, i, (void**)&id) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the thread identifier");

      module_call(console, message,
		  '#', "      thread: id(%qd)\n",
		  *id);
    }

  /*
   * 7)
   */

  if (machine_call(thread, dump) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function reserves a thread with the given priority.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the task object.
 * 2) reserve a thread identifier.
 * 3) initialize and fill the object.
 * 4) reserve a set for the waiting threads.
 * 5) add the object to the set of threads.
 * 6) add the thread identifier to the task's set of threads.
 * 7) call the machine.
 */

t_error			thread_reserve(i_task			taskid,
				       t_priority		prior,
				       i_thread*		id)
{
  o_task*		task;
  o_thread		o;

  /*
   * 0)
   */

  if (id == NULL)
    CORE_ESCAPE("the 'id' argument is null");

  if ((prior < THREAD_PRIORITY_LOW) ||
      (prior > THREAD_PRIORITY_HIGH))
    CORE_ESCAPE("invalid priority");

  /*
   * 1)
   */

  if (task_get(taskid, &task) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  if (id_reserve(&_thread->id, id) != ERROR_OK)
    CORE_ESCAPE("unable to reserve a thread identifier");

  /*
   * 3)
   */

  memset(&o, 0x0, sizeof(o_thread));

  o.id = *id;
  o.task = taskid;
  o.priority = prior;
  o.state = THREAD_STATE_STOP;
  o.waits = ID_UNUSED;
  o.value = WAIT_VALUE_UNKNOWN;
  o.timer = ID_UNUSED;

  o.wait.state = WAIT_STATE_NONE;

  /*
   * 4)
   */

  if (set_reserve(array,
		  SET_OPTION_ALLOCATE,
		  THREAD_WAITS_INITSZ,
		  sizeof (i_thread),
		  &o.waits) != ERROR_OK)
    CORE_ESCAPE("unable to reserve a set for the waiting tasks/threads");

  /*
   * 5)
   */

  if (set_add(_thread->threads, &o) != ERROR_OK)
    CORE_ESCAPE("unable to add the object to the set of threads");

  /*
   * 6)
   */

  if (set_add(task->threads, &o.id) != ERROR_OK)
    CORE_ESCAPE("unable to add the thread to the task's set of threads");

  /*
   * 7)
   */

  if (machine_call(thread, reserve, taskid, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function releases a thread.
 *
 * steps:
 *
 * 1) call the machine.
 * 2) retrieve the thread object.
 * 3) retrieve the task object.
 * 4) if the thread possesses a stack, release it.
 * 5) if necessary, release the thread's timer.
 * 6) release the thread identifier.
 * 7) remove the thread identifier from the task's set of threads.
 * 8) remove the thread object from the set of threads.
 */

t_error			thread_release(i_thread			threadid)
{
  o_task*		task;
  o_thread*		o;

  /*
   * 1)
   */

  if (machine_call(thread, release, threadid) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  if (thread_get(threadid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  /*
   * 3)
   */

  if (task_get(o->task, &task) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 4)
   */

  if (o->stack != (t_vaddr)NULL)
    {
      if (map_release(task->as, o->stack) != ERROR_OK)
	CORE_ESCAPE("unable to release the map for the thread's stack");
    }

  /*
   * 5)
   */

  if (o->timer != ID_UNUSED)
    {
      if (timer_release(o->timer) != ERROR_OK)
	CORE_ESCAPE("unable to release the timer");
    }

  /*
   * 6)
   */

  if (id_release(&_thread->id, o->id) != ERROR_OK)
    CORE_ESCAPE("unable to release the thread identifier");

  /*
   * 7)
   */

  if (set_remove(task->threads, threadid) != ERROR_OK)
    CORE_ESCAPE("unable to remove the thread from the task's set of threads");

  /*
   * 8)
   */

  if (set_remove(_thread->threads, threadid) != ERROR_OK)
    CORE_ESCAPE("unable to remove the thread identifier from the set of "
		"threads");

  CORE_LEAVE();
}

/*
 * this function updates the thread's priority.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the thread and task objects.
 * 2) if the priority is identical, exit.
 * 3) update the thread's priority.
 * 4) call the machine.
 * 5) finally, if both the task and thread are running, request the
 *    scheduler to re-compute the thred's scheduling priority and, if
 *    necessary, to move the thread to another queue etc. this operation
 *    is performed after calling the machine because the execution may
 *    be yielded.
 */

t_error			thread_priority(i_thread		threadid,
					t_priority		prior)
{
  o_task*		task;
  o_thread*		o;

  /*
   * 0)
   */

  if ((prior < THREAD_PRIORITY_LOW) ||
      (prior > THREAD_PRIORITY_HIGH))
    CORE_ESCAPE("invalid priority");

  /*
   * 1)
   */

  if (thread_get(threadid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  if (task_get(o->task, &task) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  if (o->priority == prior)
    CORE_LEAVE();

  /*
   * 3)
   */

  o->priority = prior;

  /*
   * 4)
   */

  if (machine_call(thread, priority, threadid, prior) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 5)
   */

  if ((task->state == TASK_STATE_START) &&
      (o->state == THREAD_STATE_START))
    {
      if (scheduler_update(threadid) != ERROR_OK)
	CORE_ESCAPE("unable to update the scheduled thread");
    }

  CORE_LEAVE();
}

/*
 * this function starts a thread.
 *
 * steps:
 *
 * 1) retrieve the thread and task objects.
 * 2) if the thread is already running, return an error.
 * 3) modify the thread's state.
 * 4) go through the waiting thread in order to wake up the ones waiting
 *    for this change.
 *   a) retrieve the thread identifier and object.
 *   b) if the thread is waiting for the thread to start.
 *     i) fill the waiting thread's wait structure with the cause: start.
 *     ii) wake up the waiting thread.
 *     iii) delete the waiting thread from the list.
 *     iv) restart the process since the set has been modified i.e the
 *         iterators may be inconsistent.
 * 5) call the machine.
 * 6) if the task is running, add the now running thread to the scheduler.
 */

t_error			thread_start(i_thread			id)
{
  o_thread*		thread;
  o_task*		task;
  t_state		s;
  s_iterator		i;

  /*
   * 1)
   */

  if (thread_get(id, &thread) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  if (task_get(thread->task, &task) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  if (thread->state == THREAD_STATE_START)
    CORE_ESCAPE("a thread cannot be started twice");

  /*
   * 3)
   */

  thread->state = THREAD_STATE_START;

  /*
   * 4)
   */

 try:
  set_foreach(SET_OPTION_FORWARD, thread->waits, &i, s)
    {
      i_thread*		id;
      o_thread*		o;

      /*
       * a)
       */

      if (set_object(thread->waits, i, (void**)&id) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the waiting thread's identifier");

      if (thread_get(*id, &o) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the waiting thread");

      /*
       * b)
       */

      if (o->wait.state & WAIT_STATE_START)
	{
	  /*
	   * i)
	   */

	  o->wait.cause = WAIT_STATE_START;
	  o->wait.value = WAIT_VALUE_UNKNOWN;

	  /*
	   * ii)
	   */

	  if (thread_start(o->id) != ERROR_OK)
	    CORE_ESCAPE("unable to start the waiting thread");

	  /*
	   * iii)
	   */

	  if (set_delete(thread->waits, i) != ERROR_OK)
	    CORE_ESCAPE("unable to delete the thread from the waiting list");

	  /*
	   * iv)
	   */

	  goto try;
	}
    }

  /*
   * 5)
   */

  if (machine_call(thread, start, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 6)
   */

  if (task->state == TASK_STATE_START)
    {
      if (scheduler_add(id) != ERROR_OK)
	CORE_ESCAPE("unable to add the thread to the scheduler");
    }

  CORE_LEAVE();
}

/*
 * this function stops a thread.
 *
 * steps:
 *
 * 1) retrieve the thread and task objects.
 * 2) save the current thread's state.
 * 3) if the thread is already stop, return an error.
 * 4) modify the thread's state.
 * 5) go through the set of waiting threads.
 *   a) retrieve the thread identifier and object.
 *   b) if the thread is waiting for the thread to stop.
 *     i) fill the waiting thread's wait structure with the cause: stop.
 *     ii) wake up the waiting thread.
 *     iii) delete the waiting thread from the list.
 *     iv) restart the process since the set has been modified i.e the
 *         iterators may be inconsistent.
 * 5) call the machine.
 * 6) if the task is running and the thread was previously running,
 *    remove the thread from the scheduler
 */

t_error			thread_stop(i_thread			id)
{
  o_thread*		thread;
  o_task*		task;
  t_state		state;
  t_state		s;
  s_iterator		i;

  /*
   * 1)
   */

  if (thread_get(id, &thread) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  if (task_get(thread->task, &task) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  state = thread->state;

  /*
   * 3)
   */

  if (thread->state == THREAD_STATE_STOP)
    CORE_ESCAPE("a thread cannot be stopped twice");

  /*
   * 4)
   */

  thread->state = THREAD_STATE_STOP;

  /*
   * 5)
   */

 try:
  set_foreach(SET_OPTION_FORWARD, thread->waits, &i, s)
    {
      i_thread*		ith;
      o_thread*		o;

      /*
       * a)
       */

      if (set_object(thread->waits, i, (void**)&ith) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the waiting thread's identifier");

      if (thread_get(*ith, &o) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the waiting thread");

      /*
       * b)
       */

      if (o->wait.state & WAIT_STATE_STOP)
	{
	  /*
	   * i)
	   */

	  o->wait.cause = WAIT_STATE_STOP;
	  o->wait.value = WAIT_VALUE_UNKNOWN;

	  /*
	   * ii)
	   */

	  if (thread_start(o->id) != ERROR_OK)
	    CORE_ESCAPE("unable to start the waiting thread");

	  /*
	   * iii)
	   */

	  if (set_delete(thread->waits, i) != ERROR_OK)
	    CORE_ESCAPE("unable to delete the thread from the waiting list");

	  /*
	   * iv)
	   */

	  goto try;
	}
    }

  /*
   * 6)
   */

  if (machine_call(thread, stop, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 7)
   */

  if ((task->state == TASK_STATE_START) &&
      (state == THREAD_STATE_START))
    {
      if (scheduler_remove(id) != ERROR_OK)
	CORE_ESCAPE("unable to remove the thread from the scheduler");
    }

  CORE_LEAVE();
}

/*
 * this function blocks a thread.
 *
 * steps:
 *
 * 1) retrieve the thread and task objects.
 * 2) save the thread's current state.
 * 3) if the thread is already blocked, return an error.
 * 4) modify the thread's state.
 * 5) call the machine.
 * 6) if the task is running and the thread was previously running,
 *    remove the thread from the scheduler.
 */

t_error			thread_block(i_thread			id)
{
  o_task*		task;
  o_thread*		thread;
  t_state		state;

  /*
   * 1)
   */

  if (thread_get(id, &thread) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  if (task_get(thread->task, &task) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  state = thread->state;

  /*
   * 3)
   */

  if (thread->state == THREAD_STATE_BLOCK)
    CORE_ESCAPE("a thread cannot be blocked twice");

  /*
   * 4)
   */

  thread->state = THREAD_STATE_BLOCK;

  /*
   * 5)
   */

  if (machine_call(thread, block, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 6)
   */

  if ((task->state == TASK_STATE_START) &&
      (state == THREAD_STATE_START))
    {
      if (scheduler_remove(id) != ERROR_OK)
	CORE_ESCAPE("unable to remove the thread from the scheduler");
    }

  CORE_LEAVE();
}

/*
 * this function exits a thread, passing the exit value to the threads
 * waiting for this event.
 *
 * note that this function performs a quite unusual operation. the morgue,
 * in which the dead thread are stored, relies on a timer. unfortunately,
 * the timer manager must be initialized after the thread manager. therefore
 * the thread manager could not reserve a timer in its initialization phase.
 * the timer reservation has thus been put in this function such that, if
 * no task dies, the timer is never reserved and the morgue never run.
 *
 * steps:
 *
 * ~) if required, reserve the timer associated with the morgue.
 * 1) retrieve the thread and task objects.
 * 2) save the thread's current state.
 * 3) if the thread is already dying or dead, return an error.
 * 4) store the exit value in the thread object.
 * 5) change the thread's state.
 * 6) go through the waiting threads.
 *   a) retrieve the thread identifier.
 *   b) retrieve the thread object.
 *   c) if the thread is waiting for the exiting thread to exit.
 *     i) set the waiting state by specific both the cause, i.e death, and
 *        the exit value.
 *     ii) wake up the thread.
 *     iii) delete the thread from the waiting list.
 *     iv) mark the fact that someone took notice of the thread's death.
 *     v) restart the whole process since the set has been modified, the
 *        iterator may be inconsistent.
 * 7) if someone took notice of the thread's death, the thread can be buried.
 *   a) change the thread's state to dead.
 *   b) add the thread identifer to the morgue.
 * 8) call the machine.
 * 9) if the task is running and the thread was running, remove the thread
 *    from the scheduler.
 */

t_error			thread_exit(i_thread			id,
				    t_value			value)
{
  o_task*		task;
  o_thread*		object;
  t_state		state;
  s_iterator		i;
  t_state		s;
  t_boolean		w;

  /*
   * ~)
   */

  if (_thread->morgue.timer == ID_UNUSED)
    {
      if (timer_reserve(TIMER_TYPE_FUNCTION,
			TIMER_ROUTINE(thread_bury),
			TIMER_DATA(NULL),
			THREAD_MORGUE_DELAY,
			TIMER_OPTION_REPEAT,
			&_thread->morgue.timer) != ERROR_OK)
	MACHINE_ESCAPE("unable to reserve the timer");
    }

  /*
   * 1)
   */

  if (thread_get(id, &object) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  if (task_get(object->task, &task) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  state = object->state;

  /*
   * 3)
   */

  if ((object->state == THREAD_STATE_ZOMBIE) ||
      (object->state == THREAD_STATE_DEAD))
    CORE_ESCAPE("a thread cannot be exited twice");

  /*
   * 4)
   */

  object->value = value;

  /*
   * 5)
   */

  object->state = THREAD_STATE_ZOMBIE;

  /*
   * 6)
   */

  w = BOOLEAN_FALSE;

 try:
  set_foreach(SET_OPTION_FORWARD, object->waits, &i, s)
    {
      i_thread*		id;
      o_thread*		o;

      /*
       * a)
       */

      if (set_object(object->waits, i, (void**)&id) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the waiting thread's identifier");

      /*
       * b)
       */

      if (thread_get(*id, &o) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the waiting thread");

      /*
       * c)
       */

      if (o->wait.state & WAIT_STATE_DEATH)
	{
	  /*
	   * i)
	   */

	  o->wait.cause = WAIT_STATE_DEATH;
	  o->wait.value = object->value;

	  /*
	   * ii)
	   */

	  if (thread_start(o->id) != ERROR_OK)
	    CORE_ESCAPE("unable to start the waiting thread");

	  /*
	   * iii)
	   */

	  if (set_delete(object->waits, i) != ERROR_OK)
	    CORE_ESCAPE("unable to delete the thread from the waiting list");

	  /*
	   * iv)
	   */

	  w = BOOLEAN_TRUE;

	  /*
	   * v)
	   */

	  goto try;
	}
    }

  /*
   * 7)
   */

  if (w == BOOLEAN_TRUE)
    {
      /*
       * a)
       */

      object->state = THREAD_STATE_DEAD;

      /*
       * b)
       */

      if (set_add(_thread->morgue.field, &object->id) != ERROR_OK)
	CORE_ESCAPE("unable to add the thread to the morgue");
    }

  /*
   * 8)
   */

  if (machine_call(thread, exit, id, value) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 9)
   */

  if ((task->state == TASK_STATE_START) &&
      (state == THREAD_STATE_START))
    {
      if (scheduler_remove(id) != ERROR_OK)
	CORE_ESCAPE("unable to remove the thread from the scheduler");
    }

  CORE_LEAVE();
}

/*
 * this function is triggered on a regular basis in order to bury the dead
 * threads.
 *
 * steps:
 *
 * 1) go through the dead threads.
 *   a) retrieve the thread identifier and object along with the task
 *      object.
 *   b) if the thread is not dead, it should not be here. this is a fatal
 *      error.
 *   c) save the thread's exit value.
 *   d) release the thread.
 *   e) if there are no more threads in the task, exit the task with
 *      the exit value used for exiting the last thread.
 */

void			thread_bury(i_timer			timer,
				    t_vaddr			data)
{
  s_iterator		i;
  t_state		s;

  /*
   * 1)
   */

  set_foreach(SET_OPTION_FORWARD, _thread->morgue.field, &i, s)
    {
      i_thread*		id;
      o_thread*		thread;
      o_task*		task;
      t_value		value;

      /*
       * a)
       */

      assert(set_object(_thread->morgue.field, i, (void**)&id) == ERROR_OK);

      assert(thread_get(*id, &thread) == ERROR_OK);

      assert(task_get(thread->task, &task) == ERROR_OK);

      /*
       * b)
       */

      assert(thread->state == THREAD_STATE_DEAD);

      /*
       * c)
       */

      value = thread->value;

      /*
       * d)
       */

      assert(thread_release(thread->id) == ERROR_OK);

      /*
       * e)
       */

      if (set_empty(task->threads) == ERROR_TRUE)
	assert(task_exit(task->id, value) == ERROR_OK);
    }
}

/*
 * this function waits for a thread to change state.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the thread and target task objects.
 * 2) if the thread is waiting for the thread to start and the thread is
 *    already running, fill and return the information.
 * 3) if the thread is waiting for the thread to stop and the thread is
 *    already stopped, fill and return the information.
 * 4) if the thread is waiting for the thread to exit and the thread is
 *    already dying, fill and return the information.
 *   a) fill the waiting thread's wait structure.
 *   b) set the thread as being dead now that someone has taken notice
 *      of its death.
 *   c) add the thread to the morgue.
 * 5) if the thread is already dead, there is nothing to wait for, return an
 *    error as it is too late.
 * 6) set the thread's waiting state as the one provided. it is necessary
 *    to store such an information for later when the thread will finally
 *    change its state. at that moment, the task will have to know what
 *    the thread is waiting for to decide wheter to wake him up or not.
 * 7) add the thread to the thread's waiting list.
 * 8) stop the thread. the thread will be woken up by the thread once it will
 *    have changed to the given state. therefore the next steps are executed
 *    only after the thread has been woken up.
 * 9) retrieve the information passed by the thread on the event that led
 *    to the thread being woken up i.e the cause but also the value should
 *    the task have exited. note that these information are passed by the
 *    thread through the waiting thread's object 'wait' specific attribute.
 * 10) re-initialize the thread has being waiting for nothing.
 * 11) call the machine.
 */

t_error			thread_wait(i_thread			id,
				    i_thread			target,
				    t_state			state,
				    s_wait*			wait)
{
  o_thread*		object;
  o_thread*		o;

  /*
   * 0)
   */

  if (wait == NULL)
    CORE_ESCAPE("the 'wait' argument is null");

  /*
   * 1)
   */

  if (thread_get(id, &object) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  if (thread_get(target, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  /*
   * 2)
   */

  if ((state & WAIT_STATE_START) &&
      (o->state == THREAD_STATE_START))
    {
      wait->id.thread = target;
      wait->state = state;
      wait->cause = WAIT_STATE_START;
      wait->value = WAIT_VALUE_UNKNOWN;

      CORE_LEAVE();
    }

  /*
   * 3)
   */

  if ((state & WAIT_STATE_STOP) &&
      (o->state == THREAD_STATE_STOP))
    {
      wait->id.thread = target;
      wait->state = state;
      wait->cause = WAIT_STATE_STOP;
      wait->value = WAIT_VALUE_UNKNOWN;

      CORE_LEAVE();
    }

  /*
   * 4)
   */

  if ((state & WAIT_STATE_DEATH) &&
      (o->state == THREAD_STATE_ZOMBIE))
    {
      /*
       * a)
       */

      wait->id.thread = target;
      wait->state = state;
      wait->cause = WAIT_STATE_DEATH;
      wait->value = o->value;

      /*
       * b)
       */

      o->state = THREAD_STATE_DEAD;

      /*
       * c)
       */

      if (set_add(_thread->morgue.field, &o->id) != ERROR_OK)
	CORE_ESCAPE("unable to add the thread to the morgue");

      CORE_LEAVE();
    }

  /*
   * 5)
   */

  if (o->state == THREAD_STATE_DEAD)
    CORE_ESCAPE("unable to wait for a dead thread");

  /*
   * 6)
   */

  object->wait.state = state;

  /*
   * 7)
   */

  if (set_add(o->waits, &id) != ERROR_OK)
    CORE_ESCAPE("unable to add the thread identifier to the waiting list");

  /*
   * 8)
   */

  if (thread_stop(id) != ERROR_OK)
    CORE_ESCAPE("unable to stop the task");

  /*
   * 9)
   */

  wait->id.thread = target;
  wait->state = object->wait.state;
  wait->cause = object->wait.cause;
  wait->value = object->wait.value;

  /*
   * 10)
   */

  object->wait.state = WAIT_STATE_NONE;

  /*
   * 11)
   */

  if (machine_call(thread, wait, id, target, state, wait) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function allocates a stack for the given thread.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the thread object.
 * 2) retrieve the task object.
 * 3) set the task depending on the argument.
 *   A) if no base address was provided, allocate a stack.
 *   B) otherwise, use the address provided.
 * 4) set the stack size in the object.
 * 5) call the machine.
 */

t_error			thread_stack(i_thread			threadid,
				     s_stack			stack)
{
  o_thread*		o;
  o_task*		task;

  /*
   * 0)
   */

  if (stack.size < THREAD_STACKSZ_LOW)
    CORE_ESCAPE("the provided stack size is too low");

  /*
   * 1)
   */

  if (thread_get(threadid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  /*
   * 2)
   */

  if (task_get(o->task, &task) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 3)
   */

  if (stack.base == 0)
    {
      /*
       * A)
       */

      if (map_reserve(task->as,
		      MAP_OPTION_NONE |
		      (task->class == TASK_CLASS_KERNEL ?
		       MAP_OPTION_PRIVILEGED : MAP_OPTION_USER),
		      stack.size,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &(o->stack)) != ERROR_OK)
	CORE_ESCAPE("unable to reserve a map for the thread's stack");
    }
  else
    {
      /*
       * B)
       */

      o->stack = stack.base;
    }

  /*
   * 4)
   */

  o->stacksz = stack.size;

  /*
   * 5)
   */

  if (machine_call(thread, stack, threadid, stack) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function pushes arguments on the thread's stack so that, when
 * the thread starts, it can retrieve the arguments in its address space.
 *
 * note that this operation is left to the machine to implement since
 * highly machine-specific.
 */

t_error			thread_args(i_thread			threadid,
				    void*			args,
				    t_vsize			size)
{
  if (machine_call(thread, args, threadid, args, size) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function is called when a thread must be woken up.
 *
 * steps:
 *
 * 1) wake up the thread.
 */

void			thread_wakeup(i_timer			timer,
				      t_vaddr			data)
{
  i_thread*		id = (i_thread*)data;

  /*
   * 1)
   */

  assert(thread_start(*id) == ERROR_OK);
}

/*
 * this function puts the given thread to sleep.
 *
 * note that every thread object embeds an available timer identifier which,
 * in this case, is used to trigger the wakeup function.
 *
 * another particularity comes from the fact that the id of the thread to
 * wake up is passed as the data to the timer. note that since the object
 * is not supposed to be released before the timer is triggered, the
 * address of the identifier within the thread object is actually passed.
 *
 * steps:
 *
 * 1) retrieve the thread object.
 * 2) if the thread is not running, it cannot be put to sleep.
 * 3) reserve a timer.
 * 4) call the machine.
 * 5) finally, block the thread.
 */

t_error			thread_sleep(i_thread			id,
				     t_uint32			milliseconds)
{
  o_thread*		o;

  /*
   * 1)
   */

  if (thread_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  /*
   * 2)
   */

  if (o->state != THREAD_STATE_START)
    CORE_ESCAPE("unable to put to sleep a non-running thread");

  /*
   * 3)
   */

  if (timer_reserve(TIMER_TYPE_FUNCTION,
                    TIMER_ROUTINE(thread_wakeup),
                    TIMER_DATA(&o->id),
                    milliseconds,
                    TIMER_OPTION_NONE,
                    &o->timer) != ERROR_OK)
    CORE_ESCAPE("unable to reserve a timer");

  /*
   * 4)
   */

  if (machine_call(thread, sleep, id, milliseconds) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 5)
   */

  if (thread_block(o->id) != ERROR_OK)
    CORE_ESCAPE("unable to block the thread");

  CORE_LEAVE();
}

/*
 * this function removes every thread that belongs to the specified task.
 *
 * steps:
 *
 * 1) call the machine.
 * 2) retrieve the task object.
 * 3) go through the threads.
 *   a) retrieve the thread identifier.
 *   b) release the thread.
 */

t_error			thread_flush(i_task			taskid)
{
  o_task*		task;
  s_iterator		i;

  /*
   * 1)
   */

  if (machine_call(thread, flush, taskid) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  if (task_get(taskid, &task) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 3)
   */

  while (set_head(task->threads, &i) == ERROR_TRUE)
    {
      i_thread*		id;

      /*
       * a)
       */

      if (set_object(task->threads, i, (void**)&id) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the thread identifier");

      /*
       * b)
       */

      if (thread_release(*id) != ERROR_OK)
	CORE_ESCAPE("unable to release the thread");
    }

  CORE_LEAVE();
}

/*
 * this function loads an execution context in the thread.
 *
 * note that since this operation is machine-specific, the implementation
 * is left to the machine.
 */

t_error			thread_load(i_thread			threadid,
				    s_thread_context		context)
{
  if (machine_call(thread, load, threadid, context) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this fonction stores the thread execution context in the given context.
 *
 * note that since this operation is machine-specific, the implementation
 * is left to the machine.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) call the machine.
 */

t_error			thread_store(i_thread			threadid,
				     s_thread_context*		context)
{
  /*
   * 0)
   */

  if (context == NULL)
    CORE_ESCAPE("the 'context' argument is null");

  /*
   * 1)
   */

  if (machine_call(thread, store, threadid, context) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function returns the identifier of the thread being currently
 * executed.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the scheduler associated with the current CPU.
 * 2) return the identifier of the thread being currently scheduled.
 */

t_error			thread_current(i_thread*		thread)
{
  o_scheduler*		scheduler;

  /*
   * 0)
   */

  if (thread == NULL)
    CORE_ESCAPE("the 'thread' argument is null");

  /*
   * 1)
   */

  if (scheduler_current(&scheduler) == ERROR_OK)
    CORE_ESCAPE("unable to retrieve the current scheduler");

  /*
   * 2)
   */

  *thread = scheduler->thread;

  CORE_LEAVE();
}

/*
 * this function returns true if the given thread object exists.
 */

t_error			thread_exist(i_thread			threadid)
{
  if (set_exist(_thread->threads, threadid) != ERROR_TRUE)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function retrieves the thread object identified by the given
 * identifier.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the object from the set.
 */

t_error			thread_get(i_thread			id,
				   o_thread**			object)
{
  /*
   * 0)
   */

  if (object == NULL)
    CORE_ESCAPE("the 'object' argument is null");

  /*
   * 1)
   */

  if (set_get(_thread->threads, id, (void**)object) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the object from the set of threads");

  CORE_LEAVE();
}

/*
 * this function initializes the thread manager.
 *
 * steps:
 *
 * 1) display a message.
 * 2) allocate and initialize the thread manager's structure.
 * 3) build the identifier object.
 * 4) reserve the set of threads.
 * 5) reserve the set of dead threads i.e the morgue.
 * 6) initialize the morgue's timer as being unused. this is because, at
 *    that time, timers cannot be reserved because the timer manager has
 *    not been set up yet.
 * 7) reserve the kernel thread though this thread will never be scheduled
 *    per say.
 * 8) block the thread: this is to make things clear that this thread
 *    is not supposed to be scheduled.
 * 9) call the machine.
 */

t_error			thread_initialize(void)
{
  /*
   * 1)
   */

  module_call(console, message,
	      '+', "initializing the thread manager\n");

  /*
   * 2)
   */

  if ((_thread = malloc(sizeof (m_thread))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the thread manager's "
		"structure");

  memset(_thread, 0x0, sizeof (m_thread));

  /*
   * 3)
   */

  if (id_build(&_thread->id) != ERROR_OK)
    CORE_ESCAPE("unable to build the identifier object");

  /*
   * 4)
   */

  if (set_reserve(ll,
		  SET_OPTION_ALLOCATE,
		  sizeof(o_thread),
		  &_thread->threads) != ERROR_OK)
    CORE_ESCAPE("unable to reserve the set of threads");

  /*
   * 5)
   */

  if (set_reserve(ll,
		  SET_OPTION_ALLOCATE | SET_OPTION_SORT,
		  sizeof (i_thread),
		  &_thread->morgue.field) != ERROR_OK)
    CORE_ESCAPE("unable to reserve the set of dead threads i.e the morgue");

  /*
   * 6)
   */

  _thread->morgue.timer = ID_UNUSED;

  /*
   * 7)
   */

  if (thread_reserve(_kernel->task,
		     THREAD_PRIORITY,
		     &_kernel->thread) != ERROR_OK)
    CORE_ESCAPE("unable to reserve the kernel thread");

  /*
   * 8)
   */

  if (thread_block(_kernel->thread) != ERROR_OK)
    CORE_ESCAPE("unable to block the kernel thread");

  /*
   * 9)
   */

  if (machine_call(thread, initialize) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * destroy the thread manager.
 *
 * steps:
 *
 * 1) display a message.
 * 2) call the machine.
 * 3) go through the threads.
 *   a) retrieve the thread identifier.
 *   b) release the thread.
 * 4) if required, release the morgue's timer.
 * 5) release the set of dead threads.
 * 6) release the set of threads.
 * 7) destroy the identifier object.
 * 8) release the manager's structure memory.
 */

t_error			thread_clean(void)
{
  s_iterator		i;
  t_state		s;

  /*
   * 1)
   */

  module_call(console, message,
	      '+', "cleaning the thread manager\n");

  /*
   * 2)
   */

  if (machine_call(thread, clean) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 3)
   */

  set_foreach(SET_OPTION_FORWARD, _thread->threads, &i, s)
    {
      i_thread*		thread;

      /*
       * a)
       */

      if (set_object(_thread->threads, i, (void**)&thread) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the thread identifier");

      /*
       * b)
       */

      if (thread_release(*thread) != ERROR_OK)
	CORE_ESCAPE("unable to release the thread");
    }

  /*
   * 4)
   */

  if (_thread->morgue.timer != ID_UNUSED)
    {
      if (timer_release(_thread->morgue.timer) != ERROR_OK)
	CORE_ESCAPE("unable to release the timer");
    }

  /*
   * 5)
   */

  if (set_release(_thread->morgue.field) != ERROR_OK)
    CORE_ESCAPE("unable to release the set of dead threads");

  /*
   * 6)
   */

  if (set_release(_thread->threads) != ERROR_OK)
    CORE_ESCAPE("unable to release the set of threads");

  /*
   * 7)
   */

  if (id_destroy(&_thread->id) != ERROR_OK)
    CORE_ESCAPE("unable to destroy the identifier object");

  /*
   * 8)
   */

  free(_thread);

  CORE_LEAVE();
}
