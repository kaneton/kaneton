/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/cry...ne/glue/ibm-pc.ia32/educational/thread.c
 *
 * created       renaud voltz   [tue apr  4 03:08:03 2006]
 * updated       matthieu bucchianeri   [wed jan  9 13:07:50 2008]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX THREAD information need to be written.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * the thread manager.
 */

extern m_thread*	thread;

/*
 * we'll use the kernel address space identifier.
 */

extern i_as		kasid;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the thread manager dispatch.
 */

d_thread			thread_dispatch =
  {
    NULL,
    NULL,
    glue_thread_clone,
    NULL,
    glue_thread_load,
    glue_thread_store,
    glue_thread_reserve,
    NULL,
    NULL,
    NULL,
    NULL,
    glue_thread_args,
    glue_thread_initialize,
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * clone the ia32 architecture dependent part of a thread.
 *
 */

t_error			glue_thread_clone(i_task		taskid,
					  i_thread		old,
					  i_thread*		new)
{
  o_task*		o;

  THREAD_ENTER(thread);

  if (ia32_duplicate_context(old, *new) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  if (task_get(taskid, &o) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  THREAD_LEAVE(thread, ERROR_NONE);
}

/*
 * reserve a thread on the ia32 architecture
 *
 */

t_error			glue_thread_reserve(i_task		taskid,
					    i_thread*		threadid)
{
  THREAD_ENTER(thread);

  if (ia32_init_context(taskid, *threadid) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  THREAD_LEAVE(thread, ERROR_NONE);
}

/*
 * this function updates the context with the new stack and
 * instruction pointers.
 *
 */

t_error			glue_thread_load(i_thread		threadid,
					 t_thread_context	context)
{
  THREAD_ENTER(thread);

  if (ia32_setup_context(threadid, context.pc, context.sp) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  THREAD_LEAVE(thread, ERROR_NONE);
}

/*
 * this function reads from the context both the stack and instruction
 * pointers.
 *
 */

t_error			glue_thread_store(i_thread		threadid,
					  t_thread_context*	context)
{
  THREAD_ENTER(thread);

  if (ia32_status_context(threadid, &context->pc, &context->sp) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  THREAD_LEAVE(thread, ERROR_NONE);
}

/*
 * this function pushes function arguments onto the stack.
 *
 */

t_error			glue_thread_args(i_thread		threadid,
					 const void*	       	args,
					 t_vsize		size)
{
  THREAD_ENTER(thread);

  if (ia32_push_args(threadid, args, size) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  THREAD_LEAVE(thread, ERROR_NONE);
}

/*
 * initialize the machine related structures for the thread manager.
 *
 */

t_error			glue_thread_initialize(void)
{
  THREAD_ENTER(thread);

  if (ia32_init_switcher() != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  THREAD_LEAVE(thread, ERROR_NONE);
}
