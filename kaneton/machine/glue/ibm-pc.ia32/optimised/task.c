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
 * updated       julien quintard   [wed apr 15 07:24:08 2009]
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
 * ---------- extern ----------------------------------------------------------
 */

extern m_task*		task;
extern i_as		kasid;
extern t_init*		init;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the task manager dispatch.
 */

d_task			task_dispatch =
  {
    NULL,
    glue_task_clone,
    glue_task_reserve,
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

t_status		glue_task_clone(i_task			old,
					i_task*			new)
{
  TASK_ENTER(task);

  if (ia32_duplicate_io_bitmap(old, *new) != STATUS_OK)
    TASK_LEAVE(task, STATUS_UNKNOWN_ERROR);

  TASK_LEAVE(task, STATUS_OK);
}

/*
 * this function initialize the dependent structures.
 *
 */

t_status		glue_task_reserve(t_class			class,
					  t_behav			behav,
					  t_prior			prior,
					  i_task*			id)
{
  o_task*		o;

  TASK_ENTER(task);

  if (ia32_clear_io_bitmap(*id) != STATUS_OK)
    TASK_LEAVE(task, STATUS_UNKNOWN_ERROR);

  if (task_get(*id, &o) != STATUS_OK)
    TASK_LEAVE(task, STATUS_UNKNOWN_ERROR);

  o->machine.ioflush = 0;

  TASK_LEAVE(task, STATUS_OK);
}

/*
 * this function makes some architecture dependent initialization for
 * the task manager.
 */

t_status		glue_task_initialize(void)
{
  TASK_ENTER(task);

  if (ia32_extended_context_init() != STATUS_OK)
    TASK_LEAVE(task, STATUS_UNKNOWN_ERROR);

  if (ia32_kernel_as_finalize() != STATUS_OK)
    TASK_LEAVE(task, STATUS_UNKNOWN_ERROR);

  TASK_LEAVE(task, STATUS_OK);
}
