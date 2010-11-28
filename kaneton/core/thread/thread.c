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
    case THREAD_STATE_RUN:
      {
	state = "running";

	break;
      }
    case THREAD_STATE_STOP:
      {
	state = "stopped";

	break;
      }
    case THREAD_STATE_ZOMBIE:
      {
	state = "zombie";

	break;
      }
    case THREAD_STATE_BLOCK:
      {
	state = "blocked";

	break;
      }
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
 * this funtion unlinks a thread from its task and give it to another one.
 * XXX THREAD: copier la pile dans son nouvel address space (ou pas).
 *
 * steps:
 *
 * 1) get the thread to give from the threads container.
 * 2) check if the thread does not already belong to the specified task.
 * 3) get the destination task from the tasks container.
 * 4) get the source task from the tasks container.
 * 5) remove the thread from the source task.
 * 6) add the thread to the destination task.
 * 7) call the machine-dependent code.
 */

t_error			thread_give(i_task			taskid,
				    i_thread			threadid)
{
  o_thread*		o;
  o_task*		src;
  o_task*		dest;

  /*
   * 1)
   */

  if (thread_get(threadid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  /*
   * 2)
   */

  if (o->task == taskid)
    CORE_LEAVE();

  /*
   * 3)
   */

  if (task_get(taskid, &dest) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 4)
   */

  if (task_get(o->task, &src) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 5)
   */

  if (set_remove(src->threads, threadid) != ERROR_OK)
    CORE_ESCAPE("unable to remove the thread from the task's set of threads");

  /*
   * 6)
   */

  o->task = taskid;

  if (set_add(dest->threads, &threadid) != ERROR_OK)
    CORE_ESCAPE("unable to add the object to the task's set of threads");

  /*
   * 7)
   */

  // XXX what if the thread is running? should it be moved to the task's CPU?
  // XXX update CR3 value?

  if (machine_call(thread, thread_give, taskid, threadid) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * clone a thread.
 * the clone is added to the specified task.
 *
 * 1) get the thread to clone.
 * 2) reserve the clone objet.
 * 3) copy data from the original thead to the new one.
 * 4) update thread state.
 * 4) copy the waits set from the original thread to the new one.
 * 5) call the machine-dependent code.
 */

