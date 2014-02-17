/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/task/task.c
 *
 * created       julien quintard   [fri jun 22 02:25:26 2007]
 * updated       julien quintard   [thu apr  7 20:59:38 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the task manager provides functionalities for reserving, releasing,
 * waiting for etc. tasks.
 *
 * a task is a passive entity in the way that it is not actually scheduled.
 * however, a task contains several threads which can be scheduled and
 * an address space which describes the task's and its threads' useable
 * memory.
 *
 * every task is referenced by a unique task identifier. besides, every
 * task is characterised by a class, a behaviour and a priority.
 *
 * note that every task is assigned to a CPU. this way, the task's threads
 * can only be scheduled on this CPU.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * include the machine-specific definitions required by the core.
 */

machine_include(task);

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the kernel manager.
 */

extern m_kernel		_kernel;

/*
 * the init structure provided by the boot loader.
 */

extern s_init*		_init;

/*
 * the identifier of the pre-reserved segment containing the 'system'
 * server's code.
 */

extern i_segment	_system;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the task manager.
 */

m_task			_task;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function shows a task's attributes.
 *
 * steps:
 *
 * 1) retrieve the task object.
 * 2) build the state string.
 * 3) build the class string.
 * 4) build the behaviour string.
 * 5) display general information on the task object.
 * 6) display the set of children tasks.
 * 7) display the set of threads waiting for this task to change state.
 * 8) display the tasks's set of threads.
 * 9) call the machine.
 */

t_status		task_show(i_task			id,
				  mt_margin			margin)
{
  o_task*		o;
  char*			state;
  char*			class;
  char*			behaviour;
  t_setsz		size;
  s_iterator		i;
  t_state		s;

  /*
   * 1)
   */

  if (task_get(id, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  switch (o->state)
    {
      case TASK_STATE_START:
	{
	  state = "start";

	  break;
	}
      case TASK_STATE_STOP:
	{
	  state = "stop";

	  break;
	}
      case TASK_STATE_BLOCK:
	{
	  state = "block";

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

  /*
   * 3)
   */

  switch (o->class)
    {
    case TASK_CLASS_KERNEL:
      {
	class = "kernel";

	break;
      }
    case TASK_CLASS_DRIVER:
      {
	class = "driver";

	break;
      }
    case TASK_CLASS_SERVICE:
      {
	class = "service";

	break;
      }
    case TASK_CLASS_GUEST:
      {
	class = "guest";

	break;
      }
    default:
      CORE_ESCAPE("unknown task class '%u'",
		  o->class);
    }

  /*
   * 4)
   */

  switch (o->behaviour)
    {
    case TASK_BEHAVIOUR_KERNEL:
      {
	behaviour = "kernel";

	break;
      }
    case TASK_BEHAVIOUR_REALTIME:
      {
	behaviour = "realtime";

	break;
      }
    case TASK_BEHAVIOUR_INTERACTIVE:
      {
	behaviour = "interactive";

	break;
      }
    case TASK_BEHAVIOUR_TIMESHARING:
      {
	behaviour = "timesharing";

	break;
      }
    case TASK_BEHAVIOUR_BACKGROUND:
      {
	behaviour = "background";

	break;
      }
    default:
      CORE_ESCAPE("unknown task behaviour '%u'",
		  o->behaviour);
    }

  /*
   * 5)
   */

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "task:\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin));

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "  core: id(%qd) cpu(%qd) class(%s) behaviour(%s) priority(%u) "
	      "state(%s) as(%qd) threads(%qd) parent(%qd) children(%qd) "
	      "waits(%qd) value(%d) timer(%qd)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      o->id,
	      o->cpu,
	      class,
	      behaviour,
	      o->priority,
	      state,
	      o->as,
	      o->threads,
	      o->parent,
	      o->children,
	      o->waits,
	      o->value,
	      o->timer);

  /*
   * 6)
   */

  if (set_size(o->children, &size) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the size of the set of children");

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "    children: id(%qd) size(%qd)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      o->children,
	      size);

  set_foreach(SET_OPTION_FORWARD, o->children, &i, s)
    {
      i_task*		id;

      if (set_object(o->children, i, (void**)&id) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the task identifier");

      module_call(console, message,
		  '#',
		  MODULE_CONSOLE_MARGIN_FORMAT
		  "      task: id(%qd)\n",
		  MODULE_CONSOLE_MARGIN_VALUE(margin),
		  *id);
    }

  /*
   * 7)
   */

  if (set_size(o->waits, &size) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the size of the set of waiting threads");

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "    waits: id(%qd) size(%qd)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      o->waits,
	      size);

  set_foreach(SET_OPTION_FORWARD, o->waits, &i, s)
    {
      i_thread*		id;

      if (set_object(o->waits, i, (void**)&id) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the thread identifier");

      module_call(console, message,
		  '#',
		  MODULE_CONSOLE_MARGIN_FORMAT
		  "      thread: id(%qd)\n",
		  MODULE_CONSOLE_MARGIN_VALUE(margin),
		  *id);
    }

  /*
   * 8)
   */

  if (set_size(o->threads, &size) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the size of the set of threads");

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "    threads: id(%qd) size(%qd)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      o->threads,
	      size);

  set_foreach(SET_OPTION_FORWARD, o->threads, &i, s)
    {
      i_thread*		id;

      if (set_object(o->threads, i, (void**)&id) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the thread identifier");

      module_call(console, message,
		  '#',
		  MODULE_CONSOLE_MARGIN_FORMAT
		  "      thread: id(%qd)\n",
		  MODULE_CONSOLE_MARGIN_VALUE(margin),
		  *id);
    }

  /*
   * 9)
   */

  if (machine_call(task, show, id, margin) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function dumps the task manager.
 *
 * steps:
 *
 * 1) display general informatin on the manager.
 * 2) show the identifier object.
 * 3) retrieve the size of the set of tasks.
 * 4) go through the manager's tasks and show them.
 * 5) retrieve the size of the set of dead tasks.
 * 6) go through the set of dead tasks and display their identifiers.
 * 7) call the machine.
 */

t_status		task_dump(void)
{
  t_setsz		size;
  s_iterator		i;
  t_state		s;

  /*
   * 1)
   */

  module_call(console, message,
	      '#', "task manager:\n");

  module_call(console, message,
	      '#', "  core: tasks(%qd)\n",
	      _task.tasks);

  /*
   * 2)
   */

  if (id_show(&_task.id,
	      2 * MODULE_CONSOLE_MARGIN_SHIFT) != STATUS_OK)
    CORE_ESCAPE("unable to show the identifier object");

 /*
   * 3)
   */

  if (set_size(_task.tasks, &size) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the size of the set of tasks");

  /*
   * 4)
   */

  module_call(console, message,
	      '#', "    tasks: id(%qd) size(%qd)\n",
	      _task.tasks,
	      size);

  set_foreach(SET_OPTION_FORWARD, _task.tasks, &i, s)
    {
      o_task*		o;

      if (set_object(_task.tasks, i, (void**)&o) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the task object");

      if (task_show(o->id,
		    3 * MODULE_CONSOLE_MARGIN_SHIFT) != STATUS_OK)
	CORE_ESCAPE("unable to show the task");
    }

 /*
   * 5)
   */

  if (set_size(_task.morgue.field, &size) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the size of the set of dead tasks");

  /*
   * 6)
   */

  module_call(console, message,
	      '#', "    morgue: field(%qd) timer(%qd)\n",
	      _task.morgue.field,
	      _task.morgue.timer);

  module_call(console, message,
	      '#', "      field: id(%qd) size(%qd)\n",
	      _task.morgue.field,
	      size);

  set_foreach(SET_OPTION_FORWARD, _task.morgue.field, &i, s)
    {
      i_task*		id;

      if (set_object(_task.tasks, i, (void**)&id) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the task object");

      module_call(console, message,
		  '#', "        task: id(%qd)\n",
		  *id);
    }

  /*
   * 7)
   */

  if (machine_call(task, dump) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function reserves a task object.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) allocate and initialize the object.
 * 2) reserve an identifier for the task object.
 * 3) fill the parent task field and ask the current task as being of child
 *    of the parent. note that if the task being reserved is the kernel one,
 *    ignore this step.
 * 4) select the CPU on which the task will be scheduled.
 * 5) reserve the set of threads.
 * 6) reserve the set of waiting threads.
 * 7) reserve the set of children tasks.
 * 8) reserve the set of messages.
 * 9) add the task to the manager's set of tasks.
 * 10) return the identifier of the task.
 * 11) call the machine.
 */

