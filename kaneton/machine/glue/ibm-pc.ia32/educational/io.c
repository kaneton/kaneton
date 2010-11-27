/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...achine/glue/ibm-pc.ia32/educational/io.c
 *
 * created       matthieu bucchianeri   [sat jul 29 18:04:35 2006]
 * updated       julien quintard   [sat nov 27 16:21:31 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the architecture depent part of the I/O
 * manager for IA-32 microprocessors.
 *
 * the grant and deny function setup the right bits into the task's
 * TSS I/O permission bitmap to allow or refuse I/O on the given port.
 *
 * the read and write functions SHOULD NOT be used in
 * applications. prefer the direct use of assembly instructions inb,
 * inw, in, outb, outw, and out. these functions leads to bad
 * performances, and are here only to fit the io manager interface.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the io manager dispatch.
 */

d_io			glue_io_dispatch =
  {
    glue_io_grant,
    glue_io_deny,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    glue_io_initialize,
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function allow I/O to a port.
 */

t_error			glue_io_grant(i_port			id,
				      i_task			task,
				      t_uint8			width)
{
  if (ia32_set_io_bitmap(task, id, width, 1) != ERROR_OK)
    MACHINE_ESCAPE("unable to set the IO bitmap");

  MACHINE_LEAVE();
}

/*
 * this function deny I/O to a port.
 */

t_error			glue_io_deny(i_port			id,
				     i_task			task,
				     t_uint8			width)
{
  if (ia32_set_io_bitmap(task, id, width, 0) != ERROR_OK)
    MACHINE_ESCAPE("unable to set the IO bitmap");

  MACHINE_LEAVE();
}


/*
 * this function initialize the I/O manager.
 */

t_error			glue_io_initialize(void)
{
  if (ia32_reset_iopl() != ERROR_OK)
    MACHINE_ESCAPE("unable to reset the IOPL");

  MACHINE_LEAVE();
}
