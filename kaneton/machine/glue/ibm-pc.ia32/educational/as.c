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
 * updated       julien quintard   [thu apr  7 14:27:18 2011]
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

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the kernel manager.
 */

extern m_kernel		_kernel;

/*
 * the address space manager.
 */

extern m_as		_as;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the address space dispatcher.
 */

d_as			glue_as_dispatch =
  {
    glue_as_show,
    glue_as_dump,
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

t_status		glue_as_show(i_as			id,
				     mt_margin			margin)
{
  o_as*			o;

  /*
   * 1)
   */

  if (as_get(id, &o) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "  machine: pd(0x%x)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      o->machine.pd);

  MACHINE_LEAVE();
}

/*
 * this function displays additional machine-specific information on
 * the address space manager.
 *
 * steps:
 *
 * 1) display the machine-specific attributes.
 */

t_status		glue_as_dump(void)
{
  /*
   * 1)
   */

  module_call(console, message,
	      '#',
	      "  machine: pd(0x%x)\n",
	      _as.machine.pd);

  MACHINE_LEAVE();
}

/*
 * this function initializes the given address space depending to the task,
 * being either the kernel or one of the servers i.e a driver, service or
 * guest.
 */

t_status		glue_as_reserve(i_task			task,
					i_as*			as)
{
  if (*as == _kernel.as)
    {
      if (architecture_environment_kernel(*as) != STATUS_OK)
	MACHINE_ESCAPE("unable to initialize the kernel's address space");
    }
  else
    {
      if (architecture_environment_server(*as) != STATUS_OK)
	MACHINE_ESCAPE("unable to initialize the server's address space");
    }

  MACHINE_LEAVE();
}
