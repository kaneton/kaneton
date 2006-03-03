/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/core/kaneton/arch/ia32-virtual/task.c
 *
 * created       julien quintard   [sat dec 10 15:22:46 2005]
 * updated       matthieu bucchianeri   [fri mar  3 13:25:12 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements  dependent code for task manager  on ia32 with
 * paging architecture.
 *
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * students will have to develop this entire part of the project. take
 * a look at the interface  structure declaration to fill it correctly
 * and to write needed functions.
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
extern t_asid		kasid;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the address space manager interface.
 */

i_task			task_interface =
  {

/*                                                                  [cut] k2 */

    NULL,
    ia32_task_clone,
    ia32_task_reserve,
    ia32_task_release,
    ia32_task_init,
    ia32_task_clean

/*                                                                 [cut] /k2 */

  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

/*
 * XXX
 */

t_error			ia32_task_clone(t_tskid			old,
					t_tskid*		new)
{
  TASK_ENTER(task);

  /* XXX */

  TASK_LEAVE(task, ERROR_NONE);
}

/*
 * XXX
 */

t_error			ia32_task_reserve(t_class		class,
					  t_behav		behav,
					  t_prior		prior,
					  t_tskid*		tskid)
{
  TASK_ENTER(task);

  /* XXX */

  TASK_LEAVE(task, ERROR_NONE);
}

/*
 * XXX
 */

t_error			ia32_task_release(t_tskid		tskid)
{
  TASK_ENTER(task);

  /* XXX */

  TASK_LEAVE(task, ERROR_NONE);
}

/*
 * this function initializes the task manager and the kernel task.
 *
 * steps:
 *
 * 1) get the kernel address space.
 * 2) activate the new kernel address space.
 */

t_error			ia32_task_init(void)
{
  o_as*			o;

  TASK_ENTER(task);

  /*
   * 1)
   */

  if (as_get(kasid, &o) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (pd_activate(o->machdep.pd) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  TASK_LEAVE(task, ERROR_NONE);
}

/*
 * XXX
 */

t_error			ia32_task_clean(void)
{
  TASK_ENTER(task);

  /* XXX */

  TASK_LEAVE(task, ERROR_NONE);
}

/*                                                                 [cut] /k2 */
