/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/io/io.c
 *
 * created       matthieu bucchianeri   [sat jul 29 17:59:35 2006]
 * updated       julien quintard   [fri jan 14 20:03:08 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the I/O manager provides functionalities for grant and denying access
 * to low-level hardware i.e referred to as ports.
 *
 * since low-level I/O are hardware-specific, the manager relies on the
 * machine for implementating most of the manager's functionalities.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * include the machine-specific definitions required by the core.
 */

machine_include(io);

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * I/O manager structure.
 */

m_io*			_io = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function grants access on the given port and width to the
 * identified task.
 *
 * steps:
 *
 * 1) call the machine.
 */

t_error			io_grant(i_task				task,
				 i_port				port,
				 t_width			width)
{
  /*
   * 1)
   */

  if (machine_call(io, grant, task, port, width) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function denies access on a given port to the task.
 *
 * steps:
 *
 * 1) call the machine.
 */

t_error			io_deny(i_task				task,
				i_port				port,
				t_width				width)
{
  /*
   * 1)
   */

  if (machine_call(io, deny, task, port, width) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function reads data from an I/O port.
 *
 * steps:
 *
 * 0) verify the arguments
 * 1) call the machine.
 */

t_error			io_read(i_task				task,
				i_port				port,
				t_width				width,
				void*				data)
{
  /*
   * 0)
   */

  if (data == NULL)
    CORE_ESCAPE("the 'data' argument is null");

  /*
   * 1)
   */

  if (machine_call(io, read, task, port, width, data) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function writes data to an I/O port.
 *
 * steps:
 *
 * 1) call the machine.
 */

t_error			io_write_8(i_task			task,
				   i_port			port,
				   t_width			width,
				   t_uint64			data)
{
  /*
   * 1)
   */

  if (machine_call(io, write, task, port, width, data) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function initializes the I/O manager.
 *
 * steps:
 *
 * 1) display a message.
 * 2) allocate and initialize memory for the manager structure.
 * 3) call the machine.
 */

t_error			io_initialize(void)
{
  /*
   * 1)
   */

  module_call(console, message,
	      '+', "initializing the I/O manager\n");

  /*
   * 2)
   */

  if ((_io = malloc(sizeof (m_io))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the IO manager's structure");

  memset(_io, 0x0, sizeof (m_io));

  /*
   * 3)
   */

  if (machine_call(io, initialize) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function cleans the I/O manager.
 *
 * steps:
 *
 * 1) display a message.
 * 2) call the machine.
 * 3) free the manager structure.
 */

t_error			io_clean(void)
{
  /*
   * 1)
   */

  module_call(console, message,
	      '+', "cleaning the I/O manager\n");

  /*
   * 2)
   */

  if (machine_call(io, clean) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 3)
   */

  free(_io);

  CORE_LEAVE();
}
