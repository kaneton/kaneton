/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/core/kaneton/task/task.c
 *
 * created       julien quintard   [sat dec 10 13:56:00 2005]
 * updated       matthieu bucchianeri   [mon feb 20 10:59:57 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * XXX
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

machdep_include(task);

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * the init variable, filled by the bootloader, containing in this case
 * the list of segments to mark used.
 */

extern t_init*		init;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the task manager variable.
 */

m_task*			task = NULL;

/*
 * the kernel task.
 */

t_tskid			ktask = ID_UNUSED;

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k4 */

/*
 * this function shows a precise task.
 *
 * steps:
 *
 * 1) XXX
 */

t_error			task_show(t_tskid			tskid)
{
  /* XXX t_state		state; */
  /* XXX t_iterator		i; */
  /* XXX o_task*		o; */

  TASK_ENTER(task);

  /*
   * 1)
   */

  /* XXX
  if (task_get(tskid, &o) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);
  */

  cons_msg('#', "  task %qd:\n", tskid);

  /*
   * 2)
   */

  /* XXX */

  TASK_LEAVE(task, ERROR_NONE);
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

  TASK_ENTER(task);

  /*
   * 1)
   */

  if (set_size(task->container, &size) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 2)
   */

  cons_msg('#', "dumping %qu task(s):\n", size);

  set_foreach(SET_OPT_FORWARD, task->container, &i, state)
    {
      if (set_object(task->container, i, (void**)&data) != ERROR_NONE)
	TASK_LEAVE(task, ERROR_UNKNOWN);

      if (task_show(data->tskid) != ERROR_NONE)
	TASK_LEAVE(task, ERROR_UNKNOWN);
    }

  TASK_LEAVE(task, ERROR_NONE);
}

/*
 * this function clones a task.
 *
 * this function so takes care of cloning the address space and every thread.
 *
 * this function does not clone the waits set. XXX
 *
 * steps:
 *
 * 1) gets the source task object given its identifier.
 * 2) reserves the cloned task object.
 * 3) gets the destination task object previously reserved.
 * 4) clones the address space from the source task object.
 * 5) clones the thread set from the source task object.
 * 6) calls the machine-dependent code.
 */

t_error			task_clone(t_tskid			old,
				   t_tskid*			new)
{
  t_state		state;
  o_task*		from;
  o_task*		to;
  t_iterator		i;

  TASK_ENTER(task);

  /*
   * 1)
   */

  if (task_get(old, &from) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (task_reserve(from->class, from->behav, from->prior, new) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (task_get(*new, &to) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (from->asid != ID_UNUSED)
    {
      if (as_clone(to->tskid, from->asid, &to->asid) != ERROR_NONE)
	TASK_LEAVE(task, ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  set_foreach(SET_OPT_FORWARD, from->threads, &i, state)
    {
      /* XXX t_thrid		needless; */
      t_thrid*		data;

      if (set_object(from->threads, i, (void**)&data) != ERROR_NONE)
	{
	  cons_msg('!', "task: cannot find the object "
		   "corresponding to its identifier\n");

	  TASK_LEAVE(task, ERROR_UNKNOWN);
	}

      /* XXX
	 if (thread_clone(to->tskid, *data, &needless) != ERROR_NONE)
	 TASK_LEAVE(task, ERROR_UNKNOWN);
      */
    }

  /*
   * 6)
   */

  if (machdep_call(task, task_clone, old, new) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  TASK_LEAVE(task, ERROR_NONE);
}

/*
 * this function reserves a task object.
 *
 * steps:
 *
 * 1) checks the validity of arguments.
 * 2) initialises the task object.
 * 3) reserves an identifier for the task object.
 * 4) reserves the set of threads for the new task object.
 * 5) reserves the set of waits for the new task object.
 * 6) adds the new task object in the task container.
 * 7) calls the machine-dependent code.
 */

t_error			task_reserve(t_class			class,
				     t_behav			behav,
				     t_prior			prior,
				     t_tskid*			tskid)
{
  o_task		o;

  TASK_ENTER(task);

  /*
   * 1)
   */

  if ((class != TASK_CLASS_CORE) &&
      (class != TASK_CLASS_DRIVER) &&
      (class != TASK_CLASS_SERVICE) &&
      (class != TASK_CLASS_PROGRAM))
    TASK_LEAVE(task, ERROR_UNKNOWN);

  if ((behav != TASK_BEHAV_CORE) &&
      (behav != TASK_BEHAV_REALTIME) &&
      (behav != TASK_BEHAV_INTERACTIVE) &&
      (behav != TASK_BEHAV_TIMESHARING) &&
      (behav != TASK_BEHAV_BACKGROUND))
    TASK_LEAVE(task, ERROR_UNKNOWN);

  if ((prior != TASK_PRIOR_CORE) &&
      (prior != TASK_PRIOR_REALTIME) &&
      (prior != TASK_PRIOR_INTERACTIVE) &&
      (prior != TASK_PRIOR_TIMESHARING) &&
      (prior != TASK_PRIOR_BACKGROUND))
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 2)
   */

  memset(&o, 0x0, sizeof(o_task));

  o.parent = 0; /* XXX */
  o.class = class;
  o.behav = behav;
  o.prior = prior;

  o.asid = ID_UNUSED;
  o.threads = ID_UNUSED;

  o.sched = 0; /* XXX SCHEDULE_STATE_STOP */

  /* XXX o.status = XXX; */

  o.waits = ID_UNUSED;

  /*
   * 3)
   */

  if (id_reserve(&task->id, tskid) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  o.tskid = *tskid;

  /*
   * 4)
   */

  if (set_reserve(array, SET_OPT_SORT | SET_OPT_ALLOC, TASK_THREADS_INITSZ,
		  sizeof(t_thrid), &o.threads) != ERROR_NONE)
    {
      id_release(&task->id, o.tskid);

      TASK_LEAVE(task, ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  if (set_reserve(array, SET_OPT_SORT | SET_OPT_ALLOC, TASK_WAITS_INITSZ,
		  sizeof(t_thrid), &o.waits) != ERROR_NONE)
    {
      id_release(&task->id, o.tskid);

      set_release(o.threads);

      TASK_LEAVE(task, ERROR_UNKNOWN);
    }

  /*
   * 6)
   */

  if (set_add(task->container, &o) != ERROR_NONE)
    {
      id_release(&task->id, o.tskid);

      set_release(o.threads);
      set_release(o.waits);

      TASK_LEAVE(task, ERROR_UNKNOWN);
    }

  /*
   * 7)
   */

  if (machdep_call(task, task_reserve, class,
		   behav, prior, tskid) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  TASK_LEAVE(task, ERROR_NONE);
}

/*
 * this function releases a task.
 *
 * steps:
 *
 * 1) calls the machine-dependent code.
 * 2) gets the task object given its identifier.
 * 3) releases the task's object identifier.
 * 4) releases the task object's set of threads.
 * 5) releases the task object's set of waits.
 * 6) removes the task object from the task container.
 */

t_error			task_release(t_tskid			tskid)
{
  o_task*		o;

  TASK_ENTER(task);

  /*
   * 1)
   */

  if (machdep_call(task, task_release, tskid) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (task_get(tskid, &o) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (id_release(&task->id, o->tskid) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (o->asid != ID_UNUSED)
    {
      if (as_release(o->asid) != ERROR_NONE)
	TASK_LEAVE(task, ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  /* XXX
     if (thread_flush(o->threads) != ERROR_NONE)
     TASK_LEAVE(task, ERROR_UNKNOWN);
  */

  if (set_release(o->threads) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 5)
   */

  if (set_release(o->waits) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 6)
   */

  if (set_remove(task->container, o->tskid) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  TASK_LEAVE(task, ERROR_NONE);
}

/*
 * this function gets a task object from the task container.
 */

t_error			task_get(t_tskid			tskid,
				 o_task**			o)
{
  TASK_ENTER(task);

  if (set_get(task->container, tskid, (void**)o) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  TASK_LEAVE(task, ERROR_NONE);
}

/*
 * this functions initialises the task manager.
 *
 * this function takes care of initialising and builing the kernel task.
 *
 * steps:
 *
 * 1) allocates and initialises the task manager structure.
 * 2) initialises the identifier object to be able to generate
 *    the task identifiers.
 * 3) reserves the task container set which will contain the task built later.
 * 4) tries to reserve a statistics object.
 * 5) reserves the kernel task and its address space.
 * 6) adds the segments to the kernel address space.
 * 7) adds the regions to the kernel address space.
 * 8) calls the machine-dependent code.
 * 9) if asked, dumps the task manager.
 */

t_error			task_init(void)
{
  t_asid		asid;
  t_id			needless;
  t_uint32		i;

  /*
   * 1)
   */

  if ((task = malloc(sizeof(m_task))) == NULL)
    {
      cons_msg('!', "task: cannot allocate memory for the task manager "
	       "structure\n");

      return (ERROR_UNKNOWN);
    }

  memset(task, 0x0, sizeof(m_task));

  /*
   * 2)
   */

  if (id_build(&task->id) != ERROR_NONE)
    {
      cons_msg('!', "task: unable to initialise the identifier object\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  if (set_reserve(ll, SET_OPT_ALLOC | SET_OPT_SORT,
		  sizeof(o_task), &task->container) != ERROR_NONE)
    {
      cons_msg('!', "task: unable to reserve the task container\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  STATS_RESERVE("task", &task->stats);

  /*
   * 5)
   */

  if (task_reserve(TASK_CLASS_CORE, TASK_BEHAV_CORE,
		   TASK_PRIOR_CORE, &ktask) != ERROR_NONE)
    {
      cons_msg('!', "task: unable to reserve the kernel task\n");

      return (ERROR_UNKNOWN);
    }

  if (as_reserve(ktask, &asid) != ERROR_NONE)
    {
      cons_msg('!', "task: unable to reserve the kernel address space\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 6)
   */

  for (i = 0; i < init->nsegments; i++)
    {
      if (segment_inject(&init->segments[i], asid) != ERROR_NONE)
	{
	  cons_msg('!', "segment: cannot add a pre-reserved segment in "
		   "the segment container\n");

	  return (ERROR_UNKNOWN);
	}
    }

  /*
   * 7)
   */

  for (i = 0; i < init->nregions; i++)
    {
      if (region_reserve(asid, (t_segid)init->regions[i].address, 0,
			 REGION_OPT_FORCE | REGION_OPT_MAPALL,
			 init->regions[i].address, 0,
			 &needless) != ERROR_NONE)
	{
	  cons_msg('!', "region: cannot map a region to a pre-reserved "
		   "segment\n");

	  return (ERROR_UNKNOWN);
	}
    }

  /*
   * XXX on vient de regenerer l'as du kernel, il faudrait le switcher
   * normalement. donc la on le fait en dur dans le machdep_call,
   * faudra changer.
   */

  /*
   * 8)
   */

  if (machdep_call(task, task_init) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 9)
   */

#if (DEBUG & DEBUG_TASK)
  task_dump();
#endif

  return (ERROR_NONE);
}

/*
 * this function just reinitialises the task manager.
 *
 * steps:
 *
 * 1) calls the machine-dependent code.
 * 2) releases the kernel task.
 * 3) releases the statistics object.
 * 4) releases the task's container.
 * 5) destroys the id object.
 * 6) frees the task manager structure's memory.
 */

t_error			task_clean(void)
{
  /*
   * 1)
   */

  if (task_release(ktask) != ERROR_NONE)
    {
      cons_msg('!', "task: unable to release the kernel task\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 2)
   */

  if (machdep_call(task, task_clean) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 3)
   */

  STATS_RELEASE(task->stats);

  /*
   * 4)
   */

  if (set_release(task->container) != ERROR_NONE)
    {
      cons_msg('!', "task: unable to release the task container\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  if (id_destroy(&task->id) != ERROR_NONE)
    {
      cons_msg('!', "task: unable to destroy the identifier object\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 6)
   */

  free(task);

  return (ERROR_NONE);
}

/*                                                                  [cut] k4 */
