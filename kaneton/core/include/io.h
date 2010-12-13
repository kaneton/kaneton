/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/io.h
 *
 * created       julien quintard   [wed jun  6 13:26:01 2007]
 * updated       julien quintard   [fri dec 10 21:18:08 2010]
 */

#ifndef CORE_IO_H
#define CORE_IO_H			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/error.h>
#include <core/id.h>

#include <machine/machine.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the I/O manager structure.
 */

typedef struct
{
  machine_data(m_io);
}				m_io;

/*
 * the I/O dispatcher.
 */

typedef struct
{
  t_error			(*io_grant)(i_task,
					    i_port,
					    t_uint8);
  t_error			(*io_deny)(i_task,
					   i_port,
					   t_uint8);
  t_error			(*io_read_8)(i_task,
					     i_port,
					     t_uint8*);
  t_error			(*io_read_16)(i_task,
					      i_port,
					      t_uint16*);
  t_error			(*io_read_32)(i_task,
					      i_port,
					      t_uint32*);
  t_error			(*io_read_64)(i_task,
					      i_port,
					      t_uint64*);
  t_error			(*io_write_8)(i_task,
					      i_port,
					      t_uint8);
  t_error			(*io_write_16)(i_task,
					       i_port,
					       t_uint16);
  t_error			(*io_write_32)(i_task,
					       i_port,
					       t_uint32);
  t_error			(*io_write_64)(i_task,
					       i_port,
					       t_uint64);
  t_error			(*io_initialize)(void);
  t_error			(*io_clean)(void);
}				d_io;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/io/io.c
 */

/*
 * ../../core/io/io.c
 */

t_error			io_grant(i_task				task,
				 i_port				port,
				 t_uint8			width);

t_error			io_deny(i_task				task,
				i_port				port,
				t_uint8				width);

t_error			io_read_8(i_task			task,
				  i_port			port,
				  t_uint8*			data);

t_error			io_read_16(i_task			task,
				   i_port			port,
				   t_uint16*			data);

t_error			io_read_32(i_task			task,
				   i_port			port,
				   t_uint32*			data);

t_error			io_read_64(i_task			task,
				   i_port			port,
				   t_uint64*			data);

t_error			io_write_8(i_task			task,
				   i_port			port,
				   t_uint8			data);

t_error			io_write_16(i_task			task,
				    i_port			port,
				    t_uint16			data);

t_error			io_write_32(i_task			task,
				    i_port			port,
				    t_uint32			data);

t_error			io_write_64(i_task			task,
				    i_port			port,
				    t_uint64			data);

t_error			io_initialize(void);

t_error			io_clean(void);


/*
 * eop
 */

#endif
