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
 * updated       julien quintard   [mon dec 13 18:06:51 2010]
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

/*						   [block::macro::constants] */

/*
 * initial value for the scheduler quantum in milliseconds.
 */

#define SCHEDULER_QUANTUM		TIMER_DELAY

/*
 * the number of priorities i.e the number of queues.
 */

#define SCHEDULER_NPRIORITIES		40

/*
 * timeslice bounds.
 */

#define SCHEDULER_TIMESLICE_HIGH	200
#define SCHEDULER_TIMESLICE_LOW		20

/*
 * the timeslice granularity.
 */

#define SCHEDULER_GRANULARITY		_scheduler->quantum

/*
 * scheduling priorities.
 */

#define SCHEDULER_PRIORITY_HIGH		0
#define SCHEDULER_PRIORITY_LOW		SCHEDULER_NPRIORITIES - 1

/*						[endblock::macro::constants] */

/*
 * ---------- macro functions -------------------------------------------------
 */

/*						   [block::macro::functions] */

/*
 * this macro-function computes the thread's character i.e global priority.
 * the character is a high precision measurement of a thread's priority which
 * is used for timeslice computation.
 */

#define SCHEDULER_CHARACTER(_thread_)					\
  (									\
    {									\
      o_thread*		_oth_;						\
      o_task*		_otsk_;						\
      t_priority	_task_prior_;					\
      t_priority	_thread_prior_;					\
									\
      if (thread_get((_thread_), &_oth_) != ERROR_OK)			\
        CORE_ESCAPE("unable to retrieve the thread object");		\
									\
      if (task_get(_oth_->task, &_otsk_) != ERROR_OK)			\
        CORE_ESCAPE("unable to retrieve the task object");		\
									\
      _task_prior_ =							\
	((_otsk_->priority - TASK_PRIORITY_BACKGROUND_LOW) *		\
	 SCHEDULER_NPRIORITIES) /					\
	(TASK_PRIORITY_KERNEL_HIGH - TASK_PRIORITY_BACKGROUND_LOW);	\
									\
      _thread_prior_ =							\
	((_oth_->priority - THREAD_PRIORITY_LOW) *			\
	 SCHEDULER_NPRIORITIES) /					\
	(THREAD_PRIORITY_HIGH - THREAD_PRIORITY_LOW);			\
									\
      (_task_prior_ * _thread_prior_);					\
    }									\
  )

/*
 * this macro-function computes the priority queue index for a give
 * thread. this is a low precision measurement of a thread's priority
 * which is used for queue index computation.
 */

#define SCHEDULER_PRIORITY(_thread_)					\
  (									\
    {									\
      t_priority	_character_;					\
									\
      _character_ = SCHEDULER_CHARACTER((_thread_));			\
									\
      (SCHEDULER_NPRIORITIES - _character_ / SCHEDULER_NPRIORITIES);	\
    }									\
  )

/*
 * this macro-function computes a ceil timeslice taking into account
 * the granularity that may have been changed.
 */

#define SCHEDULER_SCALE(_t_)						\
  ((_t_) % SCHEDULER_GRANULARITY ?					\
   (_t_) + SCHEDULER_GRANULARITY -					\
   (_t_) % SCHEDULER_GRANULARITY					\
   : (_t_))

/*
 * this macro-function computes the timeslice given by the kernel to a
 * thread based on its character i.e global priority.
 */

#define SCHEDULER_TIMESLICE(_thread_)					\
  (									\
    {									\
      t_priority		_character_;				\
      t_timeslice		_timeslice_;				\
									\
      _character_ = SCHEDULER_CHARACTER((_thread_));			\
									\
      _timeslice_ = SCHEDULER_TIMESLICE_LOW;				\
      _timeslice_ +=							\
	((SCHEDULER_TIMESLICE_HIGH - SCHEDULER_TIMESLICE_LOW) *		\
	 _character_) / (SCHEDULER_NPRIORITIES * SCHEDULER_NPRIORITIES);\
									\
      SCHEDULER_SCALE(_timeslice_);					\
    }									\
  )

/*						[endblock::macro::functions] */

/*
 * ---------- types -----------------------------------------------------------
 */

/*							[block::t_scheduled] */

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

/*						     [endblock::t_scheduled] */

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
  /*						     [block::o_scheduler::1] */

  i_cpu				cpu;

  /*						  [endblock::o_scheduler::1] */

  i_thread			thread;
  /*						     [block::o_scheduler::2] */
  t_timeslice			timeslice;
  t_priority			priority;

  i_set				active;
  i_set				expired;

  t_state			state;

  /*						  [endblock::o_scheduler::2] */

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
