/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/arch/ibm-pc.ia32-virtual/io.c
 *
 * created       matthieu bucchianeri   [sat jul 29 18:04:35 2006]
 * updated       matthieu bucchianeri   [wed nov  1 17:08:00 2006]
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

#include <klibc.h>
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
    ia32_io_grant,
    ia32_io_deny,
    ia32_io_read_8,
    ia32_io_read_16,
    ia32_io_read_32,
    ia32_io_read_64,
    ia32_io_write_8,
    ia32_io_write_16,
    ia32_io_write_32,
    ia32_io_write_64,
    ia32_io_init,
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function modifies a bit in a bitmap.
 */

static void		ia32_io_bitmap_set(t_uint8*		bitmap,
					   t_uint32		port,
					   t_uint32		value)
{
  if (value)
    bitmap[port / 8] |= (1 << (port % 8));
  else
    bitmap[port / 8] &= ~(1 << (port % 8));
}

/*
 * this function checks one or more bits in a bitmap. bits to '1' have
 * higher precedence.
 */

static t_uint8		ia32_io_bitmap_isset(t_uint8*		bitmap,
					     t_uint32		port,
					     t_uint8		consecutive)
{
  t_uint8		i;

  for (i = 0 ; i < consecutive; i++)
    if ((bitmap[port / 8] & (1 << (port % 8))) != 0)
      return 1;
  return 0;
}

/*
 * this function allow I/O to a port.
 *
 * steps:
 *
 * 1) check the port id.
 * 2) get the task.
 * 3) change permission bitmap.
 */

t_error			ia32_io_grant(i_port			id,
				      i_task			task,
				      t_uint8			width)
{
  o_task*		o;
  t_uint8		i;

  IO_ENTER(io);

  /*
   * 1)
   */

  if (id + width >= 65536)
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (task_get(task, &o) != ERROR_NONE)
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 3)
   */

  for (i = 0; i < width; i++)
    ia32_io_bitmap_set(o->machdep.iomap, id + i, 0);

  IO_LEAVE(io, ERROR_NONE);
}

/*
 * this function deny I/O to a port.
 *
 * steps:
 *
 * 1) check the port id.
 * 2) get the task.
 * 3) change permission bitmap.
 */

t_error			ia32_io_deny(i_port			id,
				     i_task			task,
				     t_uint8			width)
{
  o_task*		o;
  t_uint8		i;

  IO_ENTER(io);

  /*
   * 1)
   */

  if (id + width >= 65536)
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (task_get(task, &o) != ERROR_NONE)
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 3)
   */

  for (i = 0; i < width; i++)
    ia32_io_bitmap_set(o->machdep.iomap, id + i, 1);

  IO_LEAVE(io, ERROR_NONE);
}

/*
 * read from 8 bit I/O port.
 *
 * steps:
 *
 * 1) check the port id.
 * 2) get the task.
 * 3) check permission bitmap.
 * 4) perform the I/O.
 */

t_error			ia32_io_read_8(i_task			task,
				       i_port			id,
				       t_uint8*			data)
{
  o_task*		o;

  IO_ENTER(io);

  /*
   * 1)
   */

  if (id + 1 >= 65536)
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (task_get(task, &o) != ERROR_NONE)
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (ia32_io_bitmap_isset(o->machdep.iomap, id, 1))
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 4)
   */

  INB((t_uint16)id, *data);

  IO_LEAVE(io, ERROR_NONE);
}

/*
 * read from 16 bit I/O port.
 *
 * steps:
 *
 * 1) check the port id.
 * 2) get the task.
 * 3) check permission bitmap.
 * 4) perform the I/O.
 */

t_error			ia32_io_read_16(i_task			task,
					i_port			id,
					t_uint16*		data)
{
  o_task*		o;

  IO_ENTER(io);

  /*
   * 1)
   */

  if (id + 2 >= 65536)
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (task_get(task, &o) != ERROR_NONE)
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (ia32_io_bitmap_isset(o->machdep.iomap, id, 2))
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 4)
   */

  INW((t_uint16)id, *data);

  IO_LEAVE(io, ERROR_NONE);
}