t_status		task_reserve(t_class			class,
				     t_behaviour		behav,
				     t_priority			prior,
				     i_task*			id)
{
  o_task		o;

  /*
   * 0)
   */

  if (id == NULL)
    CORE_ESCAPE("the 'id' argument is null");

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
   * 1)
   */

  memset(&o, 0x0, sizeof (o_task));

  o.class = class;
  o.behaviour = behav;
  o.priority = prior;
  o.as = ID_UNUSED;
  o.state = TASK_STATE_STOP;
  o.value = WAIT_VALUE_UNKNOWN;
  o.timer = ID_UNUSED;

  /*
   * 2)
   */

  if (id_reserve(&_task.id, &o.id) != STATUS_OK)
    CORE_ESCAPE("unable to reserve an identifier for the task");

  /*
   * 3)
   */

  if (_kernel.task != ID_UNUSED)
    {
      o_task*		parent;

      if (task_current(&o.parent) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the currently running task");

      if (task_get(o.parent, &parent) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the task object");

      if (set_add(parent->children, &o.id) != STATUS_OK)
	CORE_ESCAPE("unable to add the new task to the list of its parent's "
		    "children tasks");
    }
  else
    o.parent = ID_UNUSED;

  /*
   * 4)
   */

  if (cpu_select(&o.cpu) != STATUS_OK)
    CORE_ESCAPE("unable to select a CPU on which to place the task");

  /*
   * 5)
   */

  if (set_reserve(array,
		  SET_OPTION_SORT | SET_OPTION_ALLOCATE,
		  TASK_THREADS_INITSZ,
		  sizeof (i_thread),
		  &o.threads) != STATUS_OK)
    CORE_ESCAPE("unable to reserve a set of threads");

  /*
   * 6)
   */

  if (set_reserve(array,
		  SET_OPTION_ALLOCATE,
		  TASK_WAITS_INITSZ,
		  sizeof (i_thread),
		  &o.waits) != STATUS_OK)
    CORE_ESCAPE("unable to reserve a set for the waiting tasks/threads");

  /*
   * 7)
   */

  if (set_reserve(ll,
		  SET_OPTION_SORT | SET_OPTION_ALLOCATE,
		  sizeof (i_task),
		  &o.children) != STATUS_OK)
    CORE_ESCAPE("unable to reserve a set for the children tasks");

  /*
   * 8)
   */

  if (set_reserve(ll,
		  SET_OPTION_SORT | SET_OPTION_ALLOCATE,
		  sizeof (o_message_type),
		  &o.messages) != STATUS_OK)
    CORE_ESCAPE("unable to reserve a set for the messages");

  /*
   * 9)
   */

  if (set_add(_task.tasks, &o) != STATUS_OK)
    CORE_ESCAPE("unable to add the object to the set of tasks");

  /*
   * 10)
   */

  *id = o.id;

  /*
   * 11)
   */

  if (machine_call(task, reserve, class, behav, prior, id) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function releases a task.
 *
 * steps:
 *
 * 1) call the machine.
 * 2) retrieve the task object.
 * 3) release the task's timer, if necessary.
 * 4) flush the task's threads and release the set of threads.
 * 5) if the task possesses an address space and this is not the kernel's,
 *    release it.
 * 6) release the set of children.
 * 7) release the set of waiting threads.
 * 8) flush and release the set of messages.
 * 9) release the task identifier.
 * 10) remove the task from the manager's set of tasks.
 */

