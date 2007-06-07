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
 * updated       julien quintard   [thu jun  7 12:09:16 2007]
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

#define ID_UNUSED		((t_id)-1)

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * generic kaneton identifier
 */

typedef t_uint64		t_id;

/*
 * identifiers of the kaneton objects
 */

typedef t_id			i_stats;
typedef t_id			i_set;
typedef t_id			i_segment;
typedef t_id			i_region;
typedef t_id			i_as;
typedef t_id			i_event;
typedef t_id			i_port;
typedef t_id			i_task;
typedef t_id			i_thread;
typedef t_id			i_cpu;
typedef t_id			i_machine;

/*
 * a node represents a communicating task in the distributed system.
 *
 * such a task is first identified the but machine holding it, and then
 * by the task identifier.
 */
typedef struct
{
  i_machine			machine;
  i_task			task;
}				i_node;

/*
 * identifier object
 */

typedef struct
{
  t_id				id;
}				o_id;

/*
 * the identifier manager
 */

typedef struct
{
}				m_id;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define ID_CHECK(_id_)							\
  {									\
    if ((_id_) == NULL)							\
      return (ERROR_UNKNOWN);						\
  }

/*
 * enter
 */

#define ID_ENTER(_id_)							\
  {									\
    ID_CHECK((_id_));							\
  }

/*
 * leave
 */

#define ID_LEAVE(_id_, _error_)						\
  {									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/id/id.c
 */

/*
 * ../../core/id/id.c
 */

t_error			id_show(o_id*				o);

t_error			id_clone(o_id*				o,
				 t_id				old,
				 t_id*				new);

t_error			id_reserve(o_id*			o,
				   t_id*			i);

t_error			id_release(o_id*			o,
				   t_id				i);

t_error			id_build(o_id*				o);

t_error			id_destroy(o_id*			o);

t_error			id_init(void);

t_error			id_clean(void);


/*
 * eop
 */

#endif
