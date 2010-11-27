/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.TETON/kaneton/core/include/cpu.h
 *
 * created       julien quintard   [sun jun  3 20:25:39 2007]
 * updated       julien quintard   [sat nov 27 06:11:21 2010]
 */

#ifndef CORE_CPU_H
#define CORE_CPU_H			1

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
 * cpu object
 */

typedef struct
{
  i_cpu				id;

  t_timeslice			efficiency;

  machine_data(o_cpu);
}				o_cpu;

/*
 * cpu manager
 */

typedef struct
{
  t_setsz			ncpus;

  i_set				cpus;

  machine_data(m_cpu);
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
  t_error			(*cpu_initialize)(void);
  t_error			(*cpu_clean)(void);
}				d_cpu;

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

t_error			cpu_current(i_cpu*			id);

t_error			cpu_multiprocessor(void);

t_error			cpu_select(i_cpu*			id);

t_error			cpu_statistics(i_cpu			id,
				       t_timeslice		time);

t_error			cpu_balance(void);

t_error			cpu_migrate(i_task			task,
				    i_cpu			cpu);

t_error			cpu_exist(i_cpu				id);

t_error			cpu_get(i_cpu				id,
				o_cpu**				o);

t_error			cpu_initialize(void);

t_error			cpu_clean(void);


/*
 * eop
 */

#endif
