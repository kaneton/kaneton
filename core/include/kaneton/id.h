/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * id.h
 * 
 * path          /home/mycure/kaneton/core/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:44 2005   mycure
 * last update   Sun Oct 30 16:12:53 2005   mycure
 */

#ifndef KANETON_ID_H
#define KANETON_ID_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <kaneton/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

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
  t_staid			stats;
}				m_id;

/*
 * ---------- defines ---------------------------------------------------------
 */

#define ID_UNUSED		((t_id)-1)

/*
 * ---------- macros ----------------------------------------------------------
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
 *      ../../kaneton/id/id.c
 */

/*
 * ../../kaneton/id/id.c
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
