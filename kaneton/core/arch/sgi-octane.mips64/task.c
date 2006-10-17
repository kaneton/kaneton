/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/core/arch/sgi-octane.mips64/task.c
 *
 * created       Enguerrand RAYMOND   [tue oct 17 13:40:45 2006]
 * updated       Enguerrand RAYMOND   [tue oct 17 22:44:41 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements  dependent code for task manager  on octane with
 * paging architecture.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <liboctane.h>
#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the task manager dispatch.
 */

d_task			task_dispatch =
  {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
  };

/*
 * ---------- functions -------------------------------------------------------
 */


