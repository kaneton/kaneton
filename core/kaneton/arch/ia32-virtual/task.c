/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/core/kaneton/arch/ia32-virtual/task.c
 *
 * created       julien quintard   [sat dec 10 15:22:46 2005]
 * updated       matthieu bucchianeri   [sat feb 18 18:53:57 2006]
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

t_error			ia32_task_clone(t_tskid		old,
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
 * XXX
 */

t_error			ia32_task_init(void)
{
  TASK_ENTER(task);

  /* XXX */

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