t_status		task_release(i_task			id)
{
  o_task*		o;

  /*
   * 1)
   */

  if (machine_call(task, release, id) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  if (task_get(id, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 3)
   */

  if (o->timer != ID_UNUSED)
    {
      if (timer_release(o->timer) != STATUS_OK)
	CORE_ESCAPE("unable to release the timer");
    }

  /*
   * 4)
   */

  if (thread_flush(id) != STATUS_OK)
    CORE_ESCAPE("unable to flush the task's threads");

  if (set_release(o->threads) != STATUS_OK)
    CORE_ESCAPE("unable to release the set of threads");

  /*
   * 5)
   */

  if ((o->as != ID_UNUSED) &&
      (o->as != _kernel.as))
    {
      if (as_release(o->as) != STATUS_OK)
	CORE_ESCAPE("unable to release the task's address space");
    }

  /*
   * 6)
   */

  if (set_release(o->children) != STATUS_OK)
    CORE_ESCAPE("unable to release the set of children tasks");

  /*
   * 7)
   */

  if (set_release(o->waits) != STATUS_OK)
    CORE_ESCAPE("unable to release the set of waiting tasks/threads");

  /*
   * 8)
   */

  if (message_flush(o->id) != STATUS_OK)
    CORE_ESCAPE("unable to flush the messages");

  if (set_release(o->messages) != STATUS_OK)
    CORE_ESCAPE("unable to release the set of messages");

  /*
   * 9)
   */

  if (id_release(&_task.id, o->id) != STATUS_OK)
    CORE_ESCAPE("unable to release the task identifier");

  /*
   * 10)
   */

  if (set_remove(_task.tasks, o->id) != STATUS_OK)
    CORE_ESCAPE("unable to remove the object from the set of tasks");

  CORE_LEAVE();
}

/*
 * this function updates the task's priority.
 *
 * steps:
 *
 * 1) retrieve the task object.
 * 2) check that the priority lies in the priority range associated with
 *    this task i.e depending on the task's behaviour.
 * 3) update the task's priority.
 * 4) go through the task's threads.
 *   a) retrieve the thread identifier.
 *   b) retrieve the thread object.
 *   c) request the scheduler to update the thread. indeed, since the task
 *      priority has changed, the scheduling priority may have changed as
 *      well. if it has, the thread may have to be moved to another queue
 *      for instance.
 * 5) call the machine.
 */

t_status		task_priority(i_task			id,
				      t_priority		prior)
{
  o_task*		o;
  s_iterator		i;
  t_state		state;

  /*
   * 1)
   */

  if (task_get(id, &o) != STATUS_OK)
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
	CORE_ESCAPE("unknown behaviour '%u'",
		    o->behaviour);
    }

  /*
   * 3)
   */

  o->priority = prior;

  /*
   * 4)
   */

  set_foreach(SET_OPTION_FORWARD, o->threads, &i, state)
    {
      i_thread*		th;
      o_thread*		oth;

      /*
       * a)
       */

      if (set_object(o->threads, i, (void**)&th) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the thread identifier");

      /*
       * b)
       */

      if (thread_get(*th, &oth) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the thread object");

      /*
       * c)
       */

      if ((o->state == TASK_STATE_START) &&
	  (oth->state == THREAD_STATE_START))
	{
	  if (scheduler_update(*th) != STATUS_OK)
	    CORE_ESCAPE("unable to update the thread scheduling state");
	}
    }

  /*
   * 5)
   */

  if (machine_call(task, priority, id, prior) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function starts the task.
 *
 * steps:
 *
 * 1) retrieve the task object.
 * 2) return an error if the thread is already running.
 * 3) set the task's state as started.
 * 4) go through the waiting threads.
 *   a) retrieve the thread identifier.
 *   b) retrieve the thread object.
 *   c) if the thread is waiting for the task to be started.
 *     i) set the wake-up information: the task has started.
 *     ii) wake up the waiting thread.
 *     iii) delete the thread from the waiting set.
 * 5) go through the threads
 *   a) retrieve the thread identifier.
 *   b) retrieve the thread object.
 *   c) if the thread has been started but is not scheduled because the
 *      task was not started---i.e it is now the case---add the thread
 *      to the scheduler.
 * 6) call the machine.
 */

