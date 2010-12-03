/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...ne/glue/ibm-pc.ia32/educational/thread.c
 *
 * created       renaud voltz   [tue apr  4 03:08:03 2006]
 * updated       julien quintard   [thu dec  2 16:17:14 2010]
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
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the thread manager dispatch.
 */

d_thread		glue_thread_dispatch =
  {
    NULL,
    NULL,
    NULL,
    glue_thread_load,
    glue_thread_store,
    glue_thread_reserve,
    NULL,
    NULL,
    NULL,
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
 * reserve a thread on the ia32 architecture
 */

t_error			glue_thread_reserve(i_task		taskid,
					    i_thread*		threadid)
{
  if (ia32_init_context(taskid, *threadid) != ERROR_OK)
    MACHINE_ESCAPE("unable to initialize the IA32 context");

  MACHINE_LEAVE();
}

/*
 * this function updates the context with the new stack and
 * instruction pointers.
 */

t_error			glue_thread_load(i_thread		threadid,
					 t_thread_context	context)
{
  if (ia32_setup_context(threadid, context.pc, context.sp) != ERROR_OK)
    MACHINE_ESCAPE("unable to set up the IA32 context");

  MACHINE_LEAVE();
}

/*
 * this function reads from the context both the stack and instruction
 * pointers.
 */

t_error			glue_thread_store(i_thread		threadid,
					  t_thread_context*	context)
{
  if (ia32_status_context(threadid, &context->pc, &context->sp) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the IA32 context");

  // XXX ici on devrai recup le ia32 context et copier ce qu'il faut dans le
  // thread context.

  MACHINE_LEAVE();
}

/*
 * this function pushes function arguments onto the stack.
 */

t_error			glue_thread_args(i_thread		threadid,
					 const void*	       	args,
					 t_vsize		size)
{
  if (ia32_push_args(threadid, args, size) != ERROR_OK)
    MACHINE_ESCAPE("unable to push the arguments");

  MACHINE_LEAVE();
}

/*
 * initialize the machine related structures for the thread manager.
 */

t_error			glue_thread_initialize(void)
{
  if (ia32_init_switcher() != ERROR_OK)
    MACHINE_ESCAPE("unable to initialize the context switcher");

  MACHINE_LEAVE();
}
