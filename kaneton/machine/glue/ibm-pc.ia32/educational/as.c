/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...achine/glue/ibm-pc.ia32/educational/as.c
 *
 * created       matthieu bucchianeri   [sat jun 16 18:10:38 2007]
 * updated       julien quintard   [thu dec  2 16:21:32 2010]
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
 * the kernel manager.
 */

extern m_kernel*	_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the ibm-pc.ia32 as manager dispatch.
 */

d_as			glue_as_dispatch =
  {
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

t_error			glue_as_reserve(i_task			tskid,
					i_as*			asid)
{
  if (tskid == _kernel->task)
    {
      if (ia32_kernel_as_initialize(*asid) != ERROR_OK)
	MACHINE_ESCAPE("unable to initialize the kernel's address space");
    }
  else
    {
      if (ia32_task_as_initialize(*asid) != ERROR_OK)
	MACHINE_ESCAPE("unable to initialize the task's address space");
    }

  MACHINE_LEAVE();
}
