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
 * updated       julien quintard   [sun jun 10 19:03:30 2007]
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
 * io manager
 */

typedef struct
{
  machdep_data(m_io);
}				m_io;

/*
 * the io architecture-dependent interface
 */

typedef struct
{
  t_error			(*io_grant)(i_port,
					    i_task,
					    t_uint8);
  t_error			(*io_deny)(i_port,
					   i_task,
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
  t_error			(*io_init)(void);
  t_error			(*io_clean)(void);
}				d_io;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define IO_CHECK(_io_)							\
  {									\
    if ((_io_) == NULL)							\
      return (ERROR_UNKNOWN);						\
  }

/*
 * enter
 */

#define IO_ENTER(_io_)							\
  {									\
    IO_CHECK((_io_));							\
  }

/*
 * leave
 */

#define IO_LEAVE(_io_, _error_)						\
  {									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/io/io.c
 */

/*
 * ../../core/io/io.c
 */

t_error			io_grant(i_port				id,
				 i_task				task,
				 t_uint8			width);

t_error			io_deny(i_port				id,
				i_task				task,
				t_uint8				width);

t_error			io_read_8(i_task			task,
				  i_port			id,
				  t_uint8*			data);

t_error			io_read_16(i_task			task,
				   i_port			id,
				   t_uint16*			data);

t_error			io_read_32(i_task			task,
				   i_port			id,
				   t_uint32*			data);

t_error			io_read_64(i_task			task,
				   i_port			id,
				   t_uint64*			data);

t_error			io_write_8(i_task			task,
				   i_port			id,
				   t_uint8			data);

t_error			io_write_16(i_task			task,
				    i_port			id,
				    t_uint16			data);

t_error			io_write_32(i_task			task,
				    i_port			id,
				    t_uint32			data);

t_error			io_write_64(i_task			task,
				    i_port			id,
				    t_uint64			data);

t_error			io_init(void);

t_error			io_clean(void);


/*
 * eop
 */

#endif
