/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/core/include/kaneton/event.h
 *
 * created       renaud voltz   [sun feb 12 22:26:04 2006]
 * updated       renaud voltz   [sun feb 12 22:26:04 2006]
 */

#ifndef KANETON_EVENT_H
#define KANETON_EVENT_H

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <kaneton/id.h>
#include <kaneton/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * event object
 */

typedef struct
{
  t_eventid			eventid;

  t_setid			tasks;

  machdep_data(o_event);
}				o_event;


/*
 * event manager
 */

typedef struct
{
  o_id				id;

  t_staid			stats;

  t_setid			container;

  machdep_data(m_event);
}				m_event;

/*
 * the event architecture dependent interface
 */

typedef struct
{
  t_error			(*event_reserve)(t_eventid);
  t_error			(*event_release)(t_eventid);
  t_error			(*event_init)(void);
  t_error			(*event_clean)(void);
}				i_event;

/*
 * ---------- macros ----------------------------------------------------------
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
 *      ../../kaneton/event/event.c
 */

/*
 * ../../kaneton/event/event.c
 */

t_error			event_show(t_eventid			eventid);

t_error			event_dump(void);

t_error			event_notify(t_eventid			eventid);

t_error			event_reserve(t_eventid			eventid);

t_error			event_release(t_eventid			eventid);

t_error			event_subscribe(t_eventid		eventid,
					t_tskid			tskid);

t_error			event_unsubscribe(t_eventid		eventid,
					  t_tskid		tskid);

t_error			event_get(t_eventid			eventid,
				  o_event**			o);

t_error			event_init(void);

t_error			event_clean(void);


/*
 * eop
 */

#endif