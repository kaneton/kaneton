/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/cpu.h
 *
 * created       matthieu bucchianeri   [sat jul 29 18:07:16 2006]
 * updated       matthieu bucchianeri   [fri aug 18 19:40:24 2006]
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
  i_cpu		cpuid;

  t_timeslice	efficiency;

  machdep_data(o_cpu);
}		o_cpu;

/*
 * cpu manager
 */

typedef struct
{
  i_stats	stats;

  i_set		cpus;

  machdep_data(m_cpu);
}		m_cpu;

/*
 * the cpu architecture-dependent interface
 */

typedef struct
{
  t_error			(*cpu_show)(i_cpu);
  t_error			(*cpu_current)(i_cpu*);
  t_error			(*cpu_migrate)(i_task,
					       i_cpu);
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

t_error			cpu_show(i_cpu				id);

t_error			cpu_dump(void);

t_error			cpu_current(i_cpu*			cpuid);

t_error			cpu_select(i_cpu*			cpuid);

t_error			cpu_stats(i_cpu				cpuid,
				  t_timeslice			time);

t_error			cpu_balance(void);

t_error			cpu_migrate(i_task			tskid,
				    i_cpu			destination);

t_error			cpu_get(i_cpu				id,
				o_cpu**				o);

t_error			cpu_init(void);

t_error			cpu_clean(void);

void cpu1(void);

void cpu2(void);

i_thread	mk_thread(i_task tsk,
			  void *func);

void smp_test(void);


/*
 * eop
 */

#endif
