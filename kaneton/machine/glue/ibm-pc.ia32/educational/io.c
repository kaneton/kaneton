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
 * updated       julien quintard   [sun dec 19 17:32:56 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the I/O manager's glue.
 *
 * note that the read and write functions *should not* be used in servers.
 * indeed, the direct use of assembly instructions such as INB, INW, INL,
 * OUTB, OUTW, and OUTL should be preferred because (i) these functions lead
 * to very bad performance and (ii) some servers, the drivers, benefit from
 * special hardware privileges granting them access to any port.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the I/O dispatcher.
 */

d_io			glue_io_dispatch =
  {
    glue_io_grant,
    glue_io_deny,
    glue_io_read,
    glue_io_write,
    glue_io_initialize,
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function enables I/Os on the given port for the given task
 * by properly setting the I/O bitmap.
 *
 * XXX
 */

t_error			glue_io_grant(i_task			task,
				      i_port			port,
				      t_width			width)
{
  if (ia32_set_io_bitmap(task, port, width, 1) != ERROR_OK)
    MACHINE_ESCAPE("unable to set the I/O bitmap");

  MACHINE_LEAVE();
}

/*
 * this function denies access on the given I/O port.
 *
 * XXX
 */

t_error			glue_io_deny(i_task			task,
				     i_port			port,
				     t_width			width)
{
  if (ia32_set_io_bitmap(task, port, width, 0) != ERROR_OK)
    MACHINE_ESCAPE("unable to set the I/O bitmap");

  MACHINE_LEAVE();
}

/*
 * XXX
 */

t_error			glue_io_read(i_task			task,
				     i_port			port,
				     t_width			width,
				     void*			data)
{
  switch (width)
    {
    case IO_WIDTH_8:
      {
	ARCHITECTURE_IO_IN_8(port, *((t_uint8*)data));

	break;
      }
    case IO_WIDTH_16:
      {
	ARCHITECTURE_IO_IN_16(port, *((t_uint16*)data));

	break;
      }
    case IO_WIDTH_32:
      {
	ARCHITECTURE_IO_IN_32(port, *((t_uint32*)data));

	break;
      }
    case IO_WIDTH_64:
      MACHINE_ESCAPE("unsupported width '%u'",
		     width);
    default:
      MACHINE_ESCAPE("unknown width '%u'",
		     width);
    }

  MACHINE_LEAVE();
}

/*
 * XXX
 */

t_error			glue_io_write(i_task			task,
				      i_port			port,
				      t_width			width,
				      t_uint64			data)
{
  switch (width)
    {
    case IO_WIDTH_8:
      {
	ARCHITECTURE_IO_OUT_8(port, (t_uint8)data);

	break;
      }
    case IO_WIDTH_16:
      {
	ARCHITECTURE_IO_OUT_16(port, (t_uint16)data);

	break;
      }
    case IO_WIDTH_32:
      {
	ARCHITECTURE_IO_OUT_32(port, (t_uint32)data);

	break;
      }
    case IO_WIDTH_64:
      MACHINE_ESCAPE("unsupported width '%u'",
		     width);
    default:
      MACHINE_ESCAPE("unknown width '%u'",
		     width);
    }

  MACHINE_LEAVE();
}

/*
 * this function initialize the I/O manager.
 */

t_error			glue_io_initialize(void)
{
  if (ia32_reset_iopl() != ERROR_OK)
    MACHINE_ESCAPE("unable to reset the IOPL - I/O Privilege Level");

  MACHINE_LEAVE();
}
