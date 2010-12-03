/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...neton.STABLE/kaneton/core/include/wait.h
 *
 * created       julien quintard   [wed jun  6 15:48:52 2007]
 * updated       julien quintard   [thu dec  2 11:41:36 2010]
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

#define WAIT_STATE_NONE		0
#define WAIT_STATE_START	1
#define WAIT_STATE_STOP		2
#define WAIT_STATE_DEATH	4
#define WAIT_STATE_ANY		(WAIT_STATE_START |			\
				 WAIT_STATE_START |			\
				 WAIT_STATE_DEATH)

#define WAIT_TARGET_NONE	0
#define WAIT_TARGET_SIBLING	1
#define WAIT_TARGET_CHILDREN	2

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
 * which the wait should succeed should the target's state change
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
}			t_wait;

#endif
