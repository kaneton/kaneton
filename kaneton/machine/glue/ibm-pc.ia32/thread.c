/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...aneton/machine/glue/ibm-pc.ia32/thread.c
 *
 * created       renaud voltz   [tue apr  4 03:08:03 2006]
 * updated       matthieu bucchianeri   [mon jul 30 17:06:58 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX THREAD information need to be written.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * the thread manager.
 */

extern m_thread*	thread;

/*                                                                  [cut] k3 */

/*
 * we'll use the kernel address space identifier.
 */

extern i_as		kasid;

/*                                                                 [cut] /k3 */

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the thread manager dispatch.
 */

d_thread			thread_dispatch =
  {

/*                                                                  [cut] k3 */

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
    NULL,
    glue_thread_initialize,
    NULL

/*                                                                 [cut] /k3 */

  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k3 */

/*
 * clone the ia32 architecture dependent part of a thread.
 */

t_error			glue_thread_clone(i_task		taskid,
					  i_thread		old,
					  i_thread*		new)
{
  THREAD_ENTER(thread);

  if (ia32_duplicate_context(old, *new) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  THREAD_LEAVE(thread, ERROR_NONE);
}

/*
 * reserve a thread on the ia32 architecture
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
 * initialize the machdep related structures for the thread manager.
 */

t_error			glue_thread_initialize(void)
{
  THREAD_ENTER(thread);

  if (ia32_init_switcher() != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  THREAD_LEAVE(thread, ERROR_NONE);
}
