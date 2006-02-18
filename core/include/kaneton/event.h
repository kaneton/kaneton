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

#ifndef EVENT_EVENT_H
#define EVENT_EVENT_H

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
 * task object
 */

typedef struct
{
  t_eventid			eventid;

  machdep_data(o_event);
}				o_event;


/*
 * task manager
 */

typedef struct
{
  o_id				id;

  t_staid			stats;

  t_setid			container;

  machdep_data(m_event);
}				m_event;

/*
 * the task architecture dependent interface
 */

typedef struct
{
  t_error			(*event_subscribe)(void);
  t_error			(*event_init)(void);
  t_error			(*event_clean)(void);
}				i_event;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/event/event.c
 */

/*
 * ../../kaneton/event/event.c
 */

t_error			event_init(void);


/*
 * eop
 */

#endif
