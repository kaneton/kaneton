/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/include/scheduler.h
 *
 * created       julien quintard   [wed jun  6 13:44:48 2007]
 * updated       matthieu bucchianeri   [mon aug  6 22:53:24 2007]
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
 * this is thread state.
 */

#define SCHEDULER_STATE_RUN		0
#define SCHEDULER_STATE_STOP		1
#define SCHEDULER_STATE_ZOMBIE		2
#define SCHEDULER_STATE_BLOCK		3

/*
 * initial value for the scheduler quantum in milliseconds.
 */

#define SCHEDULER_QUANTUM_INIT		TIMER_MS_PER_TICK

/*
 * the number of priority levels for the scheduler.
 */

#define SCHEDULER_N_PRIORITY_QUEUE	40

/*
 * timeslice constants.
 */

#define SCHEDULER_TIMESLICE_MIN		20
#define SCHEDULER_TIMESLICE_MAX		200
#define SCHEDULER_TIMESLICE_GRANULARITY	scheduler->quantum

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro function computes the  global priority of a thread.  the
 * global  priority is  an high  precision measurement  of  a thread's
 * priority. used for timeslice computation.
 */

#define SCHEDULER_COMPUTE_GLOBAL_PRIORITY(_thread_)			\
  ({									\
    o_thread*		oth;						\
    o_task*		otsk;						\
    t_prior		task_prior;					\
    t_prior		thread_prior;					\
									\
    if (thread_get((_thread_), &oth) != ERROR_NONE)			\
      SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);			\
									\
    if (task_get(oth->taskid, &otsk) != ERROR_NONE)			\
      SCHEDULER_LEAVE(scheduler, ERROR_UNKNOWN);			\
									\
    task_prior = ((otsk->prior - TASK_LPRIOR_BACKGROUND) *		\
		  SCHEDULER_N_PRIORITY_QUEUE) /				\
      (TASK_HPRIOR_CORE - TASK_LPRIOR_BACKGROUND);			\
									\
    thread_prior = ((oth->prior - THREAD_LPRIOR) *			\
		    SCHEDULER_N_PRIORITY_QUEUE) /			\
      (THREAD_HPRIOR - THREAD_LPRIOR);					\
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
    t_prior		global_prior;					\
									\
    global_prior = SCHEDULER_COMPUTE_GLOBAL_PRIORITY((_thread_));	\
    (SCHEDULER_N_PRIORITY_QUEUE - global_prior / SCHEDULER_N_PRIORITY_QUEUE);	\
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
    t_prior		global_prior;					\
    t_timeslice		t;						\
									\
    global_prior = SCHEDULER_COMPUTE_GLOBAL_PRIORITY((_thread_));	\
									\
    t = SCHEDULER_TIMESLICE_MIN;					\
    t += ((SCHEDULER_TIMESLICE_MAX - SCHEDULER_TIMESLICE_MIN) * global_prior) /	\
      (SCHEDULER_N_PRIORITY_QUEUE * SCHEDULER_N_PRIORITY_QUEUE);	\
    SCHEDULER_SCALE_TIMESLICE(t);					\
  })

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * a schedule queue element.
 */

typedef struct
{
  i_thread			thread;
  t_timeslice			timeslice;
}				t_scheduled;

/*
 * cpu scheduling environment.
 */

typedef struct
{
  i_cpu				cpuid;

  i_thread			current;
  t_timeslice			timeslice;
  t_prior			prio;

  i_set				active;
  i_set				expired;
}				t_cpu_sched;

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
  t_error			(*scheduler_quantum)(t_quantum);
  t_error			(*scheduler_yield)(i_cpu);
  t_error			(*scheduler_switch)(i_thread);
  t_error			(*scheduler_add)(i_thread);
  t_error			(*scheduler_remove)(i_thread);
  t_error			(*scheduler_update)(i_thread);
  t_error			(*scheduler_initialize)(void);
  t_error			(*scheduler_clean)(void);
}				d_scheduler;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define SCHEDULER_CHECK(_scheduler_)					\
  {									\
    if ((_scheduler_) == NULL)						\
      return (ERROR_UNKNOWN);						\
  }

/*
 * enter
 */

#define SCHEDULER_ENTER(_scheduler_)					\
  {									\
    SCHEDULER_CHECK((_scheduler_));						\
  }

/*
 * leave
 */

#define SCHEDULER_LEAVE(_scheduler_, _error_)				\
  {									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/scheduler/scheduler.c
 */

/*
 * ../../core/scheduler/scheduler.c
 */

t_error			scheduler_dump(void);

t_error			scheduler_quantum(t_quantum			quantum);

t_error			scheduler_yield(i_cpu			cpuid);

t_error			scheduler_current(i_thread*			thread);

t_error			scheduler_switch(void);

t_error			scheduler_add(i_thread			thread);

t_error			scheduler_remove(i_thread			thread);

t_error			scheduler_update(i_thread			thread);

t_error			scheduler_initialize(void);

t_error			scheduler_clean(void);


/*
 * eop
 */

#endif
