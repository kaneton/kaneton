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
