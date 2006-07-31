/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/arch/ia32-virtual/io.c
 *
 * created       matthieu bucchianeri   [sat jul 29 18:04:35 2006]
 * updated       matthieu bucchianeri   [sat jul 29 19:35:01 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX
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
 * this function allow or deny I/O to a port.
 *
 * steps:
 *
 * 1) check the port id.
 * 2) get the task.
 * 3) change permission bitmap.
 */

t_error			ia32_io_grant(i_port			id,
				      i_task			task,
				      t_state			state)
{
  o_task*		o;

  IO_ENTER(io);

  /*
   * 1)
   */

  if (id >= 65536)
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (task_get(task, &o) != ERROR_NONE)
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 3)
   */

  ia32_io_bitmap_set(o->machdep.iomap, id, !state);

  IO_LEAVE(io, ERROR_NONE);
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
