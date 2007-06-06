/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/cpu.h
 *
 * created       julien quintard   [sun jun  3 20:25:39 2007]
 * updated       julien quintard   [wed jun  6 16:41:56 2007]
 */

#ifndef GUARD_CORE_CPU
#define GUARD_CORE_CPU			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/id.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the identifier of a cpu object
 */

typedef t_id			i_cpu;

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/core.h>
#include <core/stats.h>
#include <core/set.h>
#include <core/task.h>
#include <core/error.h>

#include <arch/machdep/machdep.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * cpu object
 */

typedef struct
{
  i_cpu				cpuid;

  t_timeslice			efficiency;

  machdep_data(o_cpu);
}				o_cpu;

/*
 * cpu manager
 */

typedef struct
{
  i_stats			stats;

  i_set				cpus;

  machdep_data(m_cpu);
}				m_cpu;

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

void cpu3(void);

void cpu4(void);

i_thread	mk_thread(i_task tsk,
			  void *func);

void smp_test(void);


/*
 * eop
 */

#endif
