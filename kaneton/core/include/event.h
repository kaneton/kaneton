/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/event.h
 *
 * created       julien quintard   [wed jun  6 13:13:41 2007]
 * updated       julien quintard   [wed mar  2 20:27:04 2011]
 */

#ifndef CORE_EVENT_H
#define CORE_EVENT_H			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/error.h>
#include <core/id.h>

#include <machine/machine.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * the type of the event handler.
 */

#define EVENT_TYPE_FUNCTION		0
#define EVENT_TYPE_MESSAGE		1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro-function casts a given routine pointer on a proper
 * event handler union type.
 */

#define EVENT_ROUTINE(_routine_)					\
  ((u_event_handler)(t_event_routine)(_routine_))

/*
 * this macro-function casts a task identifier into a proper event
 * handler union type.
 */

#define EVENT_TASK(_task_)						\
  ((u_event_handler)(i_task)(_task_))

/*
 * this macro-function casts an arbitrary data into a valid event data.
 */

#define EVENT_DATA(_data_)						\
  (t_data)((_data_))

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * intra-kernel event handler type.
 */

typedef void			(*t_event_routine)(i_event,
						   t_data);

/*
 * message object for extra-kernel events i.e message notification
 * sent to a task.
 */

typedef struct
{
  i_event			id;

  t_data			data;

  machine_data(o_event_message);
}				o_event_message;

/*
 * general-purpose event handler type.
 */

typedef union
{
  t_event_routine		routine;
  i_task			task;
}				u_event_handler;

/*
 * the event object structure.
 *
 * note that the data attribute represents data passed to the handler
 * once triggered.
 */

typedef struct
{
  i_event			id;

  t_type			type;

  u_event_handler		handler;
  t_data			data;

  machine_data(o_event);
}				o_event;

/*
 * event manager.
 */

typedef struct
{
  i_set				events;

  machine_data(m_event);
}				m_event;

/*
 * the event dispatcher.
 */

typedef struct
{
  t_status		(*event_show)(i_event,
					      mt_margin);
  t_status		(*event_dump)(void);
  t_status		(*event_notify)(i_event);
  t_status		(*event_enable)(void);
  t_status		(*event_disable)(void);
  t_status		(*event_reserve)(i_event,
						 t_type,
						 u_event_handler,
						 t_data);
  t_status		(*event_release)(i_event);
  t_status		(*event_initialize)(void);
  t_status		(*event_clean)(void);
}				d_event;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/event/event.c
 */

/*
 * ../../core/event/event.c
 */

t_status			event_show(i_event			id,
					   mt_margin			margin);

t_status			event_dump(void);

t_status			event_notify(i_event			id);

t_status			event_enable(void);

t_status			event_disable(void);

t_status			event_reserve(i_event			id,
					      t_type			type,
					      u_event_handler		handler,
					      t_data			data);

t_status			event_release(i_event			id);

t_bool				event_exist(i_event			id);

t_status			event_get(i_event			id,
					  o_event**			object);

t_status			event_initialize(void);

t_status			event_clean(void);


/*
 * eop
 */

#endif
