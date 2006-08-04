/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/sched.h
 *
 * created       matthieu bucchianeri   [sat jun  3 22:34:42 2006]
 * updated       matthieu bucchianeri   [thu aug  3 19:34:33 2006]
 */

#ifndef CORE_SCHED_H
#define CORE_SCHED_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <core/id.h>
#include <core/types.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define SCHED_STATE_RUN		0
#define SCHED_STATE_STOP	1
#define SCHED_STATE_ZOMBIE	2

#define SCHED_QUANTUM_INIT	5 // ms

#define SCHED_N_PRIORITY_QUEUE	40

#define SCHED_TIMESLICE_MIN		5
#define SCHED_TIMESLICE_MAX		200
#define SCHED_TIMESLICE_GRANULARITY	sched->quantum

/*
 * ---------- macro functions--------------------------------------------------
 */

/*
 * this macro function computes the  global priority of a thread.  the
 * global  priority is  an high  precision measurement  of  a thread's
 * priority.
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
 * this is a low precision measurement of a thread's priority.
 */

#define COMPUTE_PRIORITY(_thread_)					\
  ({									\
    t_prior		global_prior;					\
									\
    global_prior = COMPUTE_GLOBAL_PRIORITY((_thread_));			\
    (SCHED_N_PRIORITY_QUEUE - global_prior / SCHED_N_PRIORITY_QUEUE);	\
  })

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
    (t % SCHED_TIMESLICE_GRANULARITY ?					\
     t + SCHED_TIMESLICE_GRANULARITY - t % SCHED_TIMESLICE_GRANULARITY	\
     : t);								\
  })

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * a schedule queue element.
 */

typedef struct
{
  i_thread	thread;
  t_timeslice	timeslice;
}		t_scheduled;

/*
 * scheduler manager
 */

typedef struct
{
  i_stats	stats;

  t_quantum	quantum;

  i_thread	current;
  t_timeslice	timeslice;
  t_prior	prio;
  t_scheduled*	entity;

  i_set		active;
  i_set		expired;

  machdep_data(m_sched);
}		m_sched;

/*
 * the scheduler architecture-dependent interface
 */

typedef struct
{
  t_error			(*sched_quantum)(t_quantum);
  t_error			(*sched_yield)(void);
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
    if ((_error_) != ERROR_NONE)					\
      cons_msg('!', "scheduler error @ %s:%d\n", __FUNCTION__, __LINE__); \
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

t_error			sched_yield(void);

t_error			sched_current(i_thread*			thread);

t_error			sched_switch(void);

t_error			sched_add(i_thread			thread);

t_error			sched_remove(i_thread			thread);

t_error			sched_update(i_thread			thread);

t_error			sched_init(void);

t_error			sched_clean(void);

void _fun1();

void _fun2();

void _fun3();

void sched_test_add_thread(void *func, t_prior p);

void sched_test();


/*
 * eop
 */

#endif
