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
 * updated       julien quintard   [sun jan 30 20:56:52 2011]
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

#define THREAD_PRIORITY			130
#define THREAD_PRIORITY_HIGH		250
#define THREAD_PRIORITY_LOW		10

/*
 * these macro can be used whenever a stack must be allocated.
 */

#define THREAD_STACK_ADDRESS_NONE	(t_vaddr)NULL

/*
 * the stack sizes.
 */

#define THREAD_STACK_SIZE		500 * ___kaneton$pagesz
#define THREAD_STACK_SIZE_HIGH		1000 * ___kaneton$pagesz
#define THREAD_STACK_SIZE_LOW		1 * ___kaneton$pagesz

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

#define THREAD_STATE_START		1
#define THREAD_STATE_STOP		2
#define THREAD_STATE_BLOCK		3
#define THREAD_STATE_ZOMBIE		4
#define THREAD_STATE_DEAD		5

/*
 * the delay in milliseconds until the morgue runs and releases the
 * resources associated to the dead threads.
 *
 * note that this delay is based on the task's because, whenever a task
 * exits, the threads are asked to exit as well. however, the threads
 * must be buried before the task. otherwise, at the thread burial, the
 * thread's task would be retrieved, but since the task has already been
 * buried, an error would occur.
 */

#define THREAD_MORGUE_DELAY		TASK_MORGUE_DELAY / 2

/*
 * this macro defines the index of the queue in which we wishes the
 * idle thread to be placed i.e next to the lowest priority queue.
 */

#define THREAD_IDLE_QUEUE		SCHEDULER_PRIORITY_LOW + 1

/*
 * this macro defines the idle thread's priority according to the queue
 * in which we want it to be placed.
 */

#define THREAD_IDLE_PRIORITY						\
  (									\
    {									\
      extern m_kernel	_kernel;					\
									\
      o_task*		_task_;						\
									\
      assert(task_get(_kernel.task, &_task_) == STATUS_OK);		\
									\
       ((THREAD_IDLE_QUEUE - SCHEDULER_PRIORITY_LOW) *			\
	((TASK_PRIORITY_KERNEL_HIGH - TASK_PRIORITY_BACKGROUND_LOW) *	\
	 (THREAD_PRIORITY_HIGH - THREAD_PRIORITY_LOW)) /		\
	(SCHEDULER_PRIORITY_HIGH - SCHEDULER_PRIORITY_LOW) /		\
	(_task_->priority - TASK_PRIORITY_BACKGROUND_LOW)) +		\
       THREAD_PRIORITY_LOW + 1;						\
    }									\
  )

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
 * finally the 'stack' attribute references the thread's stack.
 *
 * note that a timer is provided should the thread need to trigger an
 * action. for instance the timer is used for the thread to sleep for some
 * time.
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

  struct
  {
    t_vaddr			base;
    t_vsize			size;
  }				stack;

  t_vaddr			entry;

  i_timer			timer;

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

  struct
  {
    i_set			field;
    i_timer			timer;
  }				morgue;

  machine_data(m_thread);
}				m_thread;

/*
 * the thread dispatcher.
 */

typedef struct
{
  t_status		(*thread_show)(i_thread,
					       mt_margin);
  t_status		(*thread_dump)(void);
  t_status		(*thread_reserve)(i_task,
						  t_priority,
						  t_vaddr,
						  t_vsize,
						  t_vaddr,
						  i_thread*);
  t_status		(*thread_release)(i_thread);
  t_status		(*thread_priority)(i_thread,
						  t_priority);
  t_status		(*thread_start)(i_thread);
  t_status		(*thread_stop)(i_thread);
  t_status		(*thread_block)(i_thread);
  t_status		(*thread_exit)(i_thread,
					       t_value);
  t_status		(*thread_wait)(i_thread,
					       i_thread,
					       t_state,
					       s_wait*);
  t_status		(*thread_arguments)(i_thread,
						    void*,
						    t_size);
  t_status		(*thread_sleep)(i_thread,
						t_delay);
  t_status		(*thread_flush)(i_task);
  t_status		(*thread_load)(i_thread,
					       s_thread_context);
  t_status		(*thread_store)(i_thread,
						s_thread_context*);
  t_status		(*thread_initialize)(void);
  t_status		(*thread_clean)(void);
}				d_thread;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/thread/thread.c
 */

/*
 * ../../core/thread/thread.c
 */

t_status		thread_show(i_thread			threadid,
				    mt_margin			margin);

t_status		thread_dump(void);

t_status		thread_reserve(i_task			taskid,
				       t_priority		prior,
				       t_vaddr			stack,
				       t_vsize			stacksz,
				       t_vaddr			entry,
				       i_thread*		id);

t_status		thread_release(i_thread			threadid);

t_status		thread_priority(i_thread		threadid,
					t_priority		prior);

t_status		thread_start(i_thread			id);

t_status		thread_stop(i_thread			id);

t_status		thread_block(i_thread			id);

t_status		thread_exit(i_thread			id,
				    t_value			value);

void			thread_bury(i_timer			timer,
				    t_vaddr			data);

t_status		thread_wait(i_thread			id,
				    i_thread			target,
				    t_state			state,
				    s_wait*			wait);

t_status		thread_arguments(i_thread		threadid,
					 void*			arguments,
					 t_vsize		size);

void			thread_wakeup(i_timer			timer,
				      t_vaddr			data);

t_status		thread_sleep(i_thread			id,
				     t_delay			milliseconds);

t_status		thread_flush(i_task			taskid);

t_status		thread_load(i_thread			threadid,
				    s_thread_context		context);

t_status		thread_store(i_thread			threadid,
				     s_thread_context*		context);

t_status		thread_current(i_thread*		thread);

t_bool			thread_exist(i_thread			threadid);

t_status		thread_get(i_thread			id,
				   o_thread**			object);

t_status		thread_initialize(void);

t_status		thread_clean(void);


/*
 * eop
 */

#endif
