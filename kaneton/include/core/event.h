/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/event.h
 *
 * created       renaud voltz   [sun feb 12 22:26:04 2006]
 * updated       matthieu bucchianeri   [wed aug 16 20:24:37 2006]
 */

#ifndef CORE_EVENT_H
#define CORE_EVENT_H

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <core/id.h>
#include <core/types.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * event handling way
 */

#define EVENT_FUNCTION		0
#define EVENT_MESSAGE		1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define EVENT_HANDLER(_function_)					\
  ((u_event_handler)(t_event_handler)(_function_))

/*
 * ---------- types -----------------------------------------------------------
 */


/*
 * generic event handler type.
 */

typedef void                    (*t_event_handler)(t_id);

/*
 * event handler type
 */

union				u_event_handler
{
  t_event_handler		function;
  i_task			taskid;
};

/*
 * event object
 */

typedef struct
{
  i_event			eventid;

  t_type			type;

  u_event_handler		handler;

  machdep_data(o_event);
}				o_event;


/*
 * event manager
 */

typedef struct
{
  o_id				id;

  i_stats			stats;

  i_set				events;

  machdep_data(m_event);
}				m_event;

/*
 * the event architecture dependent interface
 */

typedef struct
{
  t_error			(*event_show)(i_event);
  t_error			(*event_notify)(i_event);
  t_error			(*event_reserve)(i_event,
						 t_uint32,
						 u_event_handler);
  t_error			(*event_release)(i_event);
  t_error			(*event_init)(void);
  t_error			(*event_clean)(void);
}				d_event;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define EVENT_CHECK(_event_)						\
  {									\
    if ((_event_) == NULL)						\
      return ERROR_UNKNOWN;						\
  }

/*
 * enter
 */

#define EVENT_ENTER(_event_)						\
  {									\
    EVENT_CHECK((_event_));						\
									\
    STATS_BEGIN((_event_)->stats);					\
  }

/*
 * leave
 */

#define EVENT_LEAVE(_event_, _error_)					\
  {									\
    STATS_END((_event_)->stats, (_error_));				\
									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/event/event.c
 */

/*
 * ../../core/event/event.c
 */

t_error			event_show(i_event			id);

t_error			event_dump(void);

t_error			event_notify(i_event			id);

t_error			event_reserve(i_event			id,
				      t_type			type,
				      u_event_handler		handler);

t_error			event_release(i_event			id);

t_error			event_get(i_event			id,
				  o_event**			o);

t_error			event_init(void);

t_error			event_clean(void);


/*
 * eop
 */

#endif
