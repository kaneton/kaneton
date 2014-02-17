/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/id.h
 *
 * created       julien quintard   [wed jun  6 12:50:13 2007]
 * updated       julien quintard   [fri dec 10 16:08:01 2010]
 */

#ifndef CORE_ID_H
#define CORE_ID_H			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/error.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * this macro represents an unused/invalid identifier.
 */

#define ID_UNUSED		((t_id)-1)

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * general-purpose identifier
 */

typedef t_uint64		t_id;

/*
 * identifiers for the fundamental kaneton objects.
 */

typedef t_id			i_as;
typedef t_id			i_cell;
typedef t_id			i_cpu;
typedef t_id			i_event;
typedef t_id			i_port;
typedef t_id			i_region;
typedef t_id			i_segment;
typedef t_id			i_set;
typedef t_id			i_task;
typedef t_id			i_thread;
typedef t_id			i_timer;

/*
 * a node represents a communicating task in the kayou distributed system.
 *
 * such a task is first identified by its machine, and then by the task
 * identifier itself.
 */

typedef struct
{
  i_cell			cell;
  i_task			task;
}				i_node;

/*
 * the identifier object used to generate manager-specific identifier.
 */

typedef struct
{
  t_id				id;
}				o_id;

/*
 * the identifier manager.
 */

typedef struct
{
}				m_id;

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <modules/console/include/console.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/id/id.c
 */

/*
 * ../../core/id/id.c
 */

t_status		id_show(o_id*				object,
				mt_margin			margin);

t_status		id_reserve(o_id*			object,
				   t_id*			id);

t_status		id_release(o_id*			object,
				   t_id				id);

t_status		id_build(o_id*				object);

t_status		id_destroy(o_id*			object);

t_status		id_initialize(void);

t_status		id_clean(void);


/*
 * eop
 */

#endif
