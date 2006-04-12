/*
 * licence       kaneton licence
 *
 * project       thread
 *
 * file          /home/mycure/kaneton/kaneton/include/core/thread.h
 *
 * created       renaud voltz   [tue apr  4 03:14:51 2006]
 * updated       julien quintard   [wed apr 12 06:29:40 2006]
 */

#ifndef CORE_THREAD_H
#define CORE_THREAD_H		1

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
 * priorities
 */

#define THREAD_PRIOR		130
#define THREAD_HPRIOR		250
#define THREAD_LPRIOR		10

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * thread execution context
 */

typedef struct
{
  t_vaddr			pc;
  t_vaddr			sp;
}				t_thread_context;

/*
 * thread object
 */

typedef struct
{
  t_thrid			threadid;

  t_tskid			taskid;

  t_prior			prior;

  t_state			sched;

  t_setid			waits;
  t_wait			wait;

  t_vaddr			stack;
  t_vsize			stacksz;

  machdep_data(o_thread);
}				o_thread;


/*
 * thread manager
 */

typedef struct
{
  o_id				id;

  t_staid			stats;

  t_setid			threads;

  machdep_data(m_thread);
}				m_thread;

/*
 * the thread architecture dependent interface
 */

typedef struct
{
  t_error			(*thread_show)(t_thrid);
  t_error			(*thread_suspend)(t_thrid);
  t_error			(*thread_execute)(t_thrid);
  t_error			(*thread_clone)(t_thrid);
  t_error			(*thread_flush)(t_tskid);
  t_error			(*thread_reserve)(t_tskid,
						  t_thrid*);
  t_error			(*thread_release)(t_thrid);
  t_error			(*thread_init)(void);
  t_error			(*thread_clean)(void);
}				d_thread;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define THREAD_CHECK(_thread_)						\
  {									\
    if ((_thread_) == NULL)						\
      return ERROR_UNKNOWN;						\
  }

/*
 * enter
 */

#define THREAD_ENTER(_thread_)						\
  {									\
    THREAD_CHECK((_thread_));						\
									\
    STATS_BEGIN((_thread_)->stats);					\
  }

/*
 * leave
 */

#define THREAD_LEAVE(_thread_, _error_)					\
  {									\
    STATS_END((_thread_)->stats, (_error_));				\
									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/thread/thread.c
 */

/*
 * ../../core/thread/thread.c
 */

t_error			thread_show(t_thrid			threadid);

t_error			thread_dump(void);

t_error			thread_suspend(t_thrid			threadid);

t_error			thread_execute(t_thrid			threadid);

t_error			thread_clone(t_thrid			threadid);

t_error			thread_reserve(t_tskid			taskid,
				       t_thrid*			threadid);

t_error			thread_release(t_thrid			threadid);

t_error			thread_flush(t_tskid			taskid);

t_error			thread_get(t_thrid			threadid,
				   o_thread**			o);

t_error			thread_init(void);

t_error			thread_clean(void);


/*
 * eop
 */

#endif
