/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/sched.h
 *
 * created       julien quintard   [wed jun  6 13:44:48 2007]
 * updated       julien quintard   [thu jun  7 00:58:10 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file supposes the fundamental types are defined.
 */

#ifndef CORE_SCHED_H
#define CORE_SCHED_H			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/sched.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * this is thread state.
 */

#define SCHED_STATE_RUN			0
#define SCHED_STATE_STOP		1
#define SCHED_STATE_ZOMBIE		2
#define SCHED_STATE_BLOCK		3

/*
 * initial value for the scheduler quantum in milliseconds.
 */

#define SCHED_QUANTUM_INIT		5

/*
 * the number of priority levels for the scheduler.
 */

#define SCHED_N_PRIORITY_QUEUE		40

/*
 * timeslice constants.
 */

#define SCHED_TIMESLICE_MIN		20
#define SCHED_TIMESLICE_MAX		200
#define SCHED_TIMESLICE_GRANULARITY	sched->quantum

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro function computes the  global priority of a thread.  the
 * global  priority is  an high  precision measurement  of  a thread's
 * priority. used for timeslice computation.
 */

#define COMPUTE_GLOBAL_PRIORITY(_thread_)				\
  ({									\
    o_thread*		oth;						\
    o_task*		otsk;						\
    t_prior		task_prior;					\
    t_prior		thread_prior;					\
									\
    if (thread_get((_thread_), &oth) != ERROR_NONE)			\
      SCHED_LEAVE(sched, ERROR_UNKNOWN);				\
									\
    if (task_get(oth->taskid, &otsk) != ERROR_NONE)			\
      SCHED_LEAVE(sched, ERROR_UNKNOWN);				\
									\
    task_prior = ((otsk->prior - TASK_LPRIOR_BACKGROUND) *		\
		  SCHED_N_PRIORITY_QUEUE) /				\
      (TASK_HPRIOR_CORE - TASK_LPRIOR_BACKGROUND);			\
									\
    thread_prior = ((oth->prior - THREAD_LPRIOR) *			\
		    SCHED_N_PRIORITY_QUEUE) /				\
      (THREAD_HPRIOR - THREAD_LPRIOR);					\
									\
    (task_prior * thread_prior);					\
  })

/*
 * this  macro function  computes priority  queue index  of  a thread.
 * this is  a low precision  measurement of a thread's  priority. used
 * for queue index computation.
 */

#define COMPUTE_PRIORITY(_thread_)					\
  ({									\
    t_prior		global_prior;					\
									\
    global_prior = COMPUTE_GLOBAL_PRIORITY((_thread_));			\
    (SCHED_N_PRIORITY_QUEUE - global_prior / SCHED_N_PRIORITY_QUEUE);	\
  })

/*
 * this macro compute a ceil timeslice taking account of granularity.
 */

#define SCALE_TIMESLICE(_t_)						\
  ((_t_) % SCHED_TIMESLICE_GRANULARITY ?				\
   (_t_) + SCHED_TIMESLICE_GRANULARITY -				\
   (_t_) % SCHED_TIMESLICE_GRANULARITY					\
   : (_t_))

/*
 * this macro function computes the timeslice given by the kernel to a
 * thread based on its global priority.
 */

#define COMPUTE_TIMESLICE(_thread_)					\
  ({									\
    t_prior		global_prior;					\
    t_timeslice		t;						\
									\
    global_prior = COMPUTE_GLOBAL_PRIORITY((_thread_));			\
									\
    t = SCHED_TIMESLICE_MIN;						\
    t += ((SCHED_TIMESLICE_MAX - SCHED_TIMESLICE_MIN) * global_prior) /	\
      (SCHED_N_PRIORITY_QUEUE * SCHED_N_PRIORITY_QUEUE);		\
    SCALE_TIMESLICE(t);							\
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
  i_stats			stats;

  t_quantum			quantum;

  i_set				cpus;

  machdep_data(m_sched);
}				m_sched;

/*
 * the scheduler architecture-dependent interface
 */

typedef struct
{
  t_error			(*sched_quantum)(t_quantum);
  t_error			(*sched_yield)(i_cpu);
  t_error			(*sched_switch)(i_thread);
  t_error			(*sched_add)(i_thread);
  t_error			(*sched_remove)(i_thread);
  t_error			(*sched_update)(i_thread);
  t_error			(*sched_init)(void);
  t_error			(*sched_clean)(void);
}				d_sched;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define SCHED_CHECK(_sched_)						\
  {									\
    if ((_sched_) == NULL)						\
      return (ERROR_UNKNOWN);						\
  }

/*
 * enter
 */

#define SCHED_ENTER(_sched_)						\
  {									\
    SCHED_CHECK((_sched_));						\
									\
    STATS_BEGIN((_sched_)->stats);					\
  }

/*
 * leave
 */

#define SCHED_LEAVE(_sched_, _error_)					\
  {									\
    STATS_END((_sched_)->stats, (_error_));				\
    									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/sched/sched.c
 */

/*
 * ../../core/sched/sched.c
 */

t_error			sched_dump(void);

t_error			sched_quantum(t_quantum			quantum);

t_error			sched_yield(i_cpu			cpuid);

t_error			sched_current(i_thread*			thread);

t_error			sched_switch(void);

t_error			sched_add(i_thread			thread);

t_error			sched_remove(i_thread			thread);

t_error			sched_update(i_thread			thread);

t_error			sched_init(void);

t_error			sched_clean(void);


/*
 * eop
 */

#endif
