/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.NEW/kaneton/core/include/thread.h
 *
 * created       julien quintard   [wed jun  6 14:31:49 2007]
 * updated       julien quintard   [mon nov 22 16:52:26 2010]
 */

#ifndef CORE_THREAD_H
#define CORE_THREAD_H			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/error.h>
#include <core/id.h>
#include <core/wait.h>

#include <machine/machine.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * priorities
 */

#define THREAD_PRIORITY		130
#define THREAD_HPRIORITY	250
#define THREAD_LPRIORITY	10

/*
 * stack size
 */

#define THREAD_STACKSZ		4096
#define THREAD_MIN_STACKSZ	4096

/*
 * the thread state.
 */

#define THREAD_STATE_RUN	1
#define THREAD_STATE_STOP	2
#define THREAD_STATE_BLOCK	3
#define THREAD_STATE_ZOMBIE	4

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
  i_thread			id;

  i_task			task;

  t_priority			priority;

  t_state			state;

  i_set				waits;
  t_wait			wait;

  t_vaddr			stack;
  t_vsize			stacksz;

  machine_data(o_thread);
}				o_thread;

/*
 * thread manager
 */

typedef struct
{
  o_id				id;

  i_set				threads;

  machine_data(m_thread);
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
						  t_priority);
  t_error			(*thread_state)(i_thread,
						t_state);
  t_error			(*thread_stack)(i_thread,
						t_stack);
  t_error			(*thread_args)(i_thread,
					      const void*,
					      t_size);
  t_error			(*thread_initialize)(void);
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
      return ERROR_KO;						\
  }

/*
 * enter
 */

#define THREAD_ENTER(_thread_)						\
  {									\
    THREAD_CHECK((_thread_));						\
  }

/*
 * leave
 */

#define THREAD_LEAVE(_thread_, _error_)					\
  {									\
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
				       t_priority		prior,
				       i_thread*		threadid);

t_error			thread_release(i_thread			threadid);

t_error			thread_priority(i_thread		threadid,
					t_priority		prior);

t_error			thread_state(i_thread			threadid,
				     t_state			state);

t_error			thread_stack(i_thread			threadid,
				     t_stack			stack);

t_error			thread_args(i_thread			threadid,
				    const void*			args,
				    t_vsize			size);

void			thread_sleep_handler(i_timer		timer,
					     t_vaddr		address);

t_error			thread_sleep(i_thread			id,
				     t_uint32			milliseconds);

t_error			thread_flush(i_task			taskid);

t_error			thread_load(i_thread			threadid,
				    t_thread_context		context);

t_error			thread_store(i_thread			threadid,
				     t_thread_context*		context);

t_error			thread_get(i_thread			threadid,
				   o_thread**			o);

t_error			thread_initialize(void);

t_error			thread_clean(void);


/*
 * eop
 */

#endif
