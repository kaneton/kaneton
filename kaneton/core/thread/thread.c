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

  THREAD_ENTER(_thread);

  /*
   * 1)
   */

  if (thread_get(threadid, &o) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 2)
   */

  switch (o->state)
    {
    case THREAD_STATE_RUN:
      state = "running";
      break;
    case THREAD_STATE_STOP:
      state = "stopped";
      break;
    case THREAD_STATE_ZOMBIE:
      state = "zombie";
      break;
    case THREAD_STATE_BLOCK:
      state = "blocked";
      break;
    }

  module_call(console, console_message,
	      '#', "  thread %qu from task %qu is %s with a priority of %u\n",
	      threadid, o->task, state, o->priority);

  /*
   * 3)
   */

  if (machine_call(thread, thread_show, threadid) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  THREAD_LEAVE(_thread, ERROR_OK);
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

  THREAD_ENTER(_thread);

  /*
   * 1)
   */

  if (set_size(_thread->threads, &size) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 2)
   */

  module_call(console, console_message,
	      '#', "dumping %qu thread(s):\n", size);

  set_foreach(SET_OPTION_FORWARD, _thread->threads, &i, st)
    {
      if (set_object(_thread->threads, i, (void**)&data) != ERROR_OK)
	THREAD_LEAVE(_thread, ERROR_KO);

      if (thread_show(data->id) != ERROR_OK)
	THREAD_LEAVE(_thread, ERROR_KO);
    }

  THREAD_LEAVE(_thread, ERROR_OK);
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

  THREAD_ENTER(_thread);

  /*
   * 1)
   */

  if (thread_get(threadid, &o) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 2)
   */

  if (o->task == taskid)
    THREAD_LEAVE(_thread, ERROR_OK);

  /*
   * 3)
   */

  if (task_get(taskid, &dest) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 4)
   */

  if (task_get(o->task, &src) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 5)
   */

  if (set_remove(src->threads, threadid) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 6)
   */

  o->task = taskid;
  if (set_add(dest->threads, &threadid) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 7)
   */

  // XXX update CR3 value.

  if (machine_call(thread, thread_give, taskid, threadid) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  THREAD_LEAVE(_thread, ERROR_OK);
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

  THREAD_ENTER(_thread);

  assert(new != NULL);

  /*
   * 1)
   */

  if (thread_get(old, &from) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 2)
   */

  if (thread_reserve(taskid, from->priority, new) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  threadid = *new;

  if (thread_get(threadid, &to) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);


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
	if (scheduler_add(threadid) != ERROR_OK)
	  THREAD_LEAVE(_thread, ERROR_KO);
	break;
      case THREAD_STATE_STOP:
	if (scheduler_remove(threadid) != ERROR_OK)
	  THREAD_LEAVE(_thread, ERROR_KO);
	break;
      case THREAD_STATE_ZOMBIE:
	/* XXX */
	break;
      case THREAD_STATE_BLOCK:
	if (scheduler_remove(threadid) != ERROR_OK)
	  THREAD_LEAVE(_thread, ERROR_KO);
	break;
    }

  /*
   * 4)
   */
  /*
  set_foreach(SET_OPTION_FORWARD, from->waits, &i, st)
    {
      i_thread*		data;

      if (set_object(from->waits, i, (void**)&data) != ERROR_OK)
	{
	  module_call(console, console_message,
	              '!', "thread: cannot find the object "
		      "corresponding to its identifier\n");

	  THREAD_LEAVE(_thread, ERROR_KO);
	}

      if (set_add(to->waits, data) != ERROR_OK)
	THREAD_LEAVE(_thread, ERROR_KO);
    }
  */
  /*
   * 5)
   */

  if (machine_call(thread, thread_clone, taskid, old, new) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  THREAD_LEAVE(_thread, ERROR_OK);
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

  THREAD_ENTER(_thread);

  assert(threadid != NULL);

  /*
   * 1)
   */

  if (prior < THREAD_LPRIORITY || prior > THREAD_HPRIORITY)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 2)
   */

  if (task_get(taskid, &task) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 3)
   */

  if (id_reserve(&_thread->id, threadid) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 4)
   */

  memset(&o, 0x0, sizeof(o_thread));

  o.id = *threadid;
  o.task = taskid;
  o.priority = prior;
  o.state = THREAD_STATE_STOP;
  o.waits = ID_UNUSED; // XXX

  /*
   * 5)
   */

  if (set_add(_thread->threads, &o) != ERROR_OK)
    {
      id_release(&_thread->id, o.id);

      THREAD_LEAVE(_thread, ERROR_KO);
    }

  if (set_add(task->threads, &o.id) != ERROR_OK)
    {
      set_remove(_thread->threads, o.id);

      id_release(&_thread->id, o.id);

      THREAD_LEAVE(_thread, ERROR_KO);
    }

  /*
   * 6)
   */

  if (machine_call(thread, thread_reserve, taskid, threadid) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  THREAD_LEAVE(_thread, ERROR_OK);
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

  THREAD_ENTER(_thread);

  /*
   * 1)
   */

  if (machine_call(thread, thread_release, threadid) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 2)
   */

  if (thread_get(threadid, &o) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 3)
   */

  if (task_get(o->task, &task) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 4)
   */

  if (o->stack)
    {
      if (map_release(task->as, o->stack) != ERROR_OK)
	THREAD_LEAVE(_thread, ERROR_KO);
    }

  /*
   * 5)
   */

  if (id_release(&_thread->id, o->id) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 6)
   */

  if (set_remove(task->threads, threadid) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 7)
   */

  if (set_remove(_thread->threads, threadid) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  THREAD_LEAVE(_thread, ERROR_OK);
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

  THREAD_ENTER(_thread);

  /*
   * 1)
   */

  if (prior < THREAD_LPRIORITY || prior > THREAD_HPRIORITY)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 2)
   */

  if (thread_get(threadid, &o) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 3)
   */

  if (o->priority == prior)
    THREAD_LEAVE(_thread, ERROR_OK);

  o->priority = prior;

  /*
   * 4)
   */

  if (machine_call(thread, thread_priority, threadid, prior) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 5)
   */

  if (o->state == THREAD_STATE_RUN)
    if (scheduler_update(threadid) != ERROR_OK)
      THREAD_LEAVE(_thread, ERROR_KO);

  THREAD_LEAVE(_thread, ERROR_OK);
}

/*
 * update the thread state.
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

t_error			thread_state(i_thread			threadid,
				     t_state			state)
{
  o_thread*		o;
  //  t_iterator		i;
  //  t_state		st;
  o_task*		task;
  t_state		wakeup;
  t_state		current;

  THREAD_ENTER(_thread);

  /*
   * 2)
   */

  if (thread_get(threadid, &o) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  if (task_get(o->task, &task) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  current = o->state;

  /*
   * 1)
   */

  if (o->state == state)
    THREAD_LEAVE(_thread, ERROR_OK);

  /*
   *
   */

  switch(state)
    {
      case THREAD_STATE_RUN:
	wakeup = WAIT_START;
	break;
      case THREAD_STATE_STOP:
	wakeup = WAIT_STOP;
	break;
      case THREAD_STATE_ZOMBIE:
	wakeup = WAIT_DEATH;
	break;
      case THREAD_STATE_BLOCK:
	wakeup = 0;
	break;
      default:
	THREAD_LEAVE(_thread, ERROR_KO);
    }

  /*
   * XXX
   */

  o->state = state;

  /*
   * 4)
   */
/*
  set_foreach(SET_OPTION_FORWARD, o->waits, &i, st)
    {
      o_waitfor*	w;

      if (set_object(o->waits, i, (void**)&w) != ERROR_OK)
	{
	  module_call(console, console_message, '!', "thread: cannot find the object "
		   "corresponding to its identifier\n");

	  THREAD_LEAVE(_thread, ERROR_KO);
	}

      if (w->options & wakeup)
	thread_state(w->u.thread, THREAD_STATE_RUN);
    }
*/
  /*
   * 5)
   */

  if (machine_call(thread, thread_state, threadid, state) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 6)
   */

  // XXX n'ajouter que les threads en run qui si la task est en run aussi
  switch (state)
    {
      case THREAD_STATE_RUN:
	if (task->state == TASK_STATE_RUN)
	  if (scheduler_add(threadid) != ERROR_OK)
	    THREAD_LEAVE(_thread, ERROR_KO);

	break;
      case THREAD_STATE_STOP:
      case THREAD_STATE_ZOMBIE:
      case THREAD_STATE_BLOCK:
	if (current == THREAD_STATE_RUN)
	  if (scheduler_remove(threadid) != ERROR_OK)
	    THREAD_LEAVE(_thread, ERROR_KO);

	break;
    }

  THREAD_LEAVE(_thread, ERROR_OK);
}

/*
 * allocate the thread stack.
 *
 * steps:
 *
 * 1) check wether the stack is large enough.
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

  THREAD_ENTER(_thread);

  /*
   * 1)
   */

  if (stack.size < THREAD_MIN_STACKSZ)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 2)
   */

  if (thread_get(threadid, &o) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 3)
   */

  if (task_get(o->task, &task) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 4)
   */

  if (!stack.base)
    {
      if (map_reserve(task->as,
		      MAP_OPTION_NONE |
		      (task->class == TASK_CLASS_KERNEL ?
		       MAP_OPTION_PRIVILEGED : MAP_OPTION_USER),
		      stack.size,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &(o->stack)) != ERROR_OK)
	THREAD_LEAVE(_thread, ERROR_KO);
    }
  else
    o->stack = stack.base;

  o->stacksz = stack.size;

  /*
   * 5)
   */

  if (machine_call(thread, thread_stack, threadid, stack) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  THREAD_LEAVE(_thread, ERROR_OK);
}

/*
 * this function pushes arguments on a thread's stack.
 */

t_error			thread_args(i_thread			threadid,
				    const void*			args,
				    t_vsize			size)
{
  THREAD_ENTER(_thread);

  if (machine_call(thread, thread_args, threadid, args, size) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  THREAD_LEAVE(_thread, ERROR_OK);
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

  assert(thread_state(*data, THREAD_STATE_RUN) == ERROR_OK);

  /*
   *
   */

  free(data);
}

/*
 * XXX
 */

t_error			thread_sleep(i_thread			id,
				     t_uint32			milliseconds)
{
  i_timer		useless;
  i_thread*		data;
  o_thread*		o;

  THREAD_ENTER(_thread);

  /*
   *
   */

  if (thread_get(id, (void**)&o) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  if (o->state != THREAD_STATE_RUN)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   *
   */

  data = malloc(sizeof(i_thread));
  *data = id;

  /*
   *
   */

  if (timer_reserve(TIMER_FUNCTION,
                    TIMER_HANDLER(thread_sleep_handler),
                    (t_vaddr)data,
                    milliseconds,
                    TIMER_REPEAT_DISABLE,
                    (i_timer*)&useless) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   *
   */

  if (thread_state(id, THREAD_STATE_BLOCK) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  THREAD_LEAVE(_thread, ERROR_OK);
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

  THREAD_ENTER(_thread);

  /*
   * 1)
   */

  if (machine_call(thread, thread_flush, taskid) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 2)
   */

  if (task_get(taskid, &task) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  /*
   * 3)
   */

  while (set_head(task->threads, &i) == ERROR_OK)
    {
      if (set_object(task->threads, i, (void**)&data) != ERROR_OK)
	{
	  module_call(console, console_message,
		      '!', "thread: cannot find the object "
		      "corresponding to ist identifier\n");

	  THREAD_LEAVE(_thread, ERROR_KO);
	}
      if (thread_release(*data) != ERROR_OK)
	THREAD_LEAVE(_thread, ERROR_KO);
    }

  THREAD_LEAVE(_thread, ERROR_OK);
}

/*
 * this function loads a new execution context in the thread object.
 */

t_error			thread_load(i_thread			threadid,
				    t_thread_context		context)
{
  THREAD_ENTER(_thread);

  if (machine_call(thread, thread_load, threadid, context) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  THREAD_LEAVE(_thread, ERROR_OK);
}

/*
 * this fonction stores the thread execution context.
 */

t_error			thread_store(i_thread			threadid,
				     t_thread_context*		context)
{
  THREAD_ENTER(_thread);

  if (machine_call(thread, thread_store, threadid, context) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  THREAD_LEAVE(_thread, ERROR_OK);
}

/*
 * this function gets an thread object from the thread container.
 */

t_error			thread_get(i_thread			threadid,
				   o_thread**			o)
{
  THREAD_ENTER(_thread);

  assert(o != NULL);

  if (set_get(_thread->threads, threadid, (void**)o) != ERROR_OK)
    THREAD_LEAVE(_thread, ERROR_KO);

  THREAD_LEAVE(_thread, ERROR_OK);
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
    {
      module_call(console, console_message,
		  '!', "thread: cannot allocate memory "
		  "for the thread manager structure\n");

      return (ERROR_KO);
    }

  memset(_thread, 0x0, sizeof(m_thread));

  /*
   * 2)
   */

  if (id_build(&_thread->id) != ERROR_OK)
    {
      module_call(console, console_message,
		  '!', "thread: unable to initialize the identifier object\n");

      return (ERROR_KO);
    }

  /*
   * 3)
   */

  if (set_reserve(ll, SET_OPTION_ALLOC, sizeof(o_thread), &_thread->threads)
      != ERROR_OK)
    {
      module_call(console, console_message,
		  '!', "thread: unable to reserve the thread set\n\n");

      return (ERROR_KO);
    }

  /*
   * 4)
   */

  // XXX

  if (thread_reserve(_kernel->task,
		     THREAD_PRIORITY,
		     &_kernel->thread) != ERROR_OK)
    return (ERROR_KO);

  if (thread_get(_kernel->thread, &thread) != ERROR_OK)
    return (ERROR_KO);

  if (thread_state(_kernel->thread, THREAD_STATE_BLOCK) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 5)
   */

  // XXX

  if (machine_call(thread, thread_initialize) != ERROR_OK)
    return (ERROR_KO);

  return (ERROR_OK);
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
    return (ERROR_KO);

  /*
   * 2)
   */

  if (set_release(_thread->threads) != ERROR_OK)
    {
      module_call(console, console_message,
		  '!', "thread: unable to release the thread set\n");

      return (ERROR_KO);
    }

  /*
   * 3)
   */

  if (id_destroy(&_thread->id) != ERROR_OK)
    {
      module_call(console, console_message,
		  '!', "thread: unable to destroy the identifier object\n");

      return (ERROR_KO);
    }

  /*
   * 4)
   */

  free(_thread);

  return (ERROR_OK);
}
