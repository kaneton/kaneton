/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/core/include/kaneton/task.h
 *
 * created       julien quintard   [tue nov 29 21:32:05 2005]
 * updated       julien quintard   [sat dec 10 19:11:42 2005]
 */

#ifndef KANETON_TASK_H
#define KANETON_TASK_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>

#include <kaneton/id.h>
#include <kaneton/types.h>

/*
 * ---------- defines ---------------------------------------------------------
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
  t_tskid			tskid;

  t_tskid			parent;

  t_class			class;
  t_behav			behav;
  t_prior			prior;

  t_asid			asid;
  t_setid			threads;

  t_state			sched;

  /* XXX t_wait			status; */

  t_setid			waits;

  machdep_data(o_task);
}				o_task;

/*
 * task manager
 */

typedef struct
{
  o_id				id;

  t_staid			stats;

  t_setid			container;

  machdep_data(m_task);
}				m_task;

/*
 * ---------- macros ----------------------------------------------------------
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
 *      ../../kaneton/task/task.c
 */

/*
 * ../../kaneton/task/task.c
 */

t_error			task_show(t_tskid			tskid);

t_error			task_dump(void);

t_error			task_clone(t_tskid			old,
				   t_tskid*			new);

t_error			task_reserve(t_class			class,
				     t_behav			behav,
				     t_prior			prior,
				     t_tskid*			tskid);

t_error			task_release(t_tskid			tskid);

t_error			task_get(t_tskid				tskid,
				 o_task**				o);

t_error			task_init(void);

t_error			task_clean(void);


/*
 * eop
 */

#endif
