/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/arch/ibm-pc.ia32-virtual/task.c
 *
 * created       julien quintard   [sat dec 10 15:22:46 2005]
 * updated       matthieu bucchianeri   [tue feb  6 23:54:12 2007]
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

#include <libc.h>
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
    NULL,
    ia32_task_clone,
    ia32_task_reserve,
    NULL,
    NULL,
    NULL,
    NULL,
    ia32_task_init,
    NULL,
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function clones the dependent part of a task.
 *
 * it only copies the I/O permission bitmap.
 */

t_error			ia32_task_clone(i_task			old,
					i_task*			new)
{
  o_task*		from;
  o_task*		to;


  TASK_ENTER(task);

  if (task_get(old, &from) != ERROR_NONE ||
      task_get(*new, &to) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  memcpy(to->machdep.iomap, from->machdep.iomap, 8192);

  TASK_LEAVE(task, ERROR_NONE);
}

/*
 * this function initialize the dependent structures.
 *
 * it only clears the I/O permission bitmap, denying all I/O operations.
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
 * this function makes some architecture dependent initialization for
 * the task manager.
 *
 * steps:
 *
 * 1) initialize libia32 context module.
 * 2) disable writing the GDT.
 */

t_error			ia32_task_init(void)
{
  TASK_ENTER(task);

  /*
   * 1)
   */

  context_setup();

  /*
   * 2)
   */

  if (segment_perms((i_segment)(t_uint32)init->machdep.gdt.descriptor,
		    PERM_READ) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  TASK_LEAVE(task, ERROR_NONE);
}