t_status		task_start(i_task			id)
{
  o_task*		object;
  s_iterator		i;
  t_state		s;

  /*
   * 1)
   */

  if (task_get(id, &object) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  if (object->state == TASK_STATE_START)
    CORE_ESCAPE("a task cannot be started twice");

  /*
   * 3)
   */

  object->state = TASK_STATE_START;

  /*
   * 4)
   */

 try:
  set_foreach(SET_OPTION_FORWARD, object->waits, &i, s)
    {
      i_thread*		id;
      o_thread*		o;

      /*
       * a)
       */

      if (set_object(object->waits, i, (void**)&id) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the waiting thread's identifier");

      /*
       * b)
       */

      if (thread_get(*id, &o) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the waiting thread");

      /*
       * c)
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

	  if (thread_start(o->id) != STATUS_OK)
	    CORE_ESCAPE("unable to start the waiting thread");

	  /*
	   * iii)
	   */

	  if (set_delete(object->waits, i) != STATUS_OK)
	    CORE_ESCAPE("unable to delete the thread from the waiting list");

	  goto try;
	}
    }

  /*
   * 5)
   */

  set_foreach(SET_OPTION_FORWARD, object->threads, &i, s)
    {
      i_thread*		id;
      o_thread*		o;

      /*
       * a)
       */

      if (set_object(object->threads, i, (void**)&id) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the thread identifier");

      /*
       * b)
       */

      if (thread_get(*id, &o) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the thread object");

      /*
       * c)
       */

      if (o->state == THREAD_STATE_START)
	{
	  if (scheduler_add(o->id) != STATUS_OK)
	    CORE_ESCAPE("unable to add the thread to the scheduler");
	}
    }

  /*
   * 6)
   */

  if (machine_call(task, start, id) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function stops a task.
 *
 * steps:
 *
 * 1) retrieve the task object.
 * 2) return an error if the thread is already stopped.
 * 3) save the current task's state.
 * 4) set the task's state as stopped.
 * 5) go through the waiting threads.
 *   a) retrieve the thread identifier.
 *   b) retrieve the thread object.
 *   c) if the thread is waiting for the task to be stopped.
 *     i) set the wake-up information: the task has stopped.
 *     ii) wake up the waiting thread.
 *     iii) delete the thread from the waiting set.
 * 6) go through the threads
 *   a) retrieve the thread identifier.
 *   b) retrieve the thread object.
 *   c) if the thread is running and the task was, stop it by removing
 *      the thread from the scheduler.
 * 7) call the machine.
 */

t_status		task_stop(i_task			id)
{
  o_task*		object;
  t_state		state;
  s_iterator		i;
  t_state		s;

  /*
   * 1)
   */

  if (task_get(id, &object) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  if (object->state == TASK_STATE_STOP)
    CORE_ESCAPE("a task cannot be stopped twice");

  /*
   * 3)
   */

  state = object->state;

  /*
   * 4)
   */

  object->state = TASK_STATE_STOP;

  /*
   * 5)
   */

 try:
  set_foreach(SET_OPTION_FORWARD, object->waits, &i, s)
    {
      i_thread*		id;
      o_thread*		o;

      /*
       * a)
       */

      if (set_object(object->waits, i, (void**)&id) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the waiting thread's identifier");

      /*
       * b)
       */

      if (thread_get(*id, &o) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the waiting thread");

      /*
       * c)
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

	  if (thread_start(o->id) != STATUS_OK)
	    CORE_ESCAPE("unable to start the waiting thread");

	  /*
	   * iii)
	   */

	  if (set_delete(object->waits, i) != STATUS_OK)
	    CORE_ESCAPE("unable to delete the thread from the waiting list");

	  goto try;
	}
    }

  /*
   * 6)
   */

  set_foreach(SET_OPTION_FORWARD, object->threads, &i, s)
    {
      i_thread*		id;
      o_thread*		o;

      /*
       * a)
       */

      if (set_object(object->threads, i, (void**)&id) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the thread identifier");

      /*
       * b)
       */

      if (thread_get(*id, &o) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the thread object");

      /*
       * c)
       */

      if ((state == TASK_STATE_START) &&
	  (o->state == THREAD_STATE_START))
	{
	  if (scheduler_remove(o->id) != STATUS_OK)
	    CORE_ESCAPE("unable to add the thread to the scheduler");
	}
    }

  /*
   * 7)
   */

  if (machine_call(task, stop, id) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function block the task waiting for an event to wake it up.
 *
 * steps:
 *
 * 1) retrieve the task object.
 * 2) return an error if the thread is already blocked.
 * 3) save the current task's state.
 * 4) set the task's state as blocked.
 * 5) go through the threads
 *   a) retrieve the thread identifier.
 *   b) retrieve the thread object.
 *   c) if the thread is running and the task was running, stop it
 *      by removing the thread from the scheduler.
 * 6) call the machine.
 */

