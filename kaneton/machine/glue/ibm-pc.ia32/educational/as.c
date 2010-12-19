/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...achine/glue/ibm-pc.ia32/educational/as.c
 *
 * created       matthieu bucchianeri   [sat jun 16 18:10:38 2007]
 * updated       julien quintard   [sun dec 19 18:21:34 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the glue for the address space manager.
 *
 * the glue particularly sets up the initial address space's state by
 * building the IA32 page directory. note that glue_as_reserve()
 * detects the kernel address space as this one must be prepared specially.
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
 * the address space dispatcher.
 */

d_as			glue_as_dispatch =
  {
    glue_as_show,
    NULL,
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
 * this function displays additional machine-specific information on
 * the address space.
 *
 * steps:
 *
 * 1) retrieve the address space object.
 * 2) display the machine-specific attributes.
 */

t_error			glue_as_show(i_as			id,
				     mt_margin			margin)
{
  o_as*			o;

  /*
   * 1)
   */

  if (as_get(id, &o) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "  machine: pd(0x%08x)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      o->machine.pd);

  MACHINE_LEAVE();
}

/*
 * this function reserves an address space.
 *
 * XXX
 */

t_error			glue_as_reserve(i_task			task,
					i_as*			as)
{
  if (*as == _kernel->as)
    {
      if (ia32_kernel_as_initialize(*as) != ERROR_OK)
	MACHINE_ESCAPE("unable to initialize the kernel's address space");
    }
  else
    {
      if (ia32_task_as_initialize(*as) != ERROR_OK)
	MACHINE_ESCAPE("unable to initialize the task's address space");
    }

  MACHINE_LEAVE();
}
