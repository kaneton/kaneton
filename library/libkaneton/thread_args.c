/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/library/libkaneton/thread_args.c
 *
 * created       matthieu bucchianeri   [mon aug  6 19:07:18 2007]
 * updated       matthieu bucchianeri   [mon sep 10 00:11:30 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements uninterfacable system call thread_args.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <stdint.h>
#include <string.h>
#include <libkaneton.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function pushes arguments on a thread's stack.
 *
 * steps:
 *
 * 1) get asid attribute.
 * 2) get current thread context and decrement the stack pointer.
 * 3) copy the argument onto the stack.
 * 4) update the thread context.
 */

t_error			thread_args(i_thread			threadid,
				    const void*			args,
				    t_vsize			size)
{
  i_task		taskid;
  i_as			asid;
  t_thread_context	context;

  /*
   * 1)
   */


  if (thread_attribute_taskid(threadid, &taskid) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (task_attribute_asid(taskid, &asid) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (thread_store(threadid, &context) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  context.sp -= size;

  /*
   * 3)
   */

  if (__as_id != asid)
    {
      if (as_copy(__as_id, (t_vaddr)args,
		  asid, context.sp, size) != ERROR_NONE)
	return (ERROR_UNKNOWN);
    }
  else
    {
      memcpy((void*)context.sp, args, size);
    }

  /*
   * 4)
   */

  if (thread_load(threadid, context) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}
