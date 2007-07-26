/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/view.h
 *
 * created       matthieu bucchianeri   [thu jun 21 00:19:44 2007]
 * updated       matthieu bucchianeri   [thu jul 26 22:30:53 2007]
 */

#ifndef CORE_VIEW_H
#define CORE_VIEW_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/error.h>
#include <core/id.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define VIEW_SIGNAL_RUN		1
#define VIEW_SIGNAL_STOP	2
#define VIEW_SIGNAL_BLOCK	3
#define VIEW_SIGNAL_RESUME	4
#define VIEW_SIGNAL_IRQ		5
#define VIEW_SIGNAL_SYSCALL	6
#define VIEW_SIGNAL_PREEMPT	7
#define VIEW_SIGNAL_SCHEDULING	8

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * view object
 */

typedef struct
{
  t_id				id;

  char*				name;

  t_uint64			time;
}				o_view;

/*
 * view manager
 */

typedef struct
{
  o_id				id;

  o_view			record;
  t_uint64			cycles;

  char*				flyweight[999];

  i_set				views;
}				m_view;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define VIEW_CHECK(_view_)						\
  {									\
    if ((_view_) == NULL)						\
      return (ERROR_UNKNOWN);						\
  }

/*
 * enter
 */

#define VIEW_ENTER(_view_)						\
  {									\
    VIEW_CHECK((_view_));						\
  }

/*
 * leave
 */

#define VIEW_LEAVE(_view_, _error_)					\
  {									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/debug/view.c
 */

/*
 * ../../core/debug/view.c
 */

t_uint64		read_timestamp(void);

t_error			view_reset(void);

t_error			view_signal(const char*			name,
				    t_uint32			nr,
				    t_state			action);

t_error			view_download(void);

t_error			view_initialize(void);

t_error			view_clean(void);


/*
 * eop
 */

#endif
