/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hine/glue/ibm-pc.ia32/educational/task.c
 *
 * created       matthieu bucchianeri   [sat jun 16 18:10:38 2007]
 * updated       julien quintard   [sat nov 27 16:25:45 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements dependent code for as manager on ia32 with paging
 * architecture.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <glue/glue.h>
#include <architecture/architecture.h>
#include <platform/platform.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the task manager dispatch.
 */

d_task			glue_task_dispatch =
  {
    NULL,
    glue_task_clone,
    glue_task_reserve,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    glue_task_initialize,
    NULL,
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function clones the dependent part of a task.
 *
 */

t_error			glue_task_clone(i_task			old,
					i_task*			new)
{
  if (ia32_duplicate_io_bitmap(old, *new) != ERROR_OK)
    MACHINE_ESCAPE("unable to duplicate the IO bitmap");

  MACHINE_LEAVE();
}

/*
 * this function initialize the dependent structures.
 *
 */

t_error			glue_task_reserve(t_class		class,
					  t_behaviour		behav,
					  t_priority		prior,
					  i_task*		id)
{
  o_task*		o;

  if (ia32_clear_io_bitmap(*id) != ERROR_OK)
    MACHINE_ESCAPE("unable to clear the IO bitmap");

  if (task_get(*id, &o) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the task object");

  o->machine.ioflush = 0;

  MACHINE_LEAVE();
}

/*
 * this function makes some architecture dependent initialization for
 * the task manager.
 */

t_error			glue_task_initialize(void)
{
  if (ia32_extended_context_init() != ERROR_OK)
    MACHINE_ESCAPE("unable to initialize the IA32 context");

  if (ia32_kernel_as_finalize() != ERROR_OK)
    MACHINE_ESCAPE("unable to finilize the address space");

  MACHINE_LEAVE();
}

// XXX context_init -> setup
