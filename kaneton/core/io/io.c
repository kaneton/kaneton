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
 * the I/O  manager is  designed to grant  or deny  low-level hardware
 * accesses to tasks.
 *
 * the only available operation is allowing or denying the access of a
 * given hardware address (called a port) to a task.
 *
 * as low-level I/O are hardware specific, the manager only does calls
 * to the architecture dependent code.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

machdep_include(io);

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * io manager variable.
 */

m_io*			io = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function grant I/O to a given port for a task.
 */

t_error			io_grant(i_port				id,
				 i_task				task,
				 t_uint8			width)
{
  IO_ENTER(io);

  if (machdep_call(io, io_grant, id, task, width) != ERROR_NONE)
    IO_LEAVE(io, ERROR_UNKNOWN);

  IO_LEAVE(io, ERROR_NONE);
}

/*
 * this function deny I/O to a given port for a task.
 */

t_error			io_deny(i_port				id,
				i_task				task,
				t_uint8				width)
{
  IO_ENTER(io);

  if (machdep_call(io, io_deny, id, task, width) != ERROR_NONE)
    IO_LEAVE(io, ERROR_UNKNOWN);

  IO_LEAVE(io, ERROR_NONE);
}

/*
 * this function read a byte from an I/O port or memory-mapped I/O.
 */

t_error			io_read_8(i_task			task,
				  i_port			id,
				  t_uint8*			data)
{
  IO_ENTER(io);

  IO_LEAVE(io, machdep_call(io, io_read_8, task, id, data));
}

/*
 * this function read a 16 bits word from an I/O port or memory-mapped I/O.
 */

t_error			io_read_16(i_task			task,
				   i_port			id,
				   t_uint16*			data)
{
  IO_ENTER(io);

  IO_LEAVE(io, machdep_call(io, io_read_16, task, id, data));
}

/*
 * this function read a 32 bits word from an I/O port or memory-mapped I/O.
 */

t_error			io_read_32(i_task			task,
				   i_port			id,
				   t_uint32*			data)
{
  IO_ENTER(io);

  IO_LEAVE(io, machdep_call(io, io_read_32, task, id, data));
}

/*
 * this function read a 64 bits word from an I/O port or memory-mapped I/O.
 */

t_error			io_read_64(i_task			task,
				   i_port			id,
				   t_uint64*			data)
{
  IO_ENTER(io);

  IO_LEAVE(io, machdep_call(io, io_read_64, task, id, data));
}

/*
 * this function write a byte to an I/O port or memory-mapped I/O.
 */

t_error			io_write_8(i_task			task,
				   i_port			id,
				   t_uint8			data)
{
  IO_ENTER(io);

  IO_LEAVE(io, machdep_call(io, io_write_8, task, id, data));
}

/*
 * this function write a 16 bits word to an I/O port or memory-mapped I/O.
 */

t_error			io_write_16(i_task			task,
				    i_port			id,
				    t_uint16			data)
{
  IO_ENTER(io);

  IO_LEAVE(io, machdep_call(io, io_write_16, task, id, data));
}

/*
 * this function write a 32 bits word to an I/O port or memory-mapped I/O.
 */

t_error			io_write_32(i_task			task,
				    i_port			id,
				    t_uint32			data)
{
  IO_ENTER(io);

  IO_LEAVE(io, machdep_call(io, io_write_32, task, id, data));
}

/*
 * this function write a 64 bits word to an I/O port or memory-mapped I/O.
 */

t_error			io_write_64(i_task			task,
				    i_port			id,
				    t_uint64			data)
{
  IO_ENTER(io);

  IO_LEAVE(io, machdep_call(io, io_write_64, task, id, data));
}

/*
 * this function initialises the io manager.
 *
 * steps:
 *
 * 1) allocate some memory for the manager structure.
 * 2) call the machine dependent code.
 */

t_error			io_init(void)
{
  /*
   * 1)
   */

  if ((io = malloc(sizeof(m_io))) == NULL)
    {
      cons_msg('!', "io: cannot allocate memory for the io "
	       "manager structure\n");

      return (ERROR_UNKNOWN);
    }

  memset(io, 0x0, sizeof(m_io));

  /*
   * 2)
   */

  if (machdep_call(io, io_init) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);

}

/*
 * this function cleans the io manager.
 *
 * steps:
 *
 * 1) call the dependent code.
 * 2) free the manager structure.
 */

t_error			io_clean(void)
{
  /*
   * 1)
   */

  if (machdep_call(io, io_clean) != ERROR_NONE)
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 2)
   */

  free(io);

  return (ERROR_NONE);
}