t_status		task_block(i_task			id)
{
  o_task*		object;
  t_state		state;
  s_iterator		i;
  t_state		s;

  /*
   * 1)
   */

  if (task_get(id, &object) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  if (object->state == TASK_STATE_BLOCK)
    CORE_ESCAPE("a task cannot be blocked twice");

  /*
   * 3)
   */

  state = object->state;

  /*
   * 4)
   */

  object->state = TASK_STATE_BLOCK;

  /*
   * 5)
   */

  set_foreach(SET_OPTION_FORWARD, object->threads, &i, s)
    {
      i_thread*		id;
      o_thread*		o;

      /*
       * a)
       */

      if (set_object(object->threads, i, (void**)&id) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the thread identifier");

      /*
       * b)
       */

      if (thread_get(*id, &o) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the thread object");

      /*
       * c)
       */

      if ((state == TASK_STATE_START) &&
	  (o->state == THREAD_STATE_START))
	{
	  if (scheduler_remove(o->id) != STATUS_OK)
	    CORE_ESCAPE("unable to remove the thread from the scheduler");
	}
    }

  /*
   * 6)
   */

  if (machine_call(task, block, id) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function exits a task, provided an exit value.
 *
 * the idea behind this function is to (i) exit the task's threads (ii) wake
 * up the threads waiting for the task's death (iii) mark the task's
 * children as being orphans and, finally (iv) put the task to the morgue
 * if someone has taken notice of its death.
 *
 * note that the threads are exited before the task changes state. indeed,
 * assuming the task was running before exiting, the running threads would
 * not be removed from the scheduler if exited once the task has been marked
 * as dying.
 *
 * besides, a special case is made for the currently executing thread.
 * should this thread belong to the exiting task, it will not be exited
 * with the others. indeed, if this thread is exited, hence removed from the
 * scheduler, the execution will be yielded and the task exiting process
 * will hang in this state forever. therefore, this special thread is exited
 * at the very end of the function, hence leading the execution to be yielded
 * without interferring with the current operation since having been completed
 * successfully.
 *
 * in addition, this function performs a quite unusual operation. the
 * morgue, in which the dead tasks are stored, relies on a timer.
 * unfortunately, the timer manager must be initialized after the task manager.
 * therefore the task manager could not reserve a timer in its initialization
 * phase. the timer reservation has therefore been put in this function such
 * that, if no task dies, the timer is never reserved and the morgue never
 * run.
 *
 * steps:
 *
 * ~) if required, reserve the timer associated with the morgue.
 * 1) retrieve the task object.
 * 2) if the task is already dying or dead, return an error.
 * 3) retrieve the currently running thread identifier.
 * 4) store the exit value in the task object.
 * 5) exit the task's threads.
 *   a) retrieve the thread identifier and object.
 *   b) perform the exit depending on the nature of the thread.
 *     A) if the thread to exit is currently executing...
 *       i) save this special thread identifier so that to exit it at the
 *          very end since its exit is very likely to lead to a yield.
 *     B) otherwise...
 *       i) if the thread is already dying or dead, needless to exit it as it
 *           already has.
 *       ii) exit the thread with the exit value provided through the
 *           task_exit().
 * 6) change the task's state as being dying i.e zombie.
 * 7) go through the waiting threads.
 *   a) retrieve the waiting thread identifier.
 *   b) retrieve the thread object.
 *   c) if the thread is waiting for the task's death.
 *     i) set its waiting result.
 *     ii) wake up the thread.
 *     iii) remove the thread from set of waiting threads.
 *     iv) mark the fact that at least one thread has taken notice of the
 *         task's death i.e the task can now be buried.
 *     v) since the set has been modified i.e iterators may no longer be
 *        consistent, re-start from the beginning.
 * 8) if at least one thread has taken notice of the death, bury the task.
 *   a) set the task's state to dead.
 *   b) add the task identifier to the morgue for actual burial.
 * 9) go through the task's children so as to make them orphans.
 *   a) retrieve the task identifier.
 *   b) retrieve the task object.
 *   c) re-initialize the parent field, hence making the task an orphan.
 * 10) call the machine.
 * 11) finally, if the currently executing thread has been detecting as
 *     belonging to the dying task, exit it.
 *   a) exit the thread. note however that since the task is in a zombie
 *      or dead state, the thread_exit() function will consider that the
 *      thread is not currently in the scheduler's data structures. therefore
 *      the thread will not be removed from the scheduler.
 *   b) since it should, perform the scheduler removal manually.
 */

