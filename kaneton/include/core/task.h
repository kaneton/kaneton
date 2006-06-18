/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/task.h
 *
 * created       julien quintard   [tue nov 29 21:32:05 2005]
 * updated       matthieu bucchianeri   [sun jun 18 18:50:09 2006]
 */

#ifndef CORE_TASK_H
#define CORE_TASK_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <core/id.h>
#include <core/types.h>
#include <core/wait.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * classes
 */

#define TASK_CLASS_CORE		(1 << 0)
#define TASK_CLASS_DRIVER	(1 << 1)
#define TASK_CLASS_SERVICE	(1 << 2)
#define TASK_CLASS_PROGRAM	(1 << 3)

/*
 * behaviours
 */

#define TASK_BEHAV_CORE		(1 << 0)
#define TASK_BEHAV_REALTIME	(1 << 1)
#define TASK_BEHAV_INTERACTIVE	(1 << 2)
#define TASK_BEHAV_TIMESHARING	(1 << 3)
#define TASK_BEHAV_BACKGROUND	(1 << 4)

/*
 * priorities
 */

#define TASK_PRIOR_CORE		230
#define TASK_HPRIOR_CORE	250
#define TASK_LPRIOR_CORE	210

#define TASK_PRIOR_REALTIME	190
#define TASK_HPRIOR_REALTIME	210
#define TASK_LPRIOR_REALTIME	170

#define TASK_PRIOR_INTERACTIVE	150
#define TASK_HPRIOR_INTERACTIVE	170
#define TASK_LPRIOR_INTERACTIVE	130

#define TASK_PRIOR_TIMESHARING	90
#define TASK_HPRIOR_TIMESHARING	130
#define TASK_LPRIOR_TIMESHARING	50

#define TASK_PRIOR_BACKGROUND	30
#define TASK_HPRIOR_BACKGROUND	50
#define TASK_LPRIOR_BACKGROUND	10

/*
 * init sizes for the array data structure set
 */

#define TASK_THREADS_INITSZ	0x1
#define TASK_WAITS_INITSZ	0x1

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * task object
 */

typedef struct
{
  i_task			tskid;

  i_task			parent;
  i_set				children;

  t_class			class;
  t_behav			behav;
  t_prior			prior;

  i_as				asid;
  i_set				threads;

  t_state			sched;

  i_set				waits;
  t_wait			wait;

  machdep_data(o_task);
}				o_task;

/*
 * task manager
 */

typedef struct
{
  o_id				id;

  i_stats			stats;

  i_set				tasks;

  machdep_data(m_task);
}				m_task;

/*
 * waitlist
 */

typedef struct
{
  union
  {
    i_task			task;
    i_thread			thread;
  }				u;

  t_opts			opts;
}				o_waitfor;

/*
 * the task architecture dependent interface
 */

typedef struct
{
  t_error			(*task_show)(i_task);
  t_error			(*task_clone)(i_task,
					      i_task*);
  t_error			(*task_reserve)(t_class,
						t_behav,
						t_prior,
						i_task*);
  t_error			(*task_release)(i_task);
  t_error			(*task_priority)(i_task,
						 t_prior);
  t_error			(*task_run)(i_task);
  t_error			(*task_stop)(i_task);
  t_error			(*task_exit)(i_task);
  t_error			(*task_wait)(i_task,
					     t_opts,
					     t_wait*);
  t_error			(*task_init)(void);
  t_error			(*task_clean)(void);
}				d_task;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define TASK_CHECK(_task_)						\
  {									\
    if ((_task_) == NULL)						\
      return (ERROR_UNKNOWN);						\
  }

/*
 * enter
 */

#define TASK_ENTER(_task_)						\
  {									\
    TASK_CHECK((_task_));						\
									\
    STATS_BEGIN((_task_)->stats);					\
  }

/*
 * leave
 */

#define TASK_LEAVE(_task_, _error_)					\
  {									\
    STATS_END((_task_)->stats, (_error_));				\
									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/task/task.c
 */

/*
 * ../../core/task/task.c
 */

t_error			task_show(i_task			id);

t_error			task_dump(void);

t_error			task_clone(i_task			old,
				   i_task*			new);

t_error			task_reserve(t_class			class,
				     t_behav			behav,
				     t_prior			prior,
				     i_task*			id);

t_error			task_release(i_task			id);

t_error			task_priority(i_task			id,
				      t_prior			prior);

t_error			task_run(i_task				id);

t_error			task_stop(i_task			id);

t_error			task_exit(i_task			id);

t_error			task_wait(i_task			id,
				  t_opts			opts,
				  t_wait*			wait);

t_error			task_get(i_task				id,
				 o_task**			o);

t_error			task_init(void);

t_error			task_clean(void);


/*
 * eop
 */

#endif
