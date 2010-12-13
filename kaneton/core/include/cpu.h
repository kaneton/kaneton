/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/cpu.h
 *
 * created       julien quintard   [sun jun  3 20:25:39 2007]
 * updated       julien quintard   [fri dec 10 21:17:08 2010]
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
 * the structure for a CPU object.
 */

typedef struct
{
  i_cpu				id;

  t_timeslice			efficiency;

  machine_data(o_cpu);
}				o_cpu;

/*
 * the CPU manager.
 */

typedef struct
{
  i_set				cpus;

  machine_data(m_cpu);
}				m_cpu;

/*
 * the CPU dispatcher.
 */

typedef struct
{
  t_error			(*cpu_show)(i_cpu,
					    mt_margin);
  t_error			(*cpu_dump)(void);
  t_error			(*cpu_current)(i_cpu*);
  t_error			(*cpu_select)(i_cpu*);
  t_error			(*cpu_update)(i_cpu,
					      t_timeslice);
  t_error			(*cpu_balance)(void);
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

t_error			cpu_show(i_cpu				id,
				 mt_margin			margin);

t_error			cpu_dump(void);

t_error			cpu_current(i_cpu*			id);

t_error			cpu_multiprocessor(void);

t_error			cpu_select(i_cpu*			id);

t_error			cpu_update(i_cpu			id,
				   t_timeslice			timeslice);

t_error			cpu_balance(void);

t_error			cpu_migrate(i_task			task,
				    i_cpu			cpu);

t_error			cpu_exist(i_cpu				id);

t_error			cpu_get(i_cpu				id,
				o_cpu**				object);

t_error			cpu_initialize(void);

t_error			cpu_clean(void);


/*
 * eop
 */

#endif
