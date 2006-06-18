/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/sched.h
 *
 * created       matthieu bucchianeri   [sat jun  3 22:34:42 2006]
 * updated       matthieu bucchianeri   [sat jun 17 18:28:12 2006]
 */

#ifndef CORE_SCHED_H
#define CORE_SCHED_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <core/id.h>
#include <core/types.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define SCHED_STATE_RUN		0
#define SCHED_STATE_STOP	1
#define SCHED_STATE_ZOMBIE	2

#define SCHED_QUANTUM_INIT	50 // ms

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * scheduler manager
 */

typedef struct
{
  i_stats	stats;

  t_quantum	quantum;

  i_thread	current;

  i_set		threads;

  machdep_data(m_sched);
}		m_sched;

/*
 * the scheduler architecture-dependent interface
 */

typedef struct
{
  t_error			(*sched_quantum)(t_quantum);
  t_error			(*sched_yield)(void);
  t_error			(*sched_switch)(i_thread);
  t_error			(*sched_add)(i_thread);
  t_error			(*sched_remove)(i_thread);
  t_error			(*sched_update)(i_thread);
  t_error			(*sched_init)(void);
  t_error			(*sched_clean)(void);
}				d_sched;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define SCHED_CHECK(_sched_)						\
  {									\
    if ((_sched_) == NULL)						\
      return (ERROR_UNKNOWN);						\
  }

/*
 * enter
 */

#define SCHED_ENTER(_sched_)						\
  {									\
    SCHED_CHECK((_sched_));						\
									\
    STATS_BEGIN((_sched_)->stats);					\
  }

/*
 * leave
 */

#define SCHED_LEAVE(_sched_, _error_)					\
  {									\
    STATS_END((_sched_)->stats, (_error_));				\
									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/sched/sched.c
 */

/*
 * ../../core/sched/sched.c
 */

t_error			sched_dump(void);

t_error			sched_quantum(t_quantum			quantum);

t_error			sched_yield(void);

t_error			sched_current(i_thread*			thread);

t_error			sched_switch(void);

t_error			sched_add(i_thread			thread);

t_error			sched_remove(i_thread			thread);

t_error			sched_update(i_thread			thread);

t_error			sched_init(void);

t_error			sched_clean(void);


/*
 * eop
 */

#endif
