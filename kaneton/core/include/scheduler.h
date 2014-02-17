/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/scheduler.h
 *
 * created       julien quintard   [wed jun  6 13:44:48 2007]
 * updated       julien quintard   [sat feb  5 10:35:42 2011]
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
 * the number of priorities i.e the number of queues.
 */

#define SCHEDULER_NPRIORITIES		60

/*
 * timeslice bounds.
 */

#define SCHEDULER_TIMESLICE_HIGH	250
#define SCHEDULER_TIMESLICE_LOW		10

/*
 * the timeslice granularity.
 */

#define SCHEDULER_GRANULARITY		_scheduler.quantum

/*
 * scheduling priorities.
 */

#define SCHEDULER_PRIORITY_HIGH		SCHEDULER_NPRIORITIES - 1
#define SCHEDULER_PRIORITY_LOW		0

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro-function computes the thread's high precision character
 * by taking into account both the task's and thread's priorities.
 */

#define SCHEDULER_CHARACTER(_id_)					\
  (									\
    {									\
      o_task*		_task_;						\
      o_thread*		_thread_;					\
									\
      assert(thread_get((_id_), &_thread_) == STATUS_OK);		\
      assert(task_get(_thread_->task, &_task_) == STATUS_OK);		\
									\
      ((_task_->priority - TASK_PRIORITY_BACKGROUND_LOW) *		\
       (_thread_->priority - THREAD_PRIORITY_LOW));			\
    }									\
  )

/*
 * this macro-function computes the priority for a giver thread. this
 * is a low precision measurement of a thread's priority which is used
 * for locating the proper scheduling queue.
 *
 * indeed, while the character lies in a large range, it is then
 * reduced within the range [SCHEDULER_PRIORITY_LOW, SCHEDULER_PRIORITY_HIGH].
 */

#define SCHEDULER_PRIORITY(_thread_)					\
  (									\
    {									\
      t_priority	_character_;					\
									\
      _character_ = SCHEDULER_CHARACTER((_thread_));			\
									\
      SCHEDULER_PRIORITY_LOW +						\
	((_character_ *							\
	  (SCHEDULER_PRIORITY_HIGH - SCHEDULER_PRIORITY_LOW)) /		\
	 ((TASK_PRIORITY_KERNEL_HIGH - TASK_PRIORITY_BACKGROUND_LOW) *	\
	  (THREAD_PRIORITY_HIGH - THREAD_PRIORITY_LOW)));		\
    }									\
  )

/*
 * this macro-function takes a number of milliseconds and turns it
 * into a valid timeslice according to the scheduler quantum.
 *
 * for example, with a quantum of 25ms and a given number of 264 milliseconds,
 * this macro-function would return 275ms, the upper rounded number.
 */

#define SCHEDULER_SCALE(_timeslice_)					\
  ((((_timeslice_) % SCHEDULER_GRANULARITY) != 0) ?			\
   (((_timeslice_) + SCHEDULER_GRANULARITY) -				\
    (_timeslice_) % SCHEDULER_GRANULARITY)				\
   : (_timeslice_))

/*
 * this macro-function computes the timeslice given by the kernel to a
 * thread based on its character.
 *
 * the character basically returns task->priority * thread->priority.
 *
 * this number is then turned into a timeslice i.e within the timeslice
 * range [SCHEDULER_TIMESLICE_LOW, SCHEDULER_TIMESLICE_HIGH].
 *
 * finally, the timeslice is scaled i.e rounded up in order to fit the
 * scheduling unit known as the quantum.
 */

#define SCHEDULER_TIMESLICE(_thread_)					\
  (									\
    {									\
      t_priority		_character_;				\
      t_timeslice		_timeslice_;				\
									\
      _character_ = SCHEDULER_CHARACTER((_thread_));			\
									\
      _timeslice_ =							\
	SCHEDULER_TIMESLICE_LOW +					\
	((_character_ *							\
	  (SCHEDULER_TIMESLICE_HIGH - SCHEDULER_TIMESLICE_LOW)) /	\
	 ((TASK_PRIORITY_KERNEL_HIGH - TASK_PRIORITY_BACKGROUND_LOW) *	\
	  (THREAD_PRIORITY_HIGH - THREAD_PRIORITY_LOW)));		\
									\
      SCHEDULER_SCALE(_timeslice_);					\
    }									\
  )

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * this structure represents a scheduler's queue element.
 */

typedef struct
{
  i_thread			thread;

  t_timeslice			timeslice;
}				o_scheduler_candidate;

/*
 * this structure represents a queue of threads for a specific priority.
 *
 * note that identifiers must be 64-bit numbers but a priority is only
 * 32-bit long. therefore, priorities are converted into identifiers.
 */

typedef struct
{
  t_id				id;

  t_priority			priority;

  i_set				candidates;
}				o_scheduler_queue;

/*
 * the scheduler object which managers thread candidates for a
 * given CPU specified by _cpu_.
 *
 * the _thread_ attribute represents the currently scheduled thread
 * which operates at the priority _priority_. this thread stil has
 * _timeslice_ milliseconds of execution time before a context switch
 * occurs.
 *
 * the _active_ and _expired_ sets contain the scheduler candidates being
 * active i.e ready to be scheduled with a timeslice higher than zero or
 * expired i.e having been scheduled until their timeslice reached zero.
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

  i_set				active;
  i_set				expired;

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
  t_status		(*scheduler_show)(i_cpu,
						  mt_margin);
  t_status		(*scheduler_dump)(void);
  t_status		(*scheduler_start)(i_cpu);
  t_status		(*scheduler_stop)(i_cpu);
  t_status		(*scheduler_quantum)(t_quantum);
  t_status		(*scheduler_yield)(void);
  t_status		(*scheduler_elect)(void);
  t_status		(*scheduler_add)(i_thread);
  t_status		(*scheduler_remove)(i_thread);
  t_status		(*scheduler_update)(i_thread);
  t_status		(*scheduler_initialize)(void);
  t_status		(*scheduler_clean)(void);
}				d_scheduler;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/scheduler/scheduler-mfq.c
 */

/*
 * ../../core/scheduler/scheduler-mfq.c
 */

t_status		scheduler_show(i_cpu			id,
				       mt_margin		margin);

t_status		scheduler_dump(void);

t_status		scheduler_start(i_cpu			id);

t_status		scheduler_stop(i_cpu			id);

t_status		scheduler_quantum(t_quantum		quantum);

t_status		scheduler_yield(void);

t_status		scheduler_elect(void);

t_status		scheduler_add(i_thread			id);

t_status		scheduler_remove(i_thread		id);

t_status		scheduler_update(i_thread		id);

t_bool			scheduler_exist(i_cpu			id);

t_status		scheduler_get(i_cpu			id,
				      o_scheduler**		object);

t_status		scheduler_current(o_scheduler**		scheduler);

t_status		scheduler_initialize(void);

t_status		scheduler_clean(void);


/*
 * eop
 */

#endif
