/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/thread/thread.c
 *
 * created       renaud voltz   [tue apr  4 03:02:57 2006]
 * updated       julien quintard   [sat jul  8 02:28:36 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the thread manager.
 */

/*                                                                  [cut] k4 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

machdep_include(thread);

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the thread manager variable.
 */

m_thread*		thread = NULL;

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
  o_thread*		o;

  THREAD_ENTER(thread);

  /*
   * 1)
   */

  if (thread_get(threadid, &o) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   * 2)
   */


  /*
   * 3)
   */

  if (machdep_call(thread, thread_show, threadid) != ERROR_UNKNOWN)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);


  THREAD_LEAVE(thread, ERROR_NONE);
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
  t_state		state;
  o_thread*		data;
  t_setsz		size;
  t_iterator		i;

  THREAD_ENTER(thread);

  /*
   * 1)
   */

  if (set_size(thread->threads, &size) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   * 2)
   */

  cons_msg('#', "dumping %qu thread(s):\n");

  set_foreach(SET_OPT_FORWARD, thread->threads, &i, state)
    {
      if (set_object(thread->threads, i, (void**)&data) != ERROR_UNKNOWN)
	THREAD_LEAVE(thread, ERROR_UNKNOWN);

      if (thread_show(data->threadid) != ERROR_NONE)
	THREAD_LEAVE(thread, ERROR_UNKNOWN);
    }

  THREAD_LEAVE(thread, ERROR_NONE);
}

/*
 * this function reserves a thread for a given task.
 *
 * steps:
 *
 * 1) get the task object.
 * 2) get an id for the new thread and fill its information.
 * 3)
 * 4) add the new thread in the thread container and in the task threads list.
 * 5) call the machine-dependent code.
 */

