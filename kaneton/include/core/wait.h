/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/wait.h
 *
 * created       julien quintard   [wed jun  6 15:48:52 2007]
 * updated       julien quintard   [wed jun  6 17:24:52 2007]
 */

#ifndef GUARD_CORE_WAIT
#define GUARD_CORE_WAIT			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/core.h>
#include <core/task.h>
#include <core/thread.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define WAIT_DEATH		(1 << 1)
#define WAIT_START		(1 << 2)
#define WAIT_STOP		(1 << 3)
#define WAIT_ALL		(WAIT_DEATH | WAIT_START | WAIT_STOP)

#define WAIT_ID			(0 << 4)
#define WAIT_CHILDREN		(1 << 4)

#define WAIT_NOHANG		(1 << 0)

#define STATUS_STARTED		1
#define STATUS_STOPPED		2
#define STATUS_EXITED		3

/*
 * ---------- macro functions -------------------------------------------------
 */

#define WAIT_TASK(_wait_)						\
  (_wait_).u.task

#define WAIT_THREAD(_wait_)						\
  (_wait_).u.thread

#define WAIT_CAUSE(_wait_)						\
  (_wait_).status

#define WAIT_STATUS(_wait_)						\
  (_wait_).error

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * wait type
 */

typedef struct
{
  union
  {
    i_task			task;
    i_thread			thread;
  }				id;

  t_status			status;

  t_uint32			error;
}				t_wait;

#endif
