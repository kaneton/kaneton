/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.STABLE/kaneton/core/include/scheduler.h
 *
 * created       julien quintard   [wed jun  6 13:44:48 2007]
 * updated       julien quintard   [wed nov 24 23:02:15 2010]
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
 * ---------- macros ----------------------------------------------------------
 */

/*
 * the scheduler state.
 */
#define SCHEDULER_STATE_ENABLE		1
#define SCHEDULER_STATE_DISABLE		2

/*						   [block::macro::constants] */

/*
 * initial value for the scheduler quantum in milliseconds.
 */

#define SCHEDULER_QUANTUM		TIMER_DELAY

/*
 * the number of priority levels for the scheduler.
 */

#define SCHEDULER_N_PRIORITY_QUEUE	40

/*
 * timeslice constants.
 */

#define SCHEDULER_TIMESLICE_MIN		20
#define SCHEDULER_TIMESLICE_MAX		200
#define SCHEDULER_TIMESLICE_GRANULARITY	_scheduler->quantum

/*
 * scheduling priorities
 */

#define SCHEDULER_HPRIORITY		0
#define SCHEDULER_LPRIORITY		SCHEDULER_N_PRIORITY_QUEUE - 1

/*						[endblock::macro::constants] */

/*
 * ---------- macro functions -------------------------------------------------
 */

/*						   [block::macro::functions] */

/*
 * this macro function computes the  global priority of a thread.  the
 * global  priority is  an high  precision measurement  of  a thread's
 * priority. used for timeslice computation.
 */

#define SCHEDULER_COMPUTE_GLOBAL_PRIORITY(_thread_)			\
  ({									\
    o_thread*		oth;						\
    o_task*		otsk;						\
    t_priority		task_prior;					\
    t_priority		thread_prior;					\
									\
    if (thread_get((_thread_), &oth) != ERROR_OK)			\
      CORE_ESCAPE("unable to retrieve the thread object");		\
									\
    if (task_get(oth->task, &otsk) != ERROR_OK)				\
      CORE_ESCAPE("unable to retrieve the task object");		\
									\
    task_prior = ((otsk->priority - TASK_LPRIORITY_BACKGROUND) *	\
		  SCHEDULER_N_PRIORITY_QUEUE) /				\
      (TASK_HPRIORITY_KERNEL - TASK_LPRIORITY_BACKGROUND);		\
									\
    thread_prior = ((oth->priority - THREAD_LPRIORITY) *		\
		    SCHEDULER_N_PRIORITY_QUEUE) /			\
      (THREAD_HPRIORITY - THREAD_LPRIORITY);				\
									\
    (task_prior * thread_prior);					\
  })

/*
 * this  macro function  computes priority  queue index  of  a thread.
 * this is  a low precision  measurement of a thread's  priority. used
 * for queue index computation.
 */

#define SCHEDULER_COMPUTE_PRIORITY(_thread_)				\
  ({									\
    t_priority		global_prior;					\
									\
    global_prior = SCHEDULER_COMPUTE_GLOBAL_PRIORITY((_thread_));	\
									\
    (SCHEDULER_N_PRIORITY_QUEUE - global_prior /			\
        SCHEDULER_N_PRIORITY_QUEUE);					\
  })

/*
 * this macro compute a ceil timeslice taking account of granularity.
 */

#define SCHEDULER_SCALE_TIMESLICE(_t_)					\
  ((_t_) % SCHEDULER_TIMESLICE_GRANULARITY ?				\
   (_t_) + SCHEDULER_TIMESLICE_GRANULARITY -				\
   (_t_) % SCHEDULER_TIMESLICE_GRANULARITY				\
   : (_t_))

/*
 * this macro function computes the timeslice given by the kernel to a
 * thread based on its global priority.
 */

#define SCHEDULER_COMPUTE_TIMESLICE(_thread_)				\
  ({									\
    t_priority		global_prior;					\
    t_timeslice		t;						\
									\
    global_prior = SCHEDULER_COMPUTE_GLOBAL_PRIORITY((_thread_));	\
									\
    t = SCHEDULER_TIMESLICE_MIN;					\
    t += ((SCHEDULER_TIMESLICE_MAX - SCHEDULER_TIMESLICE_MIN) *		\
            global_prior) /						\
            (SCHEDULER_N_PRIORITY_QUEUE * SCHEDULER_N_PRIORITY_QUEUE);	\
									\
    SCHEDULER_SCALE_TIMESLICE(t);					\
  })

/*						[endblock::macro::functions] */

/*
 * ---------- types -----------------------------------------------------------
 */

/*							[block::t_scheduled] */

/*
 * a schedule queue element.
 */

typedef struct
{
  i_thread			thread;

  t_timeslice			timeslice;
}				o_scheduled;

/*						     [endblock::t_scheduled] */

/*
 * cpu scheduling environment for one processor.
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
 * scheduler manager
 */

typedef struct
{
  t_quantum			quantum;

  i_thread			idle;

  i_set				cpus;

  machine_data(m_scheduler);
}				m_scheduler;

/*
 * the scheduler architecture-dependent interface
 */

typedef struct
{
  t_error			(*scheduler_state)(t_state);
  t_error			(*scheduler_quantum)(t_quantum);
  t_error			(*scheduler_yield)(void);
  t_error			(*scheduler_add)(i_thread);
  t_error			(*scheduler_remove)(i_thread);
  t_error			(*scheduler_update)(i_thread);
  t_error			(*scheduler_elect)(void);
  t_error			(*scheduler_initialize)(void);
  t_error			(*scheduler_clean)(void);
}				d_scheduler;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/scheduler/scheduler.c
 */

/*
 * ../../core/scheduler/scheduler.c
 */

t_error			scheduler_dump(void);

t_error			scheduler_state(t_state			state);

t_error			scheduler_quantum(t_quantum		quantum);

t_error			scheduler_yield(void);

t_error			scheduler_current(i_thread*			thread);

t_error			scheduler_elect(void);

t_error			scheduler_get(o_scheduler**		scheduler);

t_error			scheduler_add(i_thread			id);

t_error			scheduler_remove(i_thread		id);

t_error			scheduler_update(i_thread			thread);

t_error			scheduler_initialize(void);

t_error			scheduler_clean(void);


/*
 * eop
 */

#endif
