/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/io.h
 *
 * created       matthieu bucchianeri   [sat jul 29 18:06:00 2006]
 * updated       matthieu bucchianeri   [wed nov  1 17:08:12 2006]
 */

#ifndef CORE_IO_H
#define CORE_IO_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <core/id.h>
#include <core/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * io manager
 */

typedef struct
{
  i_stats	stats;

  machdep_data(m_io);
}		m_io;

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
    									\
    STATS_BEGIN((_io_)->stats);						\
  }

/*
 * leave
 */

#define IO_LEAVE(_io_, _error_)						\
  {									\
    STATS_END((_io_)->stats, (_error_));				\
									\
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
