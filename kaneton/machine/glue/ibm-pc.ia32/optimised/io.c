/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../machine/glue/ibm-pc.ia32/optimised/io.c
 *
 * created       matthieu bucchianeri   [sat jul 29 18:04:35 2006]
 * updated       julien quintard   [fri may  1 19:44:20 2009]
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
 * ---------- extern ----------------------------------------------------------
 */

extern m_io*	io;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the io manager dispatch.
 */

d_io		io_dispatch =
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

t_status		glue_io_grant(i_port			id,
				      i_task			task,
				      t_uint8			width)
{
  IO_ENTER(io);

  /* XXX flush AP */
  if (ia32_set_io_bitmap(task, id, width, 1) != STATUS_OK)
    IO_LEAVE(io, STATUS_UNKNOWN_ERROR);

  IO_LEAVE(io, STATUS_OK);
}

/*
 * this function deny I/O to a port.
 */

t_status		glue_io_deny(i_port			id,
				     i_task			task,
				     t_uint8			width)
{
  IO_ENTER(io);

  /* XXX flush AP */
  if (ia32_set_io_bitmap(task, id, width, 0) != STATUS_OK)
    IO_LEAVE(io, STATUS_UNKNOWN_ERROR);

  IO_LEAVE(io, STATUS_OK);
}


/*
 * this function initialize the I/O manager.
 */

t_status		glue_io_initialize(void)
{
  IO_ENTER(io);

  if (ia32_reset_iopl() != STATUS_OK)	/* XXX broadcast to AP */
    IO_LEAVE(io, STATUS_UNKNOWN_ERROR);

  IO_LEAVE(io, STATUS_OK);
}
