/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...kaneton/kaneton/core/include/scheduler.h
 *
 * created       julien quintard   [wed jun  6 13:44:48 2007]
 * updated       julien quintard   [sat feb  5 16:59:18 2011]
 */

#ifndef CORE_SCHEDULER_H
#define CORE_SCHEDULER_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/error.h>
#include <core/id.h>

#include <machine/machine.h>

/*
 * ---------- algorithms ------------------------------------------------------
 */

/*
 * the supported scheduler algorithms.
 */

#define SCHEDULER_ALGORITHM_MFQ		(1 << 0)

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * the scheduler state either started or stopped.
 */

#define SCHEDULER_STATE_START		1
#define SCHEDULER_STATE_STOP		2

/*
 * initial value for the scheduler quantum in milliseconds.
 */

#define SCHEDULER_QUANTUM		TIMER_DELAY

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the scheduler object which managers thread candidates for a
 * given CPU specified by _cpu_.
 *
 * the _thread_ attribute represents the currently scheduled thread
 * which operates at the priority _priority_. this thread stil has
 * _timeslice_ milliseconds of execution time before a context switch
 * occurs.
 *
 * the _state_ attribute represents the scheduler's current state, either
 * started or stopped.
 */

typedef struct
{
  i_cpu				cpu;

  i_thread			thread;
  t_timeslice			timeslice;
  t_priority			priority;

  /* FIXME[code to complete] */

  t_state			state;

  machine_data(o_scheduler);
}				o_scheduler;

/*
 * the scheduler manager's structure which contains the quantum _quantum_
 * i.e the smaller unit of execution time, the idle thread's identifier _idle_
 * and the sets of schedulers, one scheduler per CPU.
 */

typedef struct
{
  t_quantum			quantum;

  i_thread			idle;

  i_set				schedulers;

  machine_data(m_scheduler);
}				m_scheduler;

/*
 * the scheduler dispatcher.
 */

typedef struct
{
  t_error			(*scheduler_show)(i_cpu,
						  mt_margin);
  t_error			(*scheduler_dump)(void);
  t_error			(*scheduler_start)(i_cpu);
  t_error			(*scheduler_stop)(i_cpu);
  t_error			(*scheduler_quantum)(t_quantum);
  t_error			(*scheduler_yield)(void);
  t_error			(*scheduler_elect)(void);
  t_error			(*scheduler_add)(i_thread);
  t_error			(*scheduler_remove)(i_thread);
  t_error			(*scheduler_update)(i_thread);
  t_error			(*scheduler_initialize)(void);
  t_error			(*scheduler_clean)(void);
}				d_scheduler;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/scheduler/scheduler-mfq.c
 */

/*
 * ../../core/scheduler/scheduler-mfq.c
 */

t_error			scheduler_show(i_cpu			id,
				       mt_margin		margin);

t_error			scheduler_dump(void);

t_error			scheduler_start(i_cpu			id);

t_error			scheduler_stop(i_cpu			id);

t_error			scheduler_quantum(t_quantum		quantum);

t_error			scheduler_yield(void);

t_error			scheduler_elect(void);

t_error			scheduler_add(i_thread			id);

t_error			scheduler_remove(i_thread		id);

t_error			scheduler_update(i_thread		id);

t_error			scheduler_exist(i_cpu			id);

t_error			scheduler_get(i_cpu			id,
				      o_scheduler**		object);

t_error			scheduler_current(o_scheduler**		scheduler);

t_error			scheduler_initialize(void);

t_error			scheduler_clean(void);


/*
 * eop
 */

#endif
