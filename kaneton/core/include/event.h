/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.TETON/kaneton/core/include/event.h
 *
 * created       julien quintard   [wed jun  6 13:13:41 2007]
 * updated       julien quintard   [fri nov 26 17:03:53 2010]
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
 * event handling way
 */

#define EVENT_FUNCTION		0
#define EVENT_MESSAGE		1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define EVENT_HANDLER(_function_)					\
  ((u_event_handler)(t_event_handler)(_function_))

#define EVENT_TASK(_task_)						\
  ((u_event_handler)(i_task)(_task_))

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * generic event handler type.
 */

typedef void			(*t_event_handler)(i_event, t_vaddr);

/*
 * event handler type
 */

typedef union
{
  t_event_handler		function;
  i_task			task;
}				u_event_handler;

/*
 * event object
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
 * message object for event
 */

typedef struct
{
  i_event			id;

  t_vaddr			data;
}				o_event_message;

/*
 * event manager
 */

typedef struct
{
  o_id				id;

  i_set				events;

  machine_data(m_event);
}				m_event;

/*
 * the event architecture dependent interface
 */

typedef struct
{
  t_error			(*event_show)(i_event);
  t_error			(*event_enable)(void);
  t_error			(*event_disable)(void);
  t_error			(*event_notify)(i_event);
  t_error			(*event_reserve)(i_event,
						 t_uint32,
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

t_error			event_show(i_event			id);

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
				  o_event**			o);

t_error			event_initialize(void);

t_error			event_clean(void);


/*
 * eop
 */

#endif