t_error			thread_reserve(i_task			taskid,
				       i_thread*		threadid)
{
  o_task*		task;
  o_thread		o;

  THREAD_ENTER(thread);

  /*
   * 1)
   */

  if (task_get(taskid, &task) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (id_reserve(&thread->id, threadid) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   * 3)
   */

  memset(&o, 0x0, sizeof(o_thread));

  o.threadid = *threadid;

  o.taskid = taskid;

  /*
   * 4)
   */

  if (set_add(thread->threads, &o) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  if (set_add(task->threads, &o.threadid) != ERROR_NONE)
    {
      set_remove(thread->threads, o.threadid);

      THREAD_LEAVE(thread, ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  if (machdep_call(thread, thread_reserve, taskid, *threadid) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  THREAD_LEAVE(thread, ERROR_NONE);
}

/*
 * this function releases a thread from a given task.
 *
 * steps:
 *
 * 1) call the machine-dependent code.
 * 2) get the thread object.
 * 3) XXX THREAD: release the thread stack.
 * 4) get the task object.
 * 5) release the thread-s object identifer.
 * 6) remove the thread from the task threads list.
 * 7) remove the thread from the threads set.
 */

t_error			thread_release(i_thread			threadid)
{
  o_task*		task;
  o_thread*		o;

  THREAD_ENTER(thread);

  /*
   * 1)
   */

  if (machdep_call(thread, thread_release, threadid) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (thread_get(threadid, &o) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   * 3)
   */

  /* XXX THREAD: release the thread stack */

  /*
   * 4)
   */

  if (task_get(o->taskid, &task) != ERROR_UNKNOWN)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   * 5)
   */

  if (id_release(&thread->id, o->threadid) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   * 6)
   */

  if (set_remove(task->threads, threadid) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   * 7)
   */

  if (set_remove(thread->threads, threadid) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  THREAD_LEAVE(thread, ERROR_NONE);
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

  THREAD_ENTER(thread);

  /*
   * 1)
   */

  if (machdep_call(thread, thread_flush, taskid) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (task_get(taskid, &task) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   * 3)
   */

  while (set_head(task->threads, &i) == ERROR_NONE)
    {
      if (set_object(task->threads, i, (void**)&data) != ERROR_NONE)
	{
	  cons_msg('!', "thread: cannot find the object "
		   "corresponding to ist identifier\n");

	  THREAD_LEAVE(thread, ERROR_UNKNOWN);
	}
      if (thread_release(*data) != ERROR_NONE)
	THREAD_LEAVE(thread, ERROR_UNKNOWN);
    }

  THREAD_LEAVE(thread, ERROR_UNKNOWN);
}

/*
 * XXX THREAD: verifier qu'une pile n'est pas deja allouee.
 */

t_error			thread_stack(i_thread			threadid,
				     t_vsize			size)
{
  o_task*		task;
  o_thread*		o;

  THREAD_ENTER(thread);


  /*
   *
   */

  if (thread_get(threadid, &o) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   *
   */

  if (task_get(o->taskid, &task) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   * XXX THREAD: opts + perms
   */

  if (map_reserve(task->asid, 0, size, 0, &(o->stack)) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  o->stacksz = size;

  /*
   *
   */

  if (machdep_call(thread, thread_stack, threadid, size) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  THREAD_LEAVE(thread, ERROR_NONE);
}

/*
 *
 */

t_error			thread_load(i_thread			threadid,
				    t_thread_context		context)
{
  o_thread*		o;

  THREAD_ENTER(thread);

  /*
   *
   */

  if (thread_get(threadid, &o) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   *
   */

  o->stack = context.sp;

  /*
   *
   */

  if (machdep_call(thread, thread_load, threadid, context) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  THREAD_LEAVE(thread, ERROR_NONE);
}

/*
 *
 */

t_error			thread_store(i_thread			threadid,
				     t_thread_context*		context)
{
  THREAD_ENTER(thread);

  /*
   *
   */

  if (machdep_call(thread, thread_store, threadid, context) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  THREAD_LEAVE(thread, ERROR_NONE);
}

/*
 * this function gets an thread object from the thread container.
 */

t_error			thread_get(i_thread			threadid,
				   o_thread**			o)
{
  THREAD_ENTER(thread);

  if (set_get(thread->threads, threadid, (void**)o) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  THREAD_LEAVE(thread, ERROR_NONE);
}

/*
 * initialise the thread manager.
 *
 * steps:
 *
 * 1) allocate and initialise the thread manager.
 * 2) initialise the object identifier.
 * 3) reserve the thread set.
 * 4) try to reserve a statistic object.
 * 5) call the machine-dependent code.
 * 6) dump the thread manager if debug is enabled.
 */

t_error			thread_init(void)
{
  /*
   * 1)
   */

  if ((thread = malloc(sizeof(m_thread))) == NULL)
    {
      cons_msg('!', "thread: cannot allocate memory "
	       "for the thread manager structure\n");

      return ERROR_UNKNOWN;
    }

  memset(thread, 0x0, sizeof(m_thread));

  /*
   * 2)
   */

  if (id_build(&thread->id) != ERROR_NONE)
    {
      cons_msg('!', "thread: unable to initialise the identifier object\n");

      return ERROR_NONE;
    }

  /*
   * 3)
   */

  if (set_reserve(ll, SET_OPT_ALLOC, sizeof(o_thread), &thread->threads)
      != ERROR_NONE)
    {
      cons_msg('!', "thread: unable to reserve the thread set\n\n");

      return ERROR_NONE;
    }

  /*
   * 4)
   */

  STATS_RESERVE("thread", &thread->stats);

  /*
   * 5)
   */

  if (machdep_call(thread, thread_init) != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 6)
   */

#if (DEBUG & DEBUG_THREAD)
  if (thread_dump() != ERROR_UNKNOWN)
    return ERROR_UNKNOWN;
#endif

  return ERROR_NONE;
}

/*
 * destroy the thread manager.
 *
 * steps:
 *
 * 1) call the machine-dependent code.
 * 2) release the statistics object.
 * 3) release every thread object.
 * 4) destroy the identifier object.
 * 5) free the thread manager's structure memory.
 */

t_error			thread_clean(void)
{
  /*
   * 1)
   */

  if (machdep_call(thread, thread_clean) != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  STATS_RELEASE(thread->stats);

  /*
   * 3)
   */

  if (set_release(thread->threads) != ERROR_NONE)
    {
      cons_msg('!', "thread: unable to release the thread set\n");

      return ERROR_UNKNOWN;
    }

  /*
   * 4)
   */

  if (id_destroy(&thread->id) != ERROR_NONE)
    {
      cons_msg('!', "thread: unable to destroy the identifier object\n");

      return ERROR_UNKNOWN;
    }

  /*
   * 5)
   */

  free(thread);

  return ERROR_NONE;
}

/*                                                                 [cut] /k4 */
