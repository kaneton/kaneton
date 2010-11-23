/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.NEW/kaneton/core/task/task.c
 *
 * created       julien quintard   [fri jun 22 02:25:26 2007]
 * updated       julien quintard   [tue nov 23 14:47:45 2010]
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

  TASK_ENTER(_task);

  assert(tsk != NULL);

  if (_scheduler == NULL)
    {
      *tsk = _kernel->task;

      TASK_LEAVE(_task, ERROR_OK);
    }

  if (scheduler_current(&current) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  if (thread_get(current, &o) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  *tsk = o->task;

  TASK_LEAVE(_task, ERROR_OK);
}

/*
 * this function shows a precise task.
 */

t_error			task_show(i_task			id)
{
  o_task*		o;
  char*			state;

  TASK_ENTER(_task);

  if (task_get(id, &o) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  switch (o->state)
    {
      case TASK_STATE_STOP:
	state = "stopped";
	break;
      case TASK_STATE_RUN:
	state = "running";
	break;
      case TASK_STATE_ZOMBIE:
	state = "zombie";
	break;
      case TASK_STATE_BLOCK:
	state = "blocked";
	break;
    }

  module_call(console, console_message,
	      '#', "  task %qu on cpu %qd is %s\n",
	      id, o->cpu, state);

  if (machine_call(task, task_show, id) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  TASK_LEAVE(_task, ERROR_OK);
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

  TASK_ENTER(_task);

  /*
   * 1)
   */

  if (set_size(_task->tasks, &size) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  /*
   * 2)
   */

  module_call(console, console_message,
	      '#', "dumping %qu task(s):\n", size);

  set_foreach(SET_OPTION_FORWARD, _task->tasks, &i, state)
    {
      if (set_object(_task->tasks, i, (void**)&data) != ERROR_OK)
	TASK_LEAVE(_task, ERROR_KO);

      if (task_show(data->id) != ERROR_OK)
	TASK_LEAVE(_task, ERROR_KO);
    }

  TASK_LEAVE(_task, ERROR_OK);
}

/*
 * this function clones a task.
 *
 * this function so takes care of cloning the address space and every thread.
 *
 * steps:
 *
 * 1) get the source task object given its identifier.
 * 2) reserve the cloned task object.
 * 3) get the destination task object previously reserved.
 * 4) clone the address space from the source task object.
 * 5) clone the thread set from the source task object.
 * 6) clone the waits.
 * 7) set the new task state.
 * 8) call the machine-dependent code.
 */

t_error			task_clone(i_task			old,
				   i_task*			new)
{
  t_state		state;
  o_task*		from;
  o_task*		to;
  t_iterator		i;

  TASK_ENTER(_task);

  assert(new != NULL);

  /*
   * 1)
   */

  if (task_get(old, &from) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  /*
   * 2)
   */

  if (task_reserve(from->class,
		   from->behaviour,
		   from->priority,
		   new) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  /*
   * 3)
   */

  if (task_get(*new, &to) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  /*
   * 4)
   */

  if (from->as != ID_UNUSED)
    {
      if (as_clone(to->id, from->as, &to->as) != ERROR_OK)
	TASK_LEAVE(_task, ERROR_KO);
    }

  /*
   * 5)
   */

  set_foreach(SET_OPTION_FORWARD, from->threads, &i, state)
    {
      i_thread		needless;
      i_thread*		data;

      if (set_object(from->threads, i, (void**)&data) != ERROR_OK)
	{
	  module_call(console, console_message,
		      '!', "task: cannot find the object "
		      "corresponding to its identifier\n");

	  TASK_LEAVE(_task, ERROR_KO);
	}

      if (thread_clone(to->id, *data, &needless) != ERROR_OK)
	TASK_LEAVE(_task, ERROR_KO);
    }

  /*
   * 6)
   */

  /* XXX waits */

  /*
   * 7)
   */

  to->state = from->state;

  /*
   * 8)
   */

  if (machine_call(task, task_clone, old, new) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  TASK_LEAVE(_task, ERROR_OK);
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

  TASK_ENTER(_task);

  assert(id != NULL);

  /*
   * 1)
   */

  if ((class != TASK_CLASS_KERNEL) &&
      (class != TASK_CLASS_DRIVER) &&
      (class != TASK_CLASS_SERVICE) &&
      (class != TASK_CLASS_GUEST))
    TASK_LEAVE(_task, ERROR_KO);

  if ((behav != TASK_BEHAVIOUR_KERNEL) &&
      (behav != TASK_BEHAVIOUR_REALTIME) &&
      (behav != TASK_BEHAVIOUR_INTERACTIVE) &&
      (behav != TASK_BEHAVIOUR_TIMESHARING) &&
      (behav != TASK_BEHAVIOUR_BACKGROUND))
    TASK_LEAVE(_task, ERROR_KO);

  switch (behav)
    {
      case TASK_BEHAVIOUR_KERNEL:
	if (prior < TASK_LPRIORITY_KERNEL ||
	    prior > TASK_HPRIORITY_KERNEL)
	  TASK_LEAVE(_task, ERROR_KO);
	break;
      case TASK_BEHAVIOUR_REALTIME:
	if (prior < TASK_LPRIORITY_REALTIME ||
	    prior > TASK_HPRIORITY_REALTIME)
	  TASK_LEAVE(_task, ERROR_KO);
	break;
      case TASK_BEHAVIOUR_INTERACTIVE:
	if (prior < TASK_LPRIORITY_INTERACTIVE ||
	    prior > TASK_HPRIORITY_INTERACTIVE)
	  TASK_LEAVE(_task, ERROR_KO);
	break;
      case TASK_BEHAVIOUR_TIMESHARING:
	if (prior < TASK_LPRIORITY_TIMESHARING ||
	    prior > TASK_HPRIORITY_TIMESHARING)
	  TASK_LEAVE(_task, ERROR_KO);
	break;
      case TASK_BEHAVIOUR_BACKGROUND:
	if (prior < TASK_LPRIORITY_BACKGROUND ||
	    prior > TASK_HPRIORITY_BACKGROUND)
	  TASK_LEAVE(_task, ERROR_KO);
	break;
      default:
	TASK_LEAVE(_task, ERROR_KO);
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
	TASK_LEAVE(_task, ERROR_KO);
    }
  else
    o.cpu = _init->bsp;

  o.state = TASK_STATE_STOP;

  /*
   * 3)
   */

  if (id_reserve(&_task->id, &o.id) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  /*
   * 4)
   */

  if (_kernel->task != ID_UNUSED)
    {
      if (task_current(&o.parent) != ERROR_OK)
	TASK_LEAVE(_task, ERROR_KO);

      if (task_get(o.parent, &parent) != ERROR_OK)
	TASK_LEAVE(_task, ERROR_KO);

      if (set_add(parent->children, &o) != ERROR_OK)
	TASK_LEAVE(_task, ERROR_KO);
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
    {
      id_release(&_task->id, o.id);

      TASK_LEAVE(_task, ERROR_KO);
    }

  /*
   * 6)
   */

  if (set_reserve(array, SET_OPTION_ALLOC, TASK_WAITS_INITSZ,
		  sizeof (o_wait), &o.waits) != ERROR_OK)
    {
      id_release(&_task->id, o.id);

      set_release(o.threads);

      TASK_LEAVE(_task, ERROR_KO);
    }

  /*
   * 7)
   */

  if (set_reserve(ll, SET_OPTION_SORT | SET_OPTION_ALLOC,
		  sizeof (i_task), &o.children) != ERROR_OK)
    {
      id_release(&_task->id, o.id);

      set_release(o.threads);
      set_release(o.waits);

      TASK_LEAVE(_task, ERROR_KO);
    }

  /*
   * 8)
   */

  if (set_reserve(bpt, SET_OPTION_SORT | SET_OPTION_ALLOC,
		  sizeof (o_message_type),
		  MESSAGE_BPT_NODESZ, &o.messages) != ERROR_OK)
  {
      id_release(&_task->id, o.id);

      set_release(o.threads);
      set_release(o.waits);
      set_release(o.children);

      TASK_LEAVE(_task, ERROR_KO);
  }

  /*
   * 9)
   */

  if (set_add(_task->tasks, &o) != ERROR_OK)
    {
      id_release(&_task->id, o.id);

      set_release(o.messages);
      set_release(o.threads);
      set_release(o.waits);
      set_release(o.children);

      TASK_LEAVE(_task, ERROR_KO);
    }

  /*
   * 10)
   */

  if (machine_call(task, task_reserve, class,
		   behav, prior, id) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  /*
   * 11)
   */

  if (o.id != _kernel->task)
    {
      if (message_register(o.id, MESSAGE_TYPE_INTERFACE,
			   sizeof (o_syscall)) != ERROR_OK)
	{
	  message_flush(o.id);

	  id_release(&_task->id, o.id);

	  set_release(o.messages);
	  set_release(o.threads);
	  set_release(o.waits);
	  set_release(o.children);

	  TASK_LEAVE(_task, ERROR_KO);
	}

      if (message_register(o.id, MESSAGE_TYPE_EVENT,
			   sizeof (o_event_message)) != ERROR_OK)
	{
	  message_flush(o.id);

	  id_release(&_task->id, o.id);

	  set_release(o.messages);
	  set_release(o.threads);
	  set_release(o.waits);
	  set_release(o.children);

	  TASK_LEAVE(_task, ERROR_KO);
	}

      if (message_register(o.id, MESSAGE_TYPE_TIMER,
			   sizeof (o_timer_message)) != ERROR_OK)

	{
	  message_flush(o.id);

	  id_release(&_task->id, o.id);

	  set_release(o.messages);
	  set_release(o.threads);
	  set_release(o.waits);
	  set_release(o.children);

	  TASK_LEAVE(_task, ERROR_KO);
	}
    }

  TASK_LEAVE(_task, ERROR_OK);
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

  TASK_ENTER(_task);

  /*
   * 1)
   */

  if (machine_call(task, task_release, id) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  /*
   * 2)
   */

  if (task_get(id, &o) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  /*
   * 3)
   */

  if (id_release(&_task->id, o->id) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  /*
   * 4)
   */

  if (o->as != ID_UNUSED && id != _kernel->task)
    {
      if (as_release(o->as) != ERROR_OK)
	TASK_LEAVE(_task, ERROR_KO);
    }

  /*
   * 5)
   */

  if (thread_flush(id) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  if (set_release(o->threads) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  /*
   * 5)
   */

  if (set_release(o->waits) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  /*
   * 6)
   */

  if (message_flush(id) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  if (set_release(o->messages) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  /*
   * 7)
   */

  if (set_remove(_task->tasks, o->id) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  TASK_LEAVE(_task, ERROR_OK);
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

  TASK_ENTER(_task);

  /*
   * 1)
   */

  if (task_get(id, &o) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  /*
   * 2)
   */

  switch (o->behaviour)
    {
      case TASK_BEHAVIOUR_KERNEL:
	if (prior < TASK_LPRIORITY_KERNEL ||
	    prior > TASK_HPRIORITY_KERNEL)
	  TASK_LEAVE(_task, ERROR_KO);
	break;
      case TASK_BEHAVIOUR_REALTIME:
	if (prior < TASK_LPRIORITY_REALTIME ||
	    prior > TASK_HPRIORITY_REALTIME)
	  TASK_LEAVE(_task, ERROR_KO);
	break;
      case TASK_BEHAVIOUR_INTERACTIVE:
	if (prior < TASK_LPRIORITY_INTERACTIVE ||
	    prior > TASK_HPRIORITY_INTERACTIVE)
	  TASK_LEAVE(_task, ERROR_KO);
	break;
      case TASK_BEHAVIOUR_TIMESHARING:
	if (prior < TASK_LPRIORITY_TIMESHARING ||
	    prior > TASK_HPRIORITY_TIMESHARING)
	  TASK_LEAVE(_task, ERROR_KO);
	break;
      case TASK_BEHAVIOUR_BACKGROUND:
	if (prior < TASK_LPRIORITY_BACKGROUND ||
	    prior > TASK_HPRIORITY_BACKGROUND)
	  TASK_LEAVE(_task, ERROR_KO);
	break;
      default:
	TASK_LEAVE(_task, ERROR_KO);
    }

  /*
   * 3)
   */

  o->priority = prior;

  /*
   * 4)
   */

  if (machine_call(task, task_priority, id, prior) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  /*
   * 5)
   */

  set_foreach(SET_OPTION_FORWARD, o->threads, &i, state)
    {
      i_thread*		th;

      if (set_object(o->threads, i, (void**)&th) != ERROR_OK)
	{
	  module_call(console, console_message,
		      '!', "task: cannot find the object "
		      "corresponding to its identifier\n");

	  TASK_LEAVE(_task, ERROR_KO);
	}

      if (scheduler_update(*th) != ERROR_OK)
	TASK_LEAVE(_task, ERROR_KO);
    }

  TASK_LEAVE(_task, ERROR_OK);
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

t_error			task_state(i_task			id,
				   t_state			state)
{
  o_task*		o;
  t_iterator		i;
  t_state		st;
  t_state		wakeup;

  TASK_ENTER(_task);

  /*
   *
   */

  switch (state)
    {
      case TASK_STATE_RUN:
        wakeup = WAIT_START;
        break;
      case TASK_STATE_STOP:
        wakeup = WAIT_STOP;
        break;
      case TASK_STATE_ZOMBIE:
        wakeup = WAIT_DEATH;
        break;
      case TASK_STATE_BLOCK:
	wakeup = 0;
	break;
      default:
        TASK_LEAVE(_task, ERROR_KO);
    }

  /*
   * 1)
   */

  if (task_get(id, &o) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  /*
   * 2)
   */

  if (o->state == state)
    TASK_LEAVE(_task, ERROR_OK);

  /*
   * 3)
   */

  set_foreach(SET_OPTION_FORWARD, o->waits, &i, st)
    {
      o_wait*		w;

      if (set_object(o->waits, i, (void**)&w) != ERROR_OK)
	{
	  module_call(console, console_message,
		      '!', "task: cannot find the object "
		      "corresponding to its identifier\n");

	  TASK_LEAVE(_task, ERROR_KO);
	}

      if (w->options & wakeup)
	task_state(w->u.task, TASK_STATE_RUN);

      /* XXX remove */
    }

  /*
   * 4)
   */

  if (machine_call(task, task_state, id, state) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  /*
   * 5)
   */

  set_foreach(SET_OPTION_FORWARD, o->threads, &i, st)
    {
      i_thread*		th;

      if (set_object(o->threads, i, (void**)&th) != ERROR_OK)
	{
	  module_call(console, console_message,
		      '!', "task: cannot find the object "
		      "corresponding to its identifier\n");

	  TASK_LEAVE(_task, ERROR_KO);
	}

      // XXX remove toutes les threads qui run si on stoppe la task
      // XXX meme chose si on la run, il faut ajouter les threads qui sont
      //     en run

      if (state != TASK_STATE_BLOCK &&
	  thread_state(*th, state) != ERROR_OK)
	TASK_LEAVE(_task, ERROR_KO);
    }

  /*
   * XXX
   */

  o->state = state;

  TASK_LEAVE(_task, ERROR_OK);
}

/*
 * this function waits for state change.
 */

t_error			task_wait(i_task			id,
				  t_options			opts,
				  t_wait*			wait)
{
  o_task*		o;
  o_wait		w;

  TASK_ENTER(_task);

  assert(wait != NULL);

  /* XXX */

  if (opts & WAIT_ID)
    {
      /*
       * 1)
       */

      /*
       * a)
       */

      if (task_get(id, &o) != ERROR_OK)
	TASK_LEAVE(_task, ERROR_KO);

      /*
       * b)
       */

      if ((opts & WAIT_START) && o->state == TASK_STATE_RUN)
	TASK_LEAVE(_task, ERROR_OK);

      if ((opts & WAIT_STOP) && o->state == TASK_STATE_STOP)
	TASK_LEAVE(_task, ERROR_OK);

      if ((opts & WAIT_DEATH) && o->state == TASK_STATE_ZOMBIE)
	TASK_LEAVE(_task, ERROR_OK);

      /*
       * c)
       */

      if (!(opts & WAIT_NOHANG))
	{
	  /*
	   * d)
	   */

	  if (task_current(&w.u.task) != ERROR_OK)
	    TASK_LEAVE(_task, ERROR_KO);;

	  w.options = opts;

	  if (set_add(o->waits, &w) != ERROR_OK)
	    TASK_LEAVE(_task, ERROR_KO);

	  /*
	   * e)
	   */

	  if (task_state(w.u.task, TASK_STATE_STOP) != ERROR_OK)
	    TASK_LEAVE(_task, ERROR_KO);
	}
    }
  else
    {
      /*
       * 2)
       */

    }

  /*
   * 3)
   */

  if (wait != NULL)
    {

    }

  /*
   * 4)
   * XXX ou mettre ca !?
   */

  if (machine_call(task, task_wait, id, opts, wait) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  TASK_LEAVE(_task, ERROR_KO);
}

/*
 * this function gets a task object from the task set.
 */

t_error			task_get(i_task				id,
				 o_task**			o)
{
  TASK_ENTER(_task);

  assert(o != NULL);

  if (set_get(_task->tasks, id, (void**)o) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  TASK_LEAVE(_task, ERROR_OK);
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
  i_segment		segments[INIT_SEGMENTS];
  i_region		useless;
  o_segment*		segment;
  o_region*		region;
  t_uint32		i;

  /*
   * 1)
   */

  if ((_task = malloc(sizeof(m_task))) == NULL)
    {
      module_call(console, console_message,
		  '!', "task: cannot allocate memory for the task manager "
		  "structure\n");

      return (ERROR_KO);
    }

  memset(_task, 0x0, sizeof(m_task));

  /*
   * 2)
   */

  if (id_build(&_task->id) != ERROR_OK)
    {
      module_call(console, console_message,
		  '!', "task: unable to initialize the identifier object\n");

      return (ERROR_KO);
    }

  /*
   * 3)
   */

  if (set_reserve(ll, SET_OPTION_ALLOC | SET_OPTION_SORT,
		  sizeof(o_task), &_task->tasks) != ERROR_OK)
    {
      module_call(console, console_message,
		  '!', "task: unable to reserve the task set\n");

      return (ERROR_KO);
    }

  /*
   * 4)
   */

  if (task_reserve(TASK_CLASS_KERNEL, TASK_BEHAVIOUR_KERNEL,
		   TASK_PRIORITY_KERNEL, &_kernel->task) != ERROR_OK)
    {
      module_call(console, console_message,
		  '!', "task: unable to reserve the kernel task\n");

      return (ERROR_KO);
    }

  if (as_reserve(_kernel->task, &_kernel->as) != ERROR_OK)
    {
      module_call(console, console_message,
		  '!', "task: unable to reserve the kernel address space\n");

      return (ERROR_KO);
    }

  if (task_state(_kernel->task, TASK_STATE_RUN) != ERROR_OK)
    {
      module_call(console, console_message,
		  '!', "task: unable to set the task as running\n");

      return (ERROR_KO);
    }

  /*
   * 5)
   */

  for (i = 0; i < _init->nsegments; i++)
    {
      if ((segment = malloc(sizeof(o_segment))) == NULL)
	return (ERROR_KO);

      segment->type = SEGMENT_TYPE_MEMORY;
      segment->address = _init->segments[i].address;
      segment->size = _init->segments[i].size;
      segment->permissions = _init->segments[i].permissions;

      assert(segment->size != 0);

      if (segment_inject(_kernel->as, segment, &segments[i]) != ERROR_OK)
	{
	  module_call(console, console_message,
		      '!', "segment: cannot add a pre-reserved segment in "
		      "the segment set\n");

	  return (ERROR_KO);
	}

      if (_init->scode == _init->segments[i].address)
	_system = segments[i];
    }

  /*
   * 6)
   */

  for (i = 0; i < _init->nregions; i++)
    {
      if ((region = malloc(sizeof(o_region))) == NULL)
	return (ERROR_KO);

      region->segment = segments[_init->regions[i].segment];
      region->address = _init->regions[i].address;
      region->offset = _init->regions[i].offset;
      region->size = _init->regions[i].size;
      region->options = _init->regions[i].options;

      assert(region->size != 0);

      if (region_inject(_kernel->as, region, &useless) != ERROR_OK)
	{
	  module_call(console, console_message,
		      '!', "region: cannot map a region to a pre-reserved "
		      "region\n");

	  return (ERROR_KO);
	}
    }

  /*
   * 7)
   */

  if (machine_call(task, task_initialize) != ERROR_OK)
    return (ERROR_KO);

  return (ERROR_OK);
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
    return (ERROR_KO);

  /*
   * 2)
   */

  while (set_head(_task->tasks, &i) == ERROR_OK)
    {
      if (set_object(_task->tasks, i, (void**)&data) != ERROR_OK)
	{
	  module_call(console, console_message,
		      '!', "task: cannot find the task object "
		      "corresponding to its identifier\n");

	  return (ERROR_KO);
	}

      if (task_release(*data) != ERROR_OK)
	return (ERROR_KO);
    }

  if (set_release(_task->tasks) != ERROR_OK)
    {
      module_call(console, console_message,
		  '!', "task: unable to release the task set\n");

      return (ERROR_KO);
    }

  /*
   * 3)
   */

  if (id_destroy(&_task->id) != ERROR_OK)
    {
      module_call(console, console_message,
		  '!', "task: unable to destroy the identifier object\n");

      return (ERROR_KO);
    }

  /*
   * 4)
   */

  free(_task);

  return (ERROR_OK);
}