t_status		task_exit(i_task			id,
				  t_value			value)
{
  o_task*		object;
  i_thread		current;
  i_thread		thread;
  s_iterator		i;
  t_state		s;
  t_boolean		w;

  /*
   * ~)
   */

  if (_task.morgue.timer == ID_UNUSED)
    {
      if (timer_reserve(TIMER_TYPE_FUNCTION,
			TIMER_ROUTINE(task_bury),
			TIMER_DATA(NULL),
			TASK_MORGUE_DELAY,
			TIMER_OPTION_REPEAT,
			&_task.morgue.timer) != STATUS_OK)
	MACHINE_ESCAPE("unable to reserve the timer");
    }

  /*
   * 1)
   */

  if (task_get(id, &object) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  if ((object->state == TASK_STATE_ZOMBIE) ||
      (object->state == TASK_STATE_DEAD))
    CORE_ESCAPE("a task cannot be exited twice");

  /*
   * 3)
   */

  if (thread_current(&current) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the currently executing thread");

  /*
   * 4)
   */

  object->value = value;

  /*
   * 5)
   */

  thread = ID_UNUSED;

  set_foreach(SET_OPTION_FORWARD, object->threads, &i, s)
    {
      i_thread*		id;
      o_thread*		o;

      /*
       * a)
       */

      if (set_object(object->threads, i, (void**)&id) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the thread identifier");

      if (thread_get(*id, &o) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the thread object");

      /*
       * b)
       */

      if (o->id == current)
	{
	  /*
	   * A)
	   */

	  /*
	   * i)
	   */

	  thread = o->id;
	}
      else
	{
	  /*
	   * B)
	   */

	  /*
	   * i)
	   */

	  if ((o->state == THREAD_STATE_ZOMBIE) ||
	      (o->state == THREAD_STATE_DEAD))
	    continue;

	  /*
	   * ii)
	   */

	  if (thread_exit(o->id, object->value) != STATUS_OK)
	    CORE_ESCAPE("unable to exit the thread");
	}
    }

  /*
   * 6)
   */

  object->state = TASK_STATE_ZOMBIE;

  /*
   * 7)
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

      if (set_object(object->waits, i, (void**)&id) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the waiting thread's identifier");

      /*
       * b)
       */

      if (thread_get(*id, &o) != STATUS_OK)
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

	  if (thread_start(o->id) != STATUS_OK)
	    CORE_ESCAPE("unable to start the waiting thread");

	  /*
	   * iii)
	   */

	  if (set_delete(object->waits, i) != STATUS_OK)
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
   * 8)
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

      if (set_add(_task.morgue.field, &object->id) != STATUS_OK)
	CORE_ESCAPE("unable to add the task to the morgue");
    }

  /*
   * 9)
   */

  set_foreach(SET_OPTION_FORWARD, object->children, &i, s)
    {
      i_task*		child;
      o_task*		o;

      /*
       * a)
       */

      if (set_object(object->children, i, (void**)&child) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the task identifier");

      /*
       * b)
       */

      if (task_get(*child, &o) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the task object");

      /*
       * c)
       */

      o->parent = ID_UNUSED;
    }

  /*
   * 10)
   */

  if (machine_call(task, exit, id, value) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 11)
   */

  if (thread != ID_UNUSED)
    {
      /*
       * a)
       */

      if (thread_exit(thread, object->value) != STATUS_OK)
	CORE_ESCAPE("unable to exit the currently running thread");

      /*
       * b)
       */

      if (scheduler_remove(thread) != STATUS_OK)
	CORE_ESCAPE("unable to remove the thread from the scheduler");
    }

  CORE_LEAVE();
}

/*
 * this function is triggered on a regular basis in order to bury the
 * dead tasks.
 *
 * steps:
 *
 * 1) go through the dead tasks.
 *   a) retrieve the task identifier and object.
 *   b) if the task is not dead, it should not be here: this is a fatal error!
 *   c) release the task.
 *   d) remove the task identifier from the morgue.
 */

void			task_bury(i_timer			timer,
				  t_vaddr			data)
{
  s_iterator		i;

  /*
   * 1)
   */

  while (set_head(_task.morgue.field, &i) == TRUE)
    {
      i_task*		task;
      o_task*		object;

      /*
       * a)
       */

      assert(set_object(_task.morgue.field, i, (void**)&task) == STATUS_OK);

      assert(task_get(*task, &object) == STATUS_OK);

      /*
       * b)
       */

      assert(object->state == TASK_STATE_DEAD);

      /*
       * c)
       */

      assert(task_release(object->id) == STATUS_OK);

      /*
       * d)
       */

      assert(set_delete(_task.morgue.field, i) == STATUS_OK);
    }
}

/*
 * this function provies the thread 'id' to wait for the target task to change
 * its state to 'state'.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the thread and target task objects.
 * 2) check if the thread willing to wait is running.
 * 3) if the thread is waiting for the task to start and the task is already
 *    running, fill and return the information.
 * 4) if the thread is waiting for the task to stop and the task is already
 *    stopped, fill and return the information.
 * 5) if the thread is waiting for the task to exit and the task is already
 *    dying, fill and return the information.
 *   a) fill the waiting thread's wait structure.
 *   b) set the task as being dead now that someone has taken notice
 *      of its death.
 *   c) add the task to the morgue.
 * 6) if the task is already dead, there is nothing to wait for, return an
 *    error as it is too late.
 * 7) set the thread's waiting state as the one provided. it is necessary
 *    to store such an information for later when the task will finally
 *    change its state. at that moment, the task will have to know what
 *    the thread is waiting for to decide wheter to wake him up or not.
 * 8) add the thread to the task's waiting list.
 * 9) stop the thread. the thread will be woken up by the task once it will
 *    have changed to the given state. therefore the next steps are executed
 *    only after the thread has been woken up.
 * 10) retrieve the information passed by the task on the event that led
 *    to the thread being woken up i.e the cause but also the value should
 *    the task have exited. note that these information are passed by the
 *    task through the thread's object 'wait' specific attribute.
 * 11) re-initialize the thread has being waiting for nothing.
 * 12) call the machine.
 */

t_status		task_wait(i_thread			id,
				  i_task			target,
				  t_state			state,
				  s_wait*			wait)
{
  o_thread*		object;
  o_task*		o;

  /*
   * 0)
   */

  if (wait == NULL)
    CORE_ESCAPE("the 'wait' argument is null");

  /*
   * 1)
   */

  if (thread_get(id, &object) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  if (task_get(target, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  /*
   * 2)
   */

  if (object->state != THREAD_STATE_START)
    CORE_ESCAPE("unable to make a non-running thread wait for a task");

  /*
   * 3)
   */

  if ((state & WAIT_STATE_START) &&
      (o->state == TASK_STATE_START))
    {
      wait->id.task = target;
      wait->state = state;
      wait->cause = WAIT_STATE_START;
      wait->value = WAIT_VALUE_UNKNOWN;

      CORE_LEAVE();
    }

  /*
   * 4)
   */

  if ((state & WAIT_STATE_STOP) &&
      (o->state == TASK_STATE_STOP))
    {
      wait->id.task = target;
      wait->state = state;
      wait->cause = WAIT_STATE_STOP;
      wait->value = WAIT_VALUE_UNKNOWN;

      CORE_LEAVE();
    }

  /*
   * 5)
   */

  if ((state & WAIT_STATE_DEATH) &&
      (o->state == TASK_STATE_ZOMBIE))
    {
      /*
       * a)
       */

      wait->id.task = target;
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

      if (set_add(_task.morgue.field, &o->id) != STATUS_OK)
	CORE_ESCAPE("unable to add the task to the morgue");

      CORE_LEAVE();
    }

  /*
   * 6)
   */

  if (o->state == TASK_STATE_DEAD)
    CORE_ESCAPE("unable to wait for a dead task");

  /*
   * 7)
   */

  object->wait.state = state;

  /*
   * 8)
   */

  if (set_add(o->waits, &id) != STATUS_OK)
    CORE_ESCAPE("unable to add the thread identifier to the waiting list");

  /*
   * 9)
   */

  if (thread_stop(id) != STATUS_OK)
    CORE_ESCAPE("unable to stop the task");

  /*
   * 10)
   */

  wait->id.task = target;
  wait->state = object->wait.state;
  wait->cause = object->wait.cause;
  wait->value = object->wait.value;

  /*
   * 11)
   */

  object->wait.state = WAIT_STATE_NONE;

  /*
   * 12)
   */

  if (machine_call(task, wait, id, target, state, wait) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function is called when a task must be woken up.
 *
 * steps:
 *
 * 1) retrieve the task object.
 * 2) re-initialize the task's timer.
 * 3) wake up the task.
 */

void			task_wakeup(i_timer			timer,
				    t_vaddr			data)
{
  i_task*		id = (i_task*)data;
  o_task*		o;

  /*
   * 1)
   */

  assert(task_get(*id, &o) == STATUS_OK);

  /*
   * 2)
   */

  o->timer = ID_UNUSED;

  /*
   * 3)
   */

  assert(task_start(o->id) == STATUS_OK);
}

/*
 * this function puts the given task to sleep. this operation implies
 * suspending the execution of all the task's threads.
 *
 * note that every task object embeds an available timer identifier which,
 * in this case, is used to trigger the wakeup function.
 *
 * another particularity comes from the fact that the id of the task to
 * wake up is passed as the data to the timer. note that since the object
 * is not supposed to be released before the timer is triggered, the
 * address of the identifier within the task object is actually passed.
 *
 * steps:
 *
 * 1) retrieve the task object.
 * 2) if the task is not running, it cannot be put to sleep.
 * 3) reserve a timer.
 * 4) call the machine.
 * 5) finally, block the task.
 */

t_status		task_sleep(i_task			id,
				   t_delay			milliseconds)
{
  o_task*		o;

  /*
   * 1)
   */

  if (task_get(id, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  if (o->state != TASK_STATE_START)
    CORE_ESCAPE("unable to put to sleep a non-running task");

  /*
   * 3)
   */

  if (timer_reserve(TIMER_TYPE_FUNCTION,
                    TIMER_ROUTINE(task_wakeup),
                    TIMER_DATA(&o->id),
                    milliseconds,
                    TIMER_OPTION_NONE,
                    &o->timer) != STATUS_OK)
    CORE_ESCAPE("unable to reserve a timer");

  /*
   * 4)
   */

  if (machine_call(task, sleep, id, milliseconds) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 5)
   */

  if (task_block(o->id) != STATUS_OK)
    CORE_ESCAPE("unable to block the task");

  CORE_LEAVE();
}

/*
 * this function returns the task identifier of the thread being executed
 * on the current CPU.
 *
 * steps:
 *
 * 0) verify the arguments
 * 1) retrieve the scheduler for the current CPU.
 * 2) retrieve the thread object being executed.
 * 3) return the task identifier of the thread.
 */

t_status		task_current(i_task*			task)
{
  o_scheduler*		scheduler;
  o_thread*		thread;

  /*
   * 0)
   */

  if (task == NULL)
    CORE_ESCAPE("the 'task' argument is null");

  /*
   * 1)
   */

  if (scheduler_current(&scheduler) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the current scheduler");

  /*
   * 2)
   */

  if (thread_get(scheduler->thread, &thread) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  /*
   * 3)
   */

  *task = thread->task;

  CORE_LEAVE();
}

/*
 * this function returns true if the task object exists.
 */

t_bool			task_exist(i_task			id)
{
  if (set_exist(_task.tasks, id) != TRUE)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function retrieve a task object given its identifier.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the object from the set of tasks.
 */

t_status		task_get(i_task				id,
				 o_task**			object)
{
  /*
   * 0)
   */

  if (object == NULL)
    CORE_ESCAPE("the 'object' argument is null");

  /*
   * 1)
   */

  if (set_get(_task.tasks, id, (void**)object) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the object from the set of tasks");

  CORE_LEAVE();
}

/*
 * this function initializes the task manager.
 *
 * note that this function takes care of reserving and initializing
 * the kernel task, address space and thread but also to inject the
 * pre-reserved segments and regions provided by the boot loader.
 *
 * finally, let us recall that the kernel will, later on, spawn the very
 * first server referred to as 'system'. this server is located somewhere
 * in a pre-allocated area of memory. this function also takes care to
 * identifies the segment which contains the server's code so that it can get
 * mapped and launched properly once the kernel is set up.
 *
 * steps:
 *
 * 1) display the message.
 * 2) initialize the task manager's structure.
 * 3) build the task manager's identifier object.
 * 4) reserve the set of tasks.
 * 5) reserve the set of dead tasks i.e the morgue.
 * 6) since the morgue timer cannot be reserved at the time, set the identifier
 *    as being unused and defer the reservation to the first task exiting i.e
 *    for more information, please refer to the task_exit() function.
 * 7) reserve the kernel task.
 * 8) reserve the kernel address space.
 * 9) start the kernel task.
 * 10) go through the pre-reserved segments.
 *   a) ignore zero-sized segments.
 *   b) allocate a segment object.
 *   c) fill the segment object's attributes.
 *   d) inject the segment in the kernel's address space.
 *   e) if the current segment corresponds to the one hosting the 'system'
 *      server's code, save the segment identifier in the '_system' variable.
 * 11) go through the pre-reserved regions.
 *   a) allocate a region object.
 *   b) fill the region object's attributes.
 *   c) inject the region in the kernel's address space.
 * 12) call the machine.
 */

t_status		task_initialize(void)
{
  i_segment		segments[_init->nsegments];
  i_region		useless;
  o_segment*		segment;
  o_region*		region;
  t_uint32		i;

  /*
   * 1)
   */

  module_call(console, message,
	      '+', "initializing the task manager\n");

  /*
   * 2)
   */

  memset(&_task, 0x0, sizeof (m_task));

  /*
   * 3)
   */

  if (id_build(&_task.id) != STATUS_OK)
    CORE_ESCAPE("unable to build the identifier object");

  /*
   * 4)
   */

  if (set_reserve(ll,
		  SET_OPTION_ALLOCATE | SET_OPTION_SORT,
		  sizeof (o_task),
		  &_task.tasks) != STATUS_OK)
    CORE_ESCAPE("unable to reserve the set of tasks");

  /*
   * 5)
   */

  if (set_reserve(ll,
		  SET_OPTION_ALLOCATE | SET_OPTION_SORT,
		  sizeof (i_task),
		  &_task.morgue.field) != STATUS_OK)
    CORE_ESCAPE("unable to reserve the set of dead tasks i.e the morgue");

  /*
   * 6)
   */

  _task.morgue.timer = ID_UNUSED;

  /*
   * 7)
   */

  if (task_reserve(TASK_CLASS_KERNEL,
		   TASK_BEHAVIOUR_KERNEL,
		   TASK_PRIORITY_KERNEL,
		   &_kernel.task) != STATUS_OK)
    CORE_ESCAPE("unable to reserve the kernel task");

  /*
   * 8)
   */

  if (as_reserve(_kernel.task,
		 &_kernel.as) != STATUS_OK)
    CORE_ESCAPE("unable to reserve the kernel address space");

  /*
   * 9)
   */

  if (task_start(_kernel.task) != STATUS_OK)
    CORE_ESCAPE("unable to start the kernel task");

  /*
   * 10)
   */

  for (i = 0; i < _init->nsegments; i++)
    {
      /*
       * a)
       */

      if (_init->segments[i].size == 0)
	continue;

      /*
       * b)
       */

      if ((segment = malloc(sizeof (o_segment))) == NULL)
	CORE_ESCAPE("unable to allocate memory for the segment object");

      /*
       * c)
       */

      segment->address = _init->segments[i].address;
      segment->size = _init->segments[i].size;
      segment->permissions = _init->segments[i].permissions;
      segment->options = _init->segments[i].options;

      /*
       * d)
       */

      if (segment_inject(_kernel.as, segment, &segments[i]) != STATUS_OK)
	CORE_ESCAPE("unable to inject the segment object pre-reserved "
		    "by the boot loader");

      /*
       * e)
       */

      if (_init->scode == _init->segments[i].address)
	_system = segments[i];
    }

  /*
   * 11)
   */

  for (i = 0; i < _init->nregions; i++)
    {
      /*
       * a)
       */

      if ((region = malloc(sizeof (o_region))) == NULL)
	CORE_ESCAPE("unable to allocate memory for the region object");

      /*
       * b)
       */

      region->segment = segments[_init->regions[i].segment];
      region->address = _init->regions[i].address;
      region->offset = _init->regions[i].offset;
      region->size = _init->regions[i].size;
      region->options = _init->regions[i].options;

      /*
       * c)
       */

      if (region_inject(_kernel.as, region, &useless) != STATUS_OK)
	CORE_ESCAPE("unable to inject the region object pre-reserved "
		    "by the boot loader");
    }

  /*
   * 12)
   */

  if (machine_call(task, initialize) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function cleans the task manager.
 *
 * steps:
 *
 * 1) display a message.
 * 2) call the machine
 * 3) go through the threads.
 *   a) retrieve the task identifier.
 *   b) release the task.
 * 4) release the morgue's timer, if necessary. for more information, pleae
 *    refer to task_initialize().
 * 5) release the morgue's set.
 * 6) release the set of tasks.
 * 7) release the identifier object.
 */

t_status		task_clean(void)
{
  s_iterator		i;
  t_state		s;

  /*
   * 1)
   */

  module_call(console, message,
	      '+', "cleaning the task manager\n");

  /*
   * 2)
   */

  if (machine_call(task, clean) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 3)
   */

  set_foreach(SET_OPTION_FORWARD, _task.tasks, &i, s)
    {
      i_task*		task;

      /*
       * a)
       */

      if (set_object(_task.tasks, i, (void**)&task) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the task identifier");

      /*
       * b)
       */

      if (task_release(*task) != STATUS_OK)
	CORE_ESCAPE("unable to release the task");
    }

  /*
   * 4)
   */

  if (_task.morgue.timer != ID_UNUSED)
    {
      if (timer_release(_task.morgue.timer) != STATUS_OK)
	CORE_ESCAPE("unable to release the timer");
    }

  /*
   * 5)
   */

  if (set_release(_task.morgue.field) != STATUS_OK)
    CORE_ESCAPE("unable to release the set of dead tasks");

  /*
   * 6)
   */

  if (set_release(_task.tasks) != STATUS_OK)
    CORE_ESCAPE("unable to release the set of tasks");

  /*
   * 7)
   */

  if (id_destroy(&_task.id) != STATUS_OK)
    CORE_ESCAPE("unable to destroy the identifier object");

  CORE_LEAVE();
}
