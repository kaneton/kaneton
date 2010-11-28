/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.STABLE/kaneton/core/include/wait.h
 *
 * created       julien quintard   [wed jun  6 15:48:52 2007]
 * updated       julien quintard   [sun nov 28 18:11:00 2010]
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

#define WAIT_STATE_UNKNOWN	0
#define WAIT_STATE_START	1
#define WAIT_STATE_STOP		2
#define WAIT_STATE_DEATH	4
#define WAIT_STATE_ANY		(WAIT_DEATH | WAIT_START | WAIT_STOP)

#define WAIT_TARGET_NONE	0
#define WAIT_TARGET_SIBLING	1
#define WAIT_TARGET_CHILDREN	2

#define WAIT_STATUS_UNKNOWN	0
#define WAIT_STATUS_EXIT	1
#define WAIT_STATUS_ERROR	2

/*
 * ---------- macro functions -------------------------------------------------
 */

#define WAIT_TASK(_wait_)						\
  (_wait_).u.task

#define WAIT_THREAD(_wait_)						\
  (_wait_).u.thread

#define WAIT_CAUSE(_wait_)						\
  (_wait_).cause

#define WAIT_STATUS(_wait_)						\
  (_wait_).status

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
  t_status		status;
}			t_wait;

#endif
