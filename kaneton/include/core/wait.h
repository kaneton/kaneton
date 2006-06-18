/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/wait.h
 *
 * created       julien quintard   [wed apr 12 05:17:42 2006]
 * updated       matthieu bucchianeri   [sun jun 18 19:07:26 2006]
 */

#ifndef CORE_WAIT_H
#define CORE_WAIT_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <core/types.h>

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
    t_thrid			thread;
  }				id;

  t_status			status;

  t_uint32			error;
}				t_wait;

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

#endif
