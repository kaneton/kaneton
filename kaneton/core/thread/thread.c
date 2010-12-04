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
 * this file implements the thread manager.
 * a thread objecct contains:
 *  - an execution context (architecture-depedent)
 *  - information relevant to its own stack
 *  - scheduling information (state, priority within the task)
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

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
 * the thread manager variable.
 */

m_thread*		_thread = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function shows a given thread.
 *
 * steps:
 *
 * 1) get the thread object from its identifier.
 * 2) display the thread information.
 * 3) call the machine-dependent code.
 */

t_error			thread_show(i_thread			threadid)
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
	state = "started";

	break;
      }
    case THREAD_STATE_STOP:
      {
	state = "stopped";

	break;
      }
    case THREAD_STATE_BLOCK:
      {
	state = "blocked";

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

  module_call(console, console_message,
	      '#', "  thread %qu from task %qu is %s with a priority of %u\n",
	      threadid, o->task, state, o->priority);

  /*
   * 3)
   */

  if (machine_call(thread, thread_show, threadid) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function dumps the thread manager.
 *
 * steps:
 *
 * 1) get the set size.
 * 2) show each thread.
 */

t_error			thread_dump(void)
{
  t_state		st;
  o_thread*		data;
  t_setsz		size;
  t_iterator		i;

  /*
   * 1)
   */

  if (set_size(_thread->threads, &size) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the size of the set of threads");

  /*
   * 2)
   */

  module_call(console, console_message,
	      '#', "dumping %qu thread(s):\n", size);

  set_foreach(SET_OPTION_FORWARD, _thread->threads, &i, st)
    {
      if (set_object(_thread->threads, i, (void**)&data) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the thread identifier");

      if (thread_show(data->id) != ERROR_OK)
	CORE_ESCAPE("unable to show the thread");
    }

  CORE_LEAVE();
}

/*
 * this function reserves a thread for a given task.
 *
 * steps:
 *
 * 1) sanity checks.
 * 2) get the task object.
 * 3) reserve an id for the new thread and fill its information.
 * 4) fill thread information.
 * 5) add the new thread in the thread container and in the task threads list.
 * 6) call the machine-dependent code.
 */

t_error			thread_reserve(i_task			taskid,
				       t_priority		prior,
				       i_thread*		threadid)
{
  o_task*		task;
  o_thread		o;

  assert(threadid != NULL);

  /*
   * 1)
   */

  if ((prior < THREAD_PRIORITY_LOW) ||
      (prior > THREAD_PRIORITY_HIGH))
    CORE_ESCAPE("invalid priority");

  /*
   * 2)
   */

  if (task_get(taskid, &task) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 3)
   */

  if (id_reserve(&_thread->id, threadid) != ERROR_OK)
    CORE_ESCAPE("unable to reserve a thread identifier");

  /*
   * 4)
   */

  memset(&o, 0x0, sizeof(o_thread));

  o.id = *threadid;
  o.task = taskid;
  o.priority = prior;
  o.state = THREAD_STATE_STOP;
  o.waits = ID_UNUSED;

  /*
   * XXX
   */

  if (set_reserve(array,
		  SET_OPTION_ALLOC,
		  THREAD_WAITS_INITSZ,
		  sizeof (i_thread),
		  &o.waits) != ERROR_OK)
    CORE_ESCAPE("unable to reserve a set for the waiting tasks/threads");

  /*
   * 5)
   */

  if (set_add(_thread->threads, &o) != ERROR_OK)
    CORE_ESCAPE("unable to add the object to the set of threads");

  if (set_add(task->threads, &o.id) != ERROR_OK)
    CORE_ESCAPE("unable to add the thread to the task's set of threads");

  /*
   * 6)
   */

  if (machine_call(thread, thread_reserve, taskid, threadid) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function releases a thread from a given task.
 *
 * steps:
 *
 * 1) call the machine-dependent code.
 * 2) get the thread object.
 * 3) get the task object.
 * 4) release the thread stack if needed.
 * 5) release the thread-s object identifer.
 * 6) remove the thread from the task threads list.
 * 7) remove the thread from the threads set.
 */

t_error			thread_release(i_thread			threadid)
{
  o_task*		task;
  o_thread*		o;

  /*
   * 1)
   */

  if (machine_call(thread, thread_release, threadid) != ERROR_OK)
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

  if (o->stack)
    {
      if (map_release(task->as, o->stack) != ERROR_OK)
	CORE_ESCAPE("unable to release the map for the thread's stack");
    }

  /*
   * 5)
   */

  if (id_release(&_thread->id, o->id) != ERROR_OK)
    CORE_ESCAPE("unable to release the thread identifier");

  /*
   * 6)
   */

  if (set_remove(task->threads, threadid) != ERROR_OK)
    CORE_ESCAPE("unable to remove the thread from the task's set of threads");

  /*
   * 7)
   */

  if (set_remove(_thread->threads, threadid) != ERROR_OK)
    CORE_ESCAPE("unable to remove the thread identifier from the set of "
		"threads");

  CORE_LEAVE();
}

