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
 * updated       julien quintard   [wed dec 15 14:25:18 2010]
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
 * the thread priorities.
 */

#define THREAD_PRIORITY		130
#define THREAD_PRIORITY_HIGH	250
#define THREAD_PRIORITY_LOW	10

/*
 * the stack sizes.
 */

#define THREAD_STACKSZ		500 * PAGESZ
#define THREAD_STACKSZ_HIGH	1000 * PAGESZ
#define THREAD_STACKSZ_LOW	1 * PAGESZ

/*
 * some initial size for the sets.
 */

#define THREAD_WAITS_INITSZ	0x1

/*
 * the thread states.
 *
 * note that whenever a thread exits, its state changes to zombie. then,
 * once someone has taken notice of the thread's death, the state changes
 * to dead.
 *
 * a dead thread resides in the morgue until it is definitely released.
 */

#define THREAD_STATE_START	1
#define THREAD_STATE_STOP	2
#define THREAD_STATE_BLOCK	3
#define THREAD_STATE_ZOMBIE	4
#define THREAD_STATE_DEAD	5

/*
 * the delay in milliseconds until the morgue runs and releases the
 * resources associated to the dead threads.
 */

#define THREAD_MORGUE_DELAY	10000

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the thread execution context which is composed of the PC - Program
 * Counter and the SP - Stack Pointer.
 *
 * note that, obviously, this is a generic context. a more specific
 * context with advanced registers etc. will probably be provided by the
 * machine.
 */

typedef struct
{
  t_vaddr			pc;
  t_vaddr			sp;
}				s_thread_context;

/*
 * the thread object identifier by a unique identifier 'id'.
 *
 * the 'task' references the task the thread belongs to.
 *
 * the 'priority' indicates the thread priority while the 'state' specifies
 * the current thread's state.
 *
 * the 'waits' set contains the identifiers of the threads waiting for
 * the thread to change its state.
 *
 * the 'wait' structure is used by the thread/task waited for for passing
 * information such as the exit value etc.
 *
 * the 'value' contains the thread's exit value, when it exits!
 *
 * finally the 'stack' and 'stacksz' reference the thread's stack.
 */

typedef struct
{
  i_thread			id;

  i_task			task;

  t_priority			priority;

  t_state			state;

  i_set				waits;

  struct
  {
    t_state			state;

    t_state			cause;
    t_value			value;
  }				wait;

  t_value			value;

  t_vaddr			stack;
  t_vsize			stacksz;

  machine_data(o_thread);
}				o_thread;

/*
 * the thread manager which is basically composed of the set of threads
 * populating the system.
 */

typedef struct
{
  o_id				id;

  i_set				threads;

  machine_data(m_thread);
}				m_thread;

/*
 * the thread dispatcher.
 */

typedef struct
{
  t_error			(*thread_show)(i_thread,
					       mt_margin);
  t_error			(*thread_dump)(void);
  t_error			(*thread_reserve)(i_task,
						  i_thread*);
  t_error			(*thread_release)(i_thread);
  t_error			(*thread_priority)(i_thread,
						  t_priority);
  t_error			(*thread_start)(i_thread);
  t_error			(*thread_stop)(i_thread);
  t_error			(*thread_block)(i_thread);
  t_error			(*thread_exit)(i_thread,
					       t_value);
  t_error			(*thread_wait)(i_thread,
					       i_thread,
					       t_state,
					       s_wait*);
  t_error			(*thread_stack)(i_thread,
						s_stack);
  t_error			(*thread_args)(i_thread,
					      const void*,
					      t_size);
  t_error			(*thread_sleep)(i_thread,
						t_uint32);
  t_error			(*thread_flush)(i_task);
  t_error			(*thread_load)(i_thread,
					       s_thread_context);
  t_error			(*thread_store)(i_thread,
						s_thread_context*);
  t_error			(*thread_initialize)(void);
  t_error			(*thread_clean)(void);
}				d_thread;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/thread/thread.c
 */

/*
 * ../../core/thread/thread.c
 */

t_error			thread_show(i_thread			threadid,
				    mt_margin			margin);

t_error			thread_dump(void);

t_error			thread_reserve(i_task			taskid,
				       t_priority		prior,
				       i_thread*		threadid);

t_error			thread_release(i_thread			threadid);

t_error			thread_priority(i_thread		threadid,
					t_priority		prior);

t_error			thread_start(i_thread			id);

t_error			thread_stop(i_thread			id);

t_error			thread_block(i_thread			id);

t_error			thread_exit(i_thread			id,
				    t_value			value);

void			thread_bury(i_timer			timer,
				    t_vaddr			address);

t_error			thread_wait(i_thread			id,
				    i_thread			target,
				    t_state			state,
				    s_wait*			wait);

t_error			thread_stack(i_thread			threadid,
				     s_stack			stack);

t_error			thread_args(i_thread			threadid,
				    void*			args,
				    t_vsize			size);

void			thread_sleep_handler(i_timer		timer,
					     t_vaddr		address);

t_error			thread_sleep(i_thread			id,
				     t_uint32			milliseconds);

t_error			thread_flush(i_task			taskid);

t_error			thread_load(i_thread			threadid,
				    s_thread_context		context);

t_error			thread_store(i_thread			threadid,
				     s_thread_context*		context);

t_error			thread_current(i_thread*		thread);

t_error			thread_exist(i_thread			threadid);

t_error			thread_get(i_thread			threadid,
				   o_thread**			o);

t_error			thread_initialize(void);

t_error			thread_clean(void);


/*
 * eop
 */

#endif
