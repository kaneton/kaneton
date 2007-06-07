/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/thread.h
 *
 * created       julien quintard   [wed jun  6 14:31:49 2007]
 * updated       julien quintard   [thu jun  7 00:21:57 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file supposes t_wait, t_error, identifier and basic types are
 * defined.
 */

#ifndef CORE_THREAD_H
#define CORE_THREAD_H			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/thread.h>

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
 * stack size
 */

#define THREAD_STACKSZ		4096
#define THREAD_MIN_STACKSZ	4096

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * generic thread execution context
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
  i_thread			threadid;

  i_task			taskid;

  t_prior			prior;

  t_state			sched;

  i_set				waits;
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

  i_stats			stats;

  i_set				threads;

  machdep_data(m_thread);
}				m_thread;

/*
 * the thread architecture dependent interface
 */

typedef struct
{
  t_error			(*thread_show)(i_thread);
  t_error			(*thread_give)(i_task,
					       i_thread);
  t_error			(*thread_clone)(i_task,
						i_thread,
						i_thread*);
  t_error			(*thread_flush)(i_task);
  t_error			(*thread_load)(i_thread,
					       t_thread_context);
  t_error			(*thread_store)(i_thread,
						t_thread_context*);
  t_error			(*thread_reserve)(i_task,
						  i_thread*);
  t_error			(*thread_release)(i_thread);
  t_error			(*thread_priority)(i_thread,
						  t_prior);
  t_error			(*thread_state)(i_thread,
						t_state);
  t_error			(*thread_stack)(i_thread,
						t_stack);
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

t_error			thread_show(i_thread			threadid);

t_error			thread_dump(void);

t_error			thread_give(i_task			taskid,
				    i_thread			threadid);

t_error			thread_clone(i_task			taskid,
				     i_thread			old,
				     i_thread*			new);

t_error			thread_reserve(i_task			taskid,
				       t_prior			prior,
				       i_thread*		threadid);

t_error			thread_release(i_thread			threadid);

t_error			thread_priority(i_thread		threadid,
					t_prior			prior);

t_error			thread_state(i_thread			threadid,
				     t_state			sched);

t_error			thread_stack(i_thread			threadid,
				     t_stack			stack);

t_error			thread_flush(i_task			taskid);

t_error			thread_load(i_thread			threadid,
				    t_thread_context		context);

t_error			thread_store(i_thread			threadid,
				     t_thread_context*		context);

t_error			thread_get(i_thread			threadid,
				   o_thread**			o);

t_error			thread_init(void);

t_error			thread_clean(void);


/*
 * eop
 */

#endif
