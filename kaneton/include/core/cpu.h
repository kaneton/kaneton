/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/cpu.h
 *
 * created       matthieu bucchianeri   [sat jul 29 18:07:16 2006]
 * updated       matthieu bucchianeri   [sat jul 29 18:08:00 2006]
 */

#ifndef CORE_CPU_H
#define CORE_CPU_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <core/id.h>
#include <core/types.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * cpu object
 */

typedef struct
{
  // XXX
}		o_cpu;

/*
 * cpu manager
 */

typedef struct
{
  i_stats	stats;

  machdep_data(m_cpu);
}		m_cpu;

/*
 * the cpu architecture-dependent interface
 */

typedef struct
{
  t_error			(*cpu_init)(void);
  t_error			(*cpu_clean)(void);
}				d_cpu;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define CPU_CHECK(_cpu_)						\
  {									\
    if ((_cpu_) == NULL)						\
      return (ERROR_UNKNOWN);						\
  }

/*
 * enter
 */

#define CPU_ENTER(_cpu_)						\
  {									\
    CPU_CHECK((_cpu_));							\
									\
    STATS_BEGIN((_cpu_)->stats);					\
  }

/*
 * leave
 */

#define CPU_LEAVE(_cpu_, _error_)					\
  {									\
    STATS_END((_cpu_)->stats, (_error_));				\
									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/cpu/cpu.c
 */

/*
 * ../../core/cpu/cpu.c
 */

t_error			cpu_init(void);

t_error			cpu_clean(void);


/*
 * eop
 */

#endif