/*
 * read from 32 bit I/O port.
 *
 * steps:
 *
 * 1) check the port id.
 * 2) get the task.
 * 3) check permission bitmap.
 * 4) perform the I/O.
 */

t_error			ia32_io_read_32(i_task			task,
					i_port			id,
					t_uint32*		data)
{
  o_task*		o;

  IO_ENTER(io);

  /*
   * 1)
   */

  if (id + 4 >= 65536)
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (task_get(task, &o) != ERROR_NONE)
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (ia32_io_bitmap_isset(o->machdep.iomap, id, 4))
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 4)
   */

  INL((t_uint16)id, *data);

  IO_LEAVE(io, ERROR_NONE);
}

/*
 * read from 64 bit I/O port. not supported in IA-32.
 */

t_error			ia32_io_read_64(i_task			task,
					i_port			id,
					t_uint64*		data)
{
  IO_ENTER(io);

  IO_LEAVE(io, ERROR_UNKNOWN);
}

/*
 * write to 8 bit I/O port.
 *
 * steps:
 *
 * 1) check the port id.
 * 2) get the task.
 * 3) check permission bitmap.
 * 4) perform the I/O.
 */

t_error			ia32_io_write_8(i_task			task,
					i_port			id,
					t_uint8			data)
{
  o_task*		o;

  IO_ENTER(io);

  /*
   * 1)
   */

  if (id + 1 >= 65536)
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (task_get(task, &o) != ERROR_NONE)
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (ia32_io_bitmap_isset(o->machdep.iomap, id, 1))
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 4)
   */

  OUTB((t_uint16)id, data);

  IO_LEAVE(io, ERROR_NONE);
}

/*
 * write to 16 bit I/O port.
 *
 * steps:
 *
 * 1) check the port id.
 * 2) get the task.
 * 3) check permission bitmap.
 * 4) perform the I/O.
 */

t_error			ia32_io_write_16(i_task			task,
					 i_port			id,
					 t_uint16		data)
{
  o_task*		o;

  IO_ENTER(io);

  /*
   * 1)
   */

  if (id + 2 >= 65536)
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (task_get(task, &o) != ERROR_NONE)
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (ia32_io_bitmap_isset(o->machdep.iomap, id, 2))
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 4)
   */

  OUTW((t_uint16)id, data);

  IO_LEAVE(io, ERROR_NONE);
}

/*
 * write to 32 bit I/O port.
 *
 * steps:
 *
 * 1) check the port id.
 * 2) get the task.
 * 3) check permission bitmap.
 * 4) perform the I/O.
 */

t_error			ia32_io_write_32(i_task			task,
					 i_port			id,
					 t_uint32		data)
{
  o_task*		o;

  IO_ENTER(io);

  /*
   * 1)
   */

  if (id + 4 >= 65536)
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (task_get(task, &o) != ERROR_NONE)
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (ia32_io_bitmap_isset(o->machdep.iomap, id, 4))
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 4)
   */

  OUTL((t_uint16)id, data);

  IO_LEAVE(io, ERROR_NONE);
}

/*
 * write to 64 bit I/O port. not supported on IA-32.
 */

t_error			ia32_io_write_64(i_task			task,
					 i_port			id,
					 t_uint64		data)
{
  IO_ENTER(io);

  IO_LEAVE(io, ERROR_UNKNOWN);
}

/*
 * this function initialise the I/O manager.
 *
 * steps:
 *
 * 1) set the IOPL flag to 0.
 */

t_error			ia32_io_init(void)
{
  IO_ENTER(io);

  /*
   * 1)
   */

  asm volatile("pushf\n\t"
	       "andl $0xFFFFCFFF, %ss:(%esp)\n\t"
	       "popf");

  IO_LEAVE(io, ERROR_NONE);
}