/*
 * update the thread priority
 *
 * steps:
 *
 * 1) sanity checs.
 * 2) get the thread object from the thread container.
 * 3) update the priority if needed.
 * 4) call the machine-dependent code.
 * 5) call the scheduler to update its internal data.
 */

t_error			thread_priority(i_thread		threadid,
					t_priority		prior)
{
  o_task*		task;
  o_thread*		o;

  /*
   * 1)
   */

  if ((prior < THREAD_PRIORITY_LOW) ||
      (prior > THREAD_PRIORITY_HIGH))
    CORE_ESCAPE("invalid priority");

  /*
   * 2)
   */

  if (thread_get(threadid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  if (task_get(o->task, &task) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 3)
   */

  if (o->priority == prior)
    CORE_LEAVE();

  o->priority = prior;

  /*
   * 4)
   */

  if (machine_call(thread, thread_priority, threadid, prior) != ERROR_OK)
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
 * set the thread as running.
 *
 * steps:
 *
 * 1) check wether the requested state is valid.
 * 2) get the thread object from the thread container.
 * 3) update the thread state if needed.
 * 4) XXX
 * 5) call the machine-dependent code.
 * 6) call the scheduler to schedule or cancel the thread.
 */

t_error			thread_start(i_thread			id)
{
  o_thread*		object;
  o_task*		task;
  t_state		st;
  t_iterator		it;

  /*
   * 2)
   */

  if (thread_get(id, &object) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  /*
   * 1)
   */

  if (object->state == THREAD_STATE_START)
    CORE_ESCAPE("a thread cannot be started twice");

  /*
   * XXX
   */

  object->state = THREAD_STATE_START;

  /*
   * XXX
   */

  object->status.cause = WAIT_STATE_START;
  object->status.value = WAIT_VALUE_UNKNOWN;

  /*
   * 4)
   */

 try:
  set_foreach(SET_OPTION_FORWARD, object->waits, &it, st)
    {
      i_thread*		i;
      o_thread*		o;

      if (set_object(object->waits, it, (void**)&i) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the waiting thread's identifier");

      if (thread_get(*i, &o) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the waiting thread");

      if (o->wait.state & WAIT_STATE_START)
	{
	  o->wait.cause = object->status.cause;
	  o->wait.value = object->status.value;

	  if (thread_start(o->id) != ERROR_OK)
	    CORE_ESCAPE("unable to start the waiting thread");

	  if (set_remove(object->waits, o->id) != ERROR_OK)
	    CORE_ESCAPE("unable to remove the thread from the waiting list");

	  goto try;
	}
    }

  if (machine_call(thread, thread_start, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * XXX
   */

  if (task_get(object->task, &task) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  if (task->state == TASK_STATE_START)
    {
      if (scheduler_add(id) != ERROR_OK)
	CORE_ESCAPE("unable to add the thread to the scheduler");
    }

  CORE_LEAVE();
}

/*
 * set the thread as stopped.
 *
 * steps:
 *
 * 1) check wether the requested state is valid.
 * 2) get the thread object from the thread container.
 * 3) update the thread state if needed.
 * 4) XXX
 * 5) call the machine-dependent code.
 * 6) call the scheduler to schedule or cancel the thread.
 */

t_error			thread_stop(i_thread			id)
{
  o_task*		task;
  o_thread*		object;
  t_state		state;
  t_state		st;
  t_iterator		it;

  /*
   * 2)
   */

  if (thread_get(id, &object) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  if (task_get(object->task, &task) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * XXX
   */

  state = object->state;

  /*
   * 1)
   */

  if (object->state == THREAD_STATE_STOP)
    CORE_ESCAPE("a thread cannot be stopped twice");

  /*
   * XXX
   */

  object->state = THREAD_STATE_STOP;

  /*
   * XXX
   */

  object->status.cause = WAIT_STATE_STOP;
  object->status.value = WAIT_VALUE_UNKNOWN;

  /*
   * 4)
   */

 try:
  set_foreach(SET_OPTION_FORWARD, object->waits, &it, st)
    {
      i_thread*		i;
      o_thread*		o;

      if (set_object(object->waits, it, (void**)&i) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the waiting thread's identifier");

      if (thread_get(*i, &o) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the waiting thread");

      if (o->wait.state & WAIT_STATE_STOP)
	{
	  o->wait.cause = object->status.cause;
	  o->wait.value = object->status.value;

	  if (thread_start(o->id) != ERROR_OK)
	    CORE_ESCAPE("unable to start the waiting thread");

	  if (set_remove(object->waits, o->id) != ERROR_OK)
	    CORE_ESCAPE("unable to remove the thread from the waiting list");

	  goto try;
	}
    }

  if (machine_call(thread, thread_stop, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * XXX
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
 * set the thread as blocking
 *
 * steps:
 *
 * 1) check wether the requested state is valid.
 * 2) get the thread object from the thread container.
 * 3) update the thread state if needed.
 * 4) XXX
 * 5) call the machine-dependent code.
 * 6) call the scheduler to schedule or cancel the thread.
 */

t_error			thread_block(i_thread			id)
{
  o_task*		task;
  o_thread*		object;
  t_state		state;

  /*
   * 2)
   */

  if (thread_get(id, &object) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  if (task_get(object->task, &task) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * XXX
   */

  state = object->state;

  /*
   * 1)
   */

  if (object->state == THREAD_STATE_BLOCK)
    CORE_ESCAPE("a thread cannot be blocked twice");

  /*
   * XXX
   */

  object->state = THREAD_STATE_BLOCK;

  /*
   * 5)
   */

  if (machine_call(thread, thread_block, id) != ERROR_OK)
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
 * set the thread as dead.
 *
 * steps:
 *
 * 1) check wether the requested state is valid.
 * 2) get the thread object from the thread container.
 * 3) update the thread state if needed.
 * 4) XXX
 * 5) call the machine-dependent code.
 * 6) call the scheduler to schedule or cancel the thread.
 */

t_error			thread_exit(i_thread			id,
				    t_value			value)
{
  o_task*		task;
  o_thread*		object;
  t_state		state;
  t_state		st;
  t_iterator		it;
  t_boolean		w;

  /*
   * 2)
   */

  if (thread_get(id, &object) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  if (task_get(object->task, &task) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * XXX
   */

  state = object->state;

  /*
   * 1)
   */

  if (object->state == THREAD_STATE_ZOMBIE)
    CORE_ESCAPE("a thread cannot exit twice");

  /*
   * XXX
   */

  object->state = THREAD_STATE_ZOMBIE;

  /*
   * XXX
   */

  object->status.cause = WAIT_STATE_DEATH;
  object->status.value = value;

  /*
   * 4)
   */

  w = BOOLEAN_FALSE;

 try:
  set_foreach(SET_OPTION_FORWARD, object->waits, &it, st)
    {
      i_thread*		i;
      o_thread*		o;

      if (set_object(object->waits, it, (void**)&i) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the waiting thread's identifier");

      if (thread_get(*i, &o) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the waiting thread");

      if (o->wait.state & WAIT_STATE_DEATH)
	{
	  o->wait.cause = object->status.cause;
	  o->wait.value = object->status.value;

	  if (thread_start(o->id) != ERROR_OK)
	    CORE_ESCAPE("unable to start the waiting thread");

	  if (set_remove(object->waits, o->id) != ERROR_OK)
	    CORE_ESCAPE("unable to remove the thread from the waiting list");

	  w = BOOLEAN_TRUE;

	  goto try;
	}
    }

  if (machine_call(thread, thread_exit, id, value) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * XXX
   */

  if (w == BOOLEAN_TRUE)
    {
      i_timer		useless;
      i_thread*		data;

      /*
       * XXX
       */

      if ((data = malloc(sizeof(i_thread))) == NULL)
	CORE_ESCAPE("unable to allocate memory for the thread identifier");

      *data = id;

      /*
       * XXX
       */

      object->state = THREAD_STATE_DEAD;

      /*
       * XXX
       */

      if (timer_reserve(TIMER_FUNCTION,
			TIMER_HANDLER(thread_bury),
			(t_vaddr)data,
			THREAD_DELAY_BURY,
			TIMER_OPTION_NONE,
			&useless) != ERROR_OK)
	CORE_ESCAPE("unable to reserve a timer");
    }

  /*
   * XXX
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
 * this function is called through a timer to make sure the thread to bury
 * is no longer in the scheduler. indeed, this removal cannot be performed
 * while the thread is currently running.
 *
 * this function buries the thread by releasing its resources.
 */

void			thread_bury(i_timer			timer,
				    t_vaddr			address)
{
  i_thread*		data = (i_thread*)address;
  o_thread*		thread;
  o_task*		task;

  /*
   * XXX
   */

  assert(thread_get(*data, &thread) == ERROR_OK);

  /*
   * XXX
   */

  assert(task_get(thread->task, &task) == ERROR_OK);

  /*
   * XXX
   */

  assert(thread_release(thread->id) == ERROR_OK);

  /*
   * XXX
   */

  if (set_empty(task->threads) == ERROR_TRUE)
    assert(task_exit(task->id, WAIT_VALUE_UNKNOWN) == ERROR_OK);

  /*
   * XXX
   */

  free(data);
}

/*
 * this function waits for a thread to change state.
 *
 * XXX wait for the current thread
 */

t_error			thread_wait(t_state			state,
				    i_thread			id,
				    t_wait*			wait)
{
  i_thread		thread;
  o_thread*		object;
  o_thread*		o;

  assert(wait != NULL);

  /*
   * XXX
   */

  if (scheduler_current(&thread) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the currently scheduled thread");

  /*
   * XXX
   */

  if (thread_get(thread, &object) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  if (thread_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  /*
   * XXX
   */

  if ((state & WAIT_STATE_START) &&
      (o->state == THREAD_STATE_START))
    {
      /*
       * XXX
       */

      wait->id.thread = id;
      wait->state = state;
      wait->cause = WAIT_STATE_START;
      wait->value = WAIT_VALUE_UNKNOWN;

      CORE_LEAVE();
    }

  if ((state & WAIT_STATE_STOP) &&
      (o->state == THREAD_STATE_STOP))
    {
      /*
       * XXX
       */

      wait->id.thread = id;
      wait->state = state;
      wait->cause = WAIT_STATE_STOP;
      wait->value = WAIT_VALUE_UNKNOWN;

      CORE_LEAVE();
    }

  if ((state & WAIT_STATE_DEATH) &&
      (o->state == THREAD_STATE_ZOMBIE))
    {
      i_timer		useless;
      i_thread*		data;

      /*
       * XXX
       */

      wait->id.thread = id;
      wait->state = state;
      wait->cause = o->status.cause;
      wait->value = o->status.value;

      /*
       * XXX
       */

      if ((data = malloc(sizeof(i_thread))) == NULL)
	CORE_ESCAPE("unable to allocate memory for the thread identifier");

      *data = id;

      /*
       * XXX
       */

      o->state = THREAD_STATE_DEAD;

      /*
       * XXX
       */

      if (timer_reserve(TIMER_FUNCTION,
			TIMER_HANDLER(thread_bury),
			(t_vaddr)data,
			5000,
			TIMER_OPTION_NONE,
			&useless) != ERROR_OK)
	MACHINE_ESCAPE("unable to reserve a timer");

      CORE_LEAVE();
    }

  /*
   * XXX
   */

  object->wait.state = state;

  /*
   * XXX
   */

  if (set_add(o->waits, &thread) != ERROR_OK)
    CORE_ESCAPE("unable to add the thread identifier to the waiting list");

  /*
   * XXX
   */

  if (thread_stop(thread) != ERROR_OK)
    CORE_ESCAPE("unable to stop the task");

  /*
   * XXX
   */

  wait->id.thread = id;
  wait->state = object->wait.state;
  wait->cause = object->wait.cause;
  wait->value = object->wait.value;

  /*
   * XXX
   */

  object->wait.state = WAIT_STATE_NONE;

  CORE_LEAVE();
}

/*
 * allocate the thread stack.
 *
 * steps:
 *
 * 1) check whether the stack is large enough.
 * 2) get the thread object from the threads container.
 * 3) get the task owning the thread.
 * 3) allocate the thread stack in the task address space.
 * 4) call the machine-dependent code.
 */

t_error			thread_stack(i_thread			threadid,
				     t_stack			stack)
{
  o_thread*		o;
  o_task*		task;

  /*
   * 1)
   */

  if (stack.size < THREAD_STACKSZ_LOW)
    CORE_ESCAPE("the provided stack size is too low");

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

  if (stack.base == 0)
    {
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
    o->stack = stack.base;

  o->stacksz = stack.size;

  /*
   * 5)
   */

  if (machine_call(thread, thread_stack, threadid, stack) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function pushes arguments on a thread's stack.
 */

t_error			thread_args(i_thread			threadid,
				    const void*			args,
				    t_vsize			size)
{
  if (machine_call(thread, thread_args, threadid, args, size) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * XXX
 */

void			thread_sleep_handler(i_timer		timer,
					     t_vaddr		address)
{
  i_thread*		data = (i_thread*)address;

  /*
   *
   */

  assert(thread_start(*data) == ERROR_OK);

  /*
   *
   */

  free(data);
}

/*
 * XXX
 *
 * XXX sleep for the current thread
 */

t_error			thread_sleep(t_uint32			milliseconds)
{
  i_timer		useless;
  i_thread*		data;
  i_thread		id;
  o_thread*		o;

  /*
   *
   */

  if (scheduler_current(&id) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the currently scheduled thread");

  if (thread_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  if (o->state != THREAD_STATE_START)
    CORE_ESCAPE("unable to put to sleep a non-running thread");

  /*
   *
   */

  if ((data = malloc(sizeof(i_thread))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the thread identifier");

  *data = id;

  /*
   *
   */

  if (timer_reserve(TIMER_FUNCTION,
                    TIMER_HANDLER(thread_sleep_handler),
                    (t_vaddr)data,
                    milliseconds,
                    TIMER_OPTION_NONE,
                    (i_timer*)&useless) != ERROR_OK)
    CORE_ESCAPE("unable to reserve a timer");

  /*
   *
   */

  if (thread_block(id) != ERROR_OK)
    CORE_ESCAPE("unable to block the thread");

  CORE_LEAVE();
}

/*
 * this function removes every thread that belongs to the specified task.
 *
 * steps:
 *
 * 1) call the machine-dependent code.
 * 2) gets the task object from its identifier.
 * 3) for every thread belonging to the task, release it.
 */

t_error			thread_flush(i_task			taskid)
{
  i_thread*		data;
  o_task*		task;
  t_iterator		i;

  /*
   * 1)
   */

  if (machine_call(thread, thread_flush, taskid) != ERROR_OK)
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
      if (set_object(task->threads, i, (void**)&data) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the thread identifier");

      if (thread_release(*data) != ERROR_OK)
	CORE_ESCAPE("unable to release the thread");
    }

  CORE_LEAVE();
}

/*
 * this function loads a new execution context in the thread object.
 */

t_error			thread_load(i_thread			threadid,
				    t_thread_context		context)
{
  if (machine_call(thread, thread_load, threadid, context) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this fonction stores the thread execution context.
 */

t_error			thread_store(i_thread			threadid,
				     t_thread_context*		context)
{
  if (machine_call(thread, thread_store, threadid, context) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function returns true if the thread object exists.
 */

t_error			thread_exist(i_thread			threadid)
{
  if (set_exist(_thread->threads, threadid) != ERROR_TRUE)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function gets an thread object from the thread container.
 */

t_error			thread_get(i_thread			threadid,
				   o_thread**			o)
{
  assert(o != NULL);

  if (set_get(_thread->threads, threadid, (void**)o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the object from the set of threads");

  CORE_LEAVE();
}

/*
 * initialize the thread manager.
 *
 * steps:
 *
 * 1) allocate and initialize the thread manager.
 * 2) initialize the object identifier.
 * 3) reserve the thread set.
 * 4) call the machine-dependent code.
 */

t_error			thread_initialize(void)
{
  /*
   * 1)
   */

  if ((_thread = malloc(sizeof(m_thread))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the thread manager's "
		"structure");

  memset(_thread, 0x0, sizeof(m_thread));

  /*
   * 2)
   */

  if (id_build(&_thread->id) != ERROR_OK)
    CORE_ESCAPE("unable to build the identifier object");

  /*
   * 3)
   */

  if (set_reserve(ll,
		  SET_OPTION_ALLOC,
		  sizeof(o_thread),
		  &_thread->threads) != ERROR_OK)
    CORE_ESCAPE("unable to reserve the set of threads");

  /*
   * 4)
   */

  if (thread_reserve(_kernel->task,
		     THREAD_PRIORITY,
		     &_kernel->thread) != ERROR_OK)
    CORE_ESCAPE("unable to reserve the kernel thread");

  if (thread_block(_kernel->thread) != ERROR_OK)
    CORE_ESCAPE("unable to block the kernel thread");

  /*
   * 5)
   */

  if (machine_call(thread, thread_initialize) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * destroy the thread manager.
 *
 * steps:
 *
 * 1) call the machine-dependent code.
 * 2) release every thread object.
 * 3) destroy the identifier object.
 * 4) free the thread manager's structure memory.
 */

t_error			thread_clean(void)
{
  /*
   * 1)
   */

  if (machine_call(thread, thread_clean) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  if (set_release(_thread->threads) != ERROR_OK)
    CORE_ESCAPE("unable to release the set of threads");

  /*
   * 3)
   */

  if (id_destroy(&_thread->id) != ERROR_OK)
    CORE_ESCAPE("unable to destroy the identifier object");

  /*
   * 4)
   */

  free(_thread);

  CORE_LEAVE();
}
