/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/arch/ia32-virtual/task.c
 *
 * created       julien quintard   [sat dec 10 15:22:46 2005]
 * updated       matthieu bucchianeri   [sat jul 29 18:36:02 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements  dependent code for task manager  on ia32 with
 * paging architecture.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

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

/*                                                                  [cut] k4 */

    NULL,
    NULL,
    ia32_task_reserve,
    NULL,
    NULL,
    NULL,
    NULL,
    ia32_task_init,
    NULL,

/*                                                                 [cut] /k4 */

  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k4 */

/*
 *
 */

t_error			ia32_task_reserve(t_class			class,
					  t_behav			behav,
					  t_prior			prior,
					  i_task*			id)
{
  o_task*		o;

  TASK_ENTER(task);

  if (task_get(*id, &o) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  memset(&o->machdep.iomap, 0xFF, 8192);

  TASK_LEAVE(task, ERROR_NONE);
}

/*
 *
 * 2) disable writing the GDT.
 */

t_error			ia32_task_init(void)
{
  TASK_ENTER(task);

  /*
   * 1)
   */

  task_setup();

  /*
   * 2)
   */

  if (segment_perms((i_segment)(t_uint32)init->machdep.gdt.descriptor,
		    PERM_READ) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  TASK_LEAVE(task, ERROR_NONE);
}

/*                                                                 [cut] /k4 */
