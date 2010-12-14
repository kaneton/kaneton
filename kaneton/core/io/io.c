/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/io/io.c
 *
 * created       matthieu bucchianeri   [sat jul 29 17:59:35 2006]
 * updated       matthieu bucchianeri   [wed nov  1 17:07:30 2006]
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
				 t_uint8			width)
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
				t_uint8				width)
{
  /*
   * 1)
   */

  if (machine_call(io, deny, task, port, width) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function reads a byte from an I/O port.
 *
 * steps:
 *
 * 0) verify the arguments
 * 1) call the machine.
 */

t_error			io_read_8(i_task			task,
				  i_port			port,
				  t_uint8*			data)
{
  /*
   * 0)
   */

  if (data == NULL)
    CORE_ESCAPE("the 'data' argument is null");

  /*
   * 1)
   */

  if (machine_call(io, read_8, task, port, data) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function reads a 16-bit word from an I/O port.
 *
 * steps:
 *
 * 0) verify the arguments
 * 1) call the machine.
 */

t_error			io_read_16(i_task			task,
				   i_port			port,
				   t_uint16*			data)
{
  /*
   * 0)
   */

  if (data == NULL)
    CORE_ESCAPE("the 'data' argument is null");

  /*
   * 1)
   */

  if (machine_call(io, read_16, task, port, data) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function reads a 32-bit word from an I/O port.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) call the machine.
 */

t_error			io_read_32(i_task			task,
				   i_port			port,
				   t_uint32*			data)
{
  /*
   * 0)
   */

  if (data == NULL)
    CORE_ESCAPE("the 'data' argument is null");

  /*
   * 1)
   */

  if (machine_call(io, read_32, task, port, data) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function reads a 64-bit word from an I/O port.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) call the machine.
 */

t_error			io_read_64(i_task			task,
				   i_port			port,
				   t_uint64*			data)
{
  /*
   * 0)
   */

  if (data == NULL)
    CORE_ESCAPE("the 'data' argument is null");

  /*
   * 1)
   */

  if (machine_call(io, read_64, task, port, data) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function writes a byte to an I/O port.
 *
 * steps:
 *
 * 1) call the machine.
 */

t_error			io_write_8(i_task			task,
				   i_port			port,
				   t_uint8			data)
{
  /*
   * 1)
   */

  if (machine_call(io, write_8, task, port, data) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function writes a 16-bit word to an I/O port.
 *
 * steps:
 *
 * 1) call the machine.
 */

t_error			io_write_16(i_task			task,
				    i_port			port,
				    t_uint16			data)
{
  /*
   * 1)
   */

  if (machine_call(io, write_16, task, port, data) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function writes a 32-bit word to an I/O port.
 *
 * steps:
 *
 * 1) call the machine.
 */

t_error			io_write_32(i_task			task,
				    i_port			port,
				    t_uint32			data)
{
  /*
   * 1)
   */

  if (machine_call(io, write_32, task, port, data) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function writes a 64-bit word to an I/O port.
 *
 * steps:
 *
 * 1) call the machine.
 */

t_error			io_write_64(i_task			task,
				    i_port			port,
				    t_uint64			data)
{
  /*
   * 1)
   */

  if (machine_call(io, write_64, task, port, data) != ERROR_OK)
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