t_error			thread_clone(i_task			taskid,
				     i_thread			old,
				     i_thread*			new)
{
  o_thread*		from;
  o_thread*		to;
  //  t_state		state; USED IN 4)
  //  t_iterator	i; USED IN 4)
  i_thread		threadid;

  assert(new != NULL);

  /*
   * 1)
   */

  if (thread_get(old, &from) != ERROR_OK)
    CORE_ESCAPE("unable to retrive the thread object");

  /*
   * 2)
   */

  if (thread_reserve(taskid, from->priority, new) != ERROR_OK)
    CORE_ESCAPE("unable to reserve a thread");

  threadid = *new;

  if (thread_get(threadid, &to) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  /*
   * 3)
   */

  to->state = from->state;

  to->stack = from->stack;
  to->stacksz = from->stacksz;

  /*
   * 4)
   */

  switch(to->state)
    {
      case THREAD_STATE_RUN:
	{
	  if (scheduler_add(threadid) != ERROR_OK)
	    CORE_ESCAPE("unable to add the thread to the scheduler");

	  break;
	}
      case THREAD_STATE_STOP:
	{
	  if (scheduler_remove(threadid) != ERROR_OK)
	    CORE_ESCAPE("unable to remove the thread from the scheduler");

	  break;
	}
      case THREAD_STATE_ZOMBIE:
	{
	  /* XXX */

	  break;
	}
      case THREAD_STATE_BLOCK:
	{
	  if (scheduler_remove(threadid) != ERROR_OK)
	    CORE_ESCAPE("unable to remove the thread from the scheduler");

	  break;
	}
    }

  /*
   * 5)
   */

  if (machine_call(thread, thread_clone, taskid, old, new) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

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

  if (prior < THREAD_LPRIORITY || prior > THREAD_HPRIORITY)
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
  o_thread*		o;

  /*
   * 1)
   */

  if (prior < THREAD_LPRIORITY || prior > THREAD_HPRIORITY)
    CORE_ESCAPE("invalid priority");

  /*
   * 2)
   */

  if (thread_get(threadid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

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

  if (o->state == THREAD_STATE_RUN)
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

t_error			thread_run(i_thread			threadid)
{
  o_thread*		o;
  //  t_iterator		i;
  //  t_state		st;
  o_task*		task;
  t_state		wakeup;

  /*
   * 2)
   */

  if (thread_get(threadid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  if (task_get(o->task, &task) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 1)
   */

  if (o->state == THREAD_STATE_RUN)
    CORE_LEAVE();

  /*
   * XXX
   */

  o->state = THREAD_STATE_RUN;

  /*
   * 4)
   */
/*
  set_foreach(SET_OPTION_FORWARD, o->waits, &i, st)
    {
      o_waitfor*	w;

      if (set_object(o->waits, i, (void**)&w) != ERROR_OK)
      XXX;

      if (w->options & wakeup)
      if (thread_run(w->u.thread) != ERROR_OK)
      XXX;
    }
*/
  /*
   * 5)
   */

  if (machine_call(thread, thread_run, threadid) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 6)
   */

  if (task->state == TASK_STATE_RUN)
    {
      if (scheduler_add(threadid) != ERROR_OK)
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

t_error			thread_stop(i_thread			threadid)
{
  o_thread*		o;
  //  t_iterator		i;
  //  t_state		st;
  o_task*		task;
  t_state		wakeup;
  t_state		state;

  /*
   * 2)
   */

  if (thread_get(threadid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  if (task_get(o->task, &task) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  state = o->state;

  /*
   * 1)
   */

  if (o->state == THREAD_STATE_STOP)
    CORE_LEAVE();

  /*
   * XXX
   */

  o->state = THREAD_STATE_STOP;

  /*
   * 4)
   */
/*
  set_foreach(SET_OPTION_FORWARD, o->waits, &i, st)
    {
      o_waitfor*	w;

      if (set_object(o->waits, i, (void**)&w) != ERROR_OK)
XXX;
      if (w->options & wakeup)
      if (thread_run(w->u.thread) != ERROR_OK)
      XXX;
    }
*/
  /*
   * 5)
   */

  if (machine_call(thread, thread_stop, threadid) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 6)
   */

  if (state == TASK_STATE_RUN)
    {
      if (scheduler_remove(threadid) != ERROR_OK)
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

t_error			thread_block(i_thread			threadid)
{
  o_thread*		o;
  //  t_iterator		i;
  //  t_state		st;
  o_task*		task;
  t_state		wakeup;
  t_state		state;

  /*
   * 2)
   */

  if (thread_get(threadid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  if (task_get(o->task, &task) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  state = o->state;

  /*
   * 1)
   */

  if (o->state == THREAD_STATE_BLOCK)
    CORE_LEAVE();

  /*
   * XXX
   */

  o->state = THREAD_STATE_BLOCK;

  /*
   * 5)
   */

  if (machine_call(thread, thread_block, threadid) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 6)
   */

  if (state == TASK_STATE_RUN)
    {
      if (scheduler_remove(threadid) != ERROR_OK)
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

t_error			thread_die(i_thread			threadid)
{
  o_thread*		o;
  //  t_iterator		i;
  //  t_state		st;
  o_task*		task;
  t_state		wakeup;
  t_state		state;

  /*
   * 2)
   */

  if (thread_get(threadid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  if (task_get(o->task, &task) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  state = o->state;

  /*
   * 1)
   */

  if (o->state == THREAD_STATE_ZOMBIE)
    CORE_LEAVE();

  /*
   * XXX
   */

  o->state = THREAD_STATE_ZOMBIE;

  /*
   * 4)
   */
/*
  set_foreach(SET_OPTION_FORWARD, o->waits, &i, st)
    {
      o_waitfor*	w;

      if (set_object(o->waits, i, (void**)&w) != ERROR_OK)
      XXX;

      if (w->options & wakeup)
      if (thread_run(w->u.thread) != ERROR_OK)
      XXX;
    }

    // XXX if list of waits empty -> release thread
*/

  /*
   * 5)
   */

  if (machine_call(thread, thread_die, threadid) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 6)
   */

  if (state == TASK_STATE_RUN)
    {
      if (scheduler_remove(threadid) != ERROR_OK)
	CORE_ESCAPE("unable to remove the thread from the scheduler");
    }

  CORE_LEAVE();
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
      (o->state == TASK_STATE_RUN))
    CORE_LEAVE();

  if ((state & WAIT_STATE_STOP) &&
      (o->state == TASK_STATE_STOP))
    CORE_LEAVE();

  if ((state & WAIT_STATE_DEATH) &&
      (o->state == TASK_STATE_ZOMBIE))
    CORE_LEAVE();

  /*
   * XXX
   */

  object->wait.id.thread = id;
  object->wait.state = state;
  object->wait.cause = WAIT_STATE_UNKNOWN;
  object->wait.status = WAIT_STATUS_UNKNOWN;

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

  memcpy(wait, &o->wait, sizeof (t_wait));

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

  if (stack.size < THREAD_LSTACKSZ)
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

  assert(thread_run(*data) == ERROR_OK);

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

  if (thread_get(id, (void**)&o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  if (o->state != THREAD_STATE_RUN)
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
  o_thread*		thread;

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
