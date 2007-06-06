/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/event.h
 *
 * created       julien quintard   [wed jun  6 13:13:41 2007]
 * updated       julien quintard   [wed jun  6 19:17:46 2007]
 */

#ifndef GUARD_CORE_EVENT
#define GUARD_CORE_EVENT		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/id.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the event object identifier
 */

typedef t_id			i_event;

/*
 * ---------- dependencies ----------------------------------------------------
 */

// XXX #include <core/core.h>
#include <core/task.h>
#include <core/set.h>
#include <core/stats.h>
#include <core/error.h>

#include <arch/machdep/machdep.h>

/*
 * ---------- macros ----------------------------------------------------------
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

typedef void			(*t_event_handler)(t_id);

/*
 * event handler type
 */

typedef union
{
  t_event_handler		function;
  i_task			taskid;
}				u_event_handler;

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
