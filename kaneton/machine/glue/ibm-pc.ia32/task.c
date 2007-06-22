/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/machine/glue/ibm-pc.ia32/task.c
 *
 * created       matthieu bucchianeri   [sat jun 16 18:10:38 2007]
 * updated       julien quintard   [fri jun 22 18:34:32 2007]
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

#include <libc.h>
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
 */

t_error			glue_task_clone(i_task			old,
					i_task*			new)
{
  TASK_ENTER(task);

  if (ia32_duplicate_io_bitmap(old, *new) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  TASK_LEAVE(task, ERROR_NONE);
}

/*
 * this function initialize the dependent structures.
 */

t_error			glue_task_reserve(t_class			class,
					  t_behav			behav,
					  t_prior			prior,
					  i_task*			id)
{
  TASK_ENTER(task);

  if (ia32_clear_io_bitmap(*id) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  TASK_LEAVE(task, ERROR_NONE);
}

/*
 * this function makes some architecture dependent initialization for
 * the task manager.
 */

t_error			glue_task_initialize(void)
{
  TASK_ENTER(task);

  if (ia32_extended_context_init() != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  TASK_LEAVE(task, ERROR_NONE);
}
