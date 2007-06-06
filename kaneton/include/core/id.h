/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/id.h
 *
 * created       julien quintard   [wed jun  6 12:50:13 2007]
 * updated       julien quintard   [wed jun  6 17:23:10 2007]
 */

#ifndef GUARD_CORE_ID
#define GUARD_CORE_ID			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

//#include <core/core.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * generic kaneton identifier
 */

typedef t_uint64		t_id;

/*
 * identifier object
 */

typedef struct
{
  t_id				id;
}				o_id;

/*
 * ---------- macros ----------------------------------------------------------
 */

#define ID_UNUSED		((t_id)-1)

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/stats.h>
#include <core/error.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the identifier manager
 */

typedef struct
{
  i_stats			stats;
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
									\
    STATS_BEGIN((_id_)->stats);						\
  }

/*
 * leave
 */

#define ID_LEAVE(_id_, _error_)						\
  {									\
    STATS_END((_id_)->stats, (_error_));				\
									\
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
