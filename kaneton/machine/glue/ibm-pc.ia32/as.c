/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/machine/glue/ibm-pc.ia32/as.c
 *
 * created       matthieu bucchianeri   [sat jun 16 18:10:38 2007]
 * updated       matthieu bucchianeri   [sat jun 16 19:43:56 2007]
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
 * ---------- externs ---------------------------------------------------------
 */

extern m_as*		as;
extern i_task		ktask;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the ibm-pc.ia32 as manager dispatch.
 */

d_as				as_dispatch =
  {
    NULL,
    glue_as_give,
    NULL,
    NULL,
    NULL,
    glue_as_reserve,
    NULL,
    NULL,
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function give an address space to a task.
 */

t_error			glue_as_give(i_task			tskid,
				     i_as			asid)
{
  AS_ENTER(as);

  if (ia32_update_pdbr(tskid, asid) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function reserves an address space.
 */

t_error			glue_as_reserve(i_task			tskid,
					i_as*			asid)
{
  AS_ENTER(as);

  if (tskid == ktask)
    {
      if (ia32_kernel_as_init(*asid) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);
    }
  else
    {
      if (ia32_task_as_init(*asid) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      if (ia32_update_pdbr(tskid, *asid) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);
    }

  AS_LEAVE(as, ERROR_NONE);
}
