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
 * updated       julien quintard   [thu nov 25 11:36:45 2010]
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
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the task manager.
 */

extern m_task*		_task;

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
  TASK_ENTER(_task);

  if (ia32_duplicate_io_bitmap(old, *new) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  TASK_LEAVE(_task, ERROR_OK);
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

  TASK_ENTER(_task);

  if (ia32_clear_io_bitmap(*id) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  if (task_get(*id, &o) != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  o->machine.ioflush = 0;

  TASK_LEAVE(_task, ERROR_OK);
}

/*
 * this function makes some architecture dependent initialization for
 * the task manager.
 */

t_error			glue_task_initialize(void)
{
  TASK_ENTER(_task);

  if (ia32_extended_context_init() != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  if (ia32_kernel_as_finalize() != ERROR_OK)
    TASK_LEAVE(_task, ERROR_KO);

  TASK_LEAVE(_task, ERROR_OK);
}
