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
 * updated       julien quintard   [sun dec 19 17:31:50 2010]
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
 * ---------- macros ----------------------------------------------------------
 */

/*
 * these macros defines the width of the I/O operations.
 */

#define IO_WIDTH_8		(1 << 0)
#define IO_WIDTH_16		(1 << 1)
#define IO_WIDTH_32		(1 << 2)
#define IO_WIDTH_64		(1 << 3)

/*
 * ---------- macro-functions -------------------------------------------------
 */

/*
 * this macro-function is used to cast any pointer to a generic
 * input/output.
 *
 * this is required since the I/O manager's functions handle all
 * types through a single routine.
 */

#define IO_INPUT(_value_)	((t_uint64)_value_)
#define IO_OUTPUT(_pointer_)	((void*)_pointer_)

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
  t_status		(*io_grant)(i_task,
					    i_port,
					    t_width);
  t_status		(*io_deny)(i_task,
					   i_port,
					   t_width);
  t_status		(*io_read)(i_task,
					   i_port,
					   t_width,
					   void*);
  t_status		(*io_write)(i_task,
					    i_port,
					    t_width,
					    t_uint64);
  t_status		(*io_initialize)(void);
  t_status		(*io_clean)(void);
}				d_io;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/io/io.c
 */

/*
 * ../../core/io/io.c
 */

t_status		io_grant(i_task				task,
				 i_port				port,
				 t_width			width);

t_status		io_deny(i_task				task,
				i_port				port,
				t_width				width);

t_status		io_read(i_task				task,
				i_port				port,
				t_width				width,
				void*				data);

t_status		io_write_8(i_task			task,
				   i_port			port,
				   t_width			width,
				   t_uint64			data);

t_status		io_initialize(void);

t_status		io_clean(void);


/*
 * eop
 */

#endif
