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
 * updated       matthieu bucchianeri   [sat dec  8 20:20:59 2007]
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
    NULL,
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
 * this function reserves an address space.
 */

t_status		glue_as_reserve(i_task			tskid,
					i_as*			asid)
{
  AS_ENTER(as);

  if (tskid == ktask)
    {
      if (ia32_kernel_as_init(*asid) != STATUS_OK)
	AS_LEAVE(as, STATUS_UNKNOWN_ERROR);
    }
  else
    {
      if (ia32_task_as_init(*asid) != STATUS_OK)
	AS_LEAVE(as, STATUS_UNKNOWN_ERROR);
    }

  AS_LEAVE(as, STATUS_OK);
}
