/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/wait.h
 *
 * created       julien quintard   [wed jun  6 15:48:52 2007]
 * updated       julien quintard   [fri dec 10 11:48:43 2010]
 */

#ifndef CORE_WAIT_H
#define CORE_WAIT_H			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/id.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * these macro indicate the state of the task/thread to wait for.
 */

#define WAIT_STATE_NONE		0
#define WAIT_STATE_START	1
#define WAIT_STATE_STOP		2
#define WAIT_STATE_DEATH	4
#define WAIT_STATE_ANY		(WAIT_STATE_START |			\
				 WAIT_STATE_START |			\
				 WAIT_STATE_DEATH)

/*
 * this value is used to indicate an unknown or irrelevant value.
 *
 * let's recall that the 'value' is the value returned through the exit()
 * function. this value is passed to the waited thread.
 */

#define WAIT_VALUE_UNKNOWN	-1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro-function returns the identifier of the task that has been
 * waiting for.
 */

#define WAIT_TASK(_wait_)						\
  (_wait_)->id.task

/*
 * this macro-function returns the identifier of the thread that has
 * been waiting for.
 */

#define WAIT_THREAD(_wait_)						\
  (_wait_)->id.thread

/*
 * this macro-function returns the reason that led to the wait to
 * succeed. indeed, the wait() functions take a set of states for
 * which the wait will succeed, should the target's state change
 * to one of these states.
 *
 * for example, a wait for the stop or block state would provide the
 * state block in the cause should the task/thread have blocked.
 */

#define WAIT_CAUSE(_wait_)						\
  (_wait_)->cause

/*
 * this macro-function returns the value which has been returned
 * by a task/thread after it exited.
 */

#define WAIT_VALUE(_wait_)						\
  (_wait_)->value

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the wait type that is returned to the caller.
 *
 * the 'id' attribute contains either the task or thread identifier
 * depending on the target i.e the function used: either task_wait() or
 * thread_wait().
 *
 * the 'state' indicates the state for which the caller is waiting for.
 *
 * the 'cause' indicates what state change caused the wait to succeed i.e
 * the cause is obviously one of the states included in 'state'.
 *
 * finally 'value' is the task/thread exit() value that is passed back to the
 * waiting thread.
 */

typedef struct
{
  union
  {
    i_task		task;
    i_thread		thread;
  }			id;

  t_state		state;
  t_state		cause;
  t_value		value;
}			s_wait;

#endif
