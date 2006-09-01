/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/io.h
 *
 * created       matthieu bucchianeri   [sat jul 29 18:06:00 2006]
 * updated       matthieu bucchianeri   [fri sep  1 15:20:54 2006]
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
					    i_task);
  t_error			(*io_deny)(i_port,
					   i_task);
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
				 i_task				task);

t_error			io_deny(i_port				id,
				i_task				task);

t_error			io_init(void);

t_error			io_clean(void);


/*
 * eop
 */

#endif
