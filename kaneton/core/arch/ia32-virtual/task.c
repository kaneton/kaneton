/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/arch/ia32-virtual/task.c
 *
 * created       julien quintard   [sat dec 10 15:22:46 2005]
 * updated       matthieu bucchianeri   [tue jul 25 23:27:49 2006]
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
    NULL,
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

t_error			ia32_task_init(void)
{
  TASK_ENTER(task);

  task_setup();

  TASK_LEAVE(task, ERROR_NONE);
}

/*                                                                 [cut] /k4 */
