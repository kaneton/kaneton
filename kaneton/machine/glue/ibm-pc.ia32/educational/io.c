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
 * updated       julien quintard   [fri dec 10 13:49:47 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file provides the complementary implementations for the core's
 * I/O manager.
 *
 * the grant and deny functions set up the right bits into the task's
 * TSS I/O permissions bitmap to allow or refuse I/O on the given port.
 *
 * the read and write functions SHOULD NOT be used in servers. prefer
 * the direct use of assembly instructions INB, INW, INL, OUTB, OUTW, and
 * OUTL. these functions lead to bad performance, and are here only to fit
 * the I/O manager.
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
    glue_io_read_8,
    glue_io_read_16,
    glue_io_read_32,
    glue_io_read_64,
    glue_io_write_8,
    glue_io_write_16,
    glue_io_write_32,
    glue_io_write_64,
    glue_io_initialize,
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function enables I/Os on the given port for the given task
 * by properly setting the I/O bitmap.
 */

t_error			glue_io_grant(i_task			task,
				      i_port			port,
				      t_uint8			width)
{
  if (ia32_set_io_bitmap(task, port, width, 1) != ERROR_OK)
    MACHINE_ESCAPE("unable to set the IO bitmap");

  MACHINE_LEAVE();
}

/*
 * this function deny I/O to a port.
 */

t_error			glue_io_deny(i_task			task,
				     i_port			port,
				     t_uint8			width)
{
  if (ia32_set_io_bitmap(task, port, width, 0) != ERROR_OK)
    MACHINE_ESCAPE("unable to set the IO bitmap");

  MACHINE_LEAVE();
}

/*
 * XXX
 */

t_error			glue_io_read_8(i_task			task,
				       i_port			port,
				       t_uint8*			data)
{
  ARCHITECTURE_IO_IN_8(port, *data);

  MACHINE_LEAVE();
}

/*
 * XXX
 */

t_error			glue_io_read_16(i_task			task,
					i_port			port,
					t_uint16*		data)
{
  ARCHITECTURE_IO_IN_16(port, *data);

  MACHINE_LEAVE();
}

/*
 * XXX
 */

t_error			glue_io_read_32(i_task			task,
					i_port			port,
					t_uint32*		data)
{
  ARCHITECTURE_IO_IN_32(port, *data);

  MACHINE_LEAVE();
}

/*
 * XXX
 */

t_error			glue_io_read_64(i_task			task,
					i_port			port,
					t_uint64*		data)
{
  MACHINE_ESCAPE("this operation is not supported in this machine");
}

/*
 * XXX
 */

t_error			glue_io_write_8(i_task			task,
					i_port			port,
					t_uint8			data)
{
  ARCHITECTURE_IO_OUT_8(port, data);

  MACHINE_LEAVE();
}

/*
 * XXX
 */

t_error			glue_io_write_16(i_task			task,
					 i_port			port,
					 t_uint16		data)
{
  ARCHITECTURE_IO_OUT_16(port, data);

  MACHINE_LEAVE();
}

/*
 * XXX
 */

t_error			glue_io_write_32(i_task			task,
					 i_port			port,
					 t_uint32		data)
{
  ARCHITECTURE_IO_OUT_32(port, data);

  MACHINE_LEAVE();
}

/*
 * XXX
 */

t_error			glue_io_write_64(i_task			task,
					 i_port			port,
					 t_uint64		data)
{
  MACHINE_ESCAPE("this operation is not supported in this machine");
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
