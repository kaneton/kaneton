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
 * updated       julien quintard   [mon dec 13 13:14:03 2010]
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
 * ---------- types -----------------------------------------------------------
 */

/*
 * intra-kernel event handler type.
 */

typedef void			(*t_event_routine)(i_event,
						   t_vaddr);

/*
 * message object for extra-kernel events i.e message notification
 * sent to a task.
 */

typedef struct
{
  i_event			id;

  t_vaddr			data;

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
  t_vaddr			data;

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
  t_error			(*event_show)(i_event,
					      mt_margin);
  t_error			(*event_dump)(void);
  t_error			(*event_notify)(i_event);
  t_error			(*event_enable)(void);
  t_error			(*event_disable)(void);
  t_error			(*event_reserve)(i_event,
						 t_type,
						 u_event_handler,
						 t_vaddr);
  t_error			(*event_release)(i_event);
  t_error			(*event_initialize)(void);
  t_error			(*event_clean)(void);
}				d_event;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/event/event.c
 */

/*
 * ../../core/event/event.c
 */

t_error			event_show(i_event			id,
				   mt_margin			margin);

t_error			event_dump(void);

t_error			event_notify(i_event			id);

t_error			event_enable(void);

t_error			event_disable(void);

t_error			event_reserve(i_event			id,
				      t_type			type,
				      u_event_handler		handler,
				      t_vaddr			data);

t_error			event_release(i_event			id);

t_error			event_exist(i_event			id);

t_error			event_get(i_event			id,
				  o_event**			object);

t_error			event_initialize(void);

t_error			event_clean(void);


/*
 * eop
 */

#endif
