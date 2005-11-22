/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * as.h
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:44 2005   mycure
 * last update   Tue Nov 15 22:09:54 2005   mycure
 */

#ifndef KANETON_AS_H
#define KANETON_AS_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <kaneton/id.h>
#include <kaneton/types.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * init sizes for the array data structure set
 */

#define AS_SEGMENTS_INITSZ	0x4
#define AS_REGIONS_INITSZ	0x4

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * as object
 */

typedef struct
{
  t_asid			asid;

  t_setid			segments;
  t_setid			regions;

  /* XXX machdep_data(o_as); */
}				o_as;

/*
 * as manager
 */

typedef struct
{
  o_id				id;

  t_staid			stats;

  t_setid			container;

  /* XXX machdep_data(m_as); */
}				m_as;

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * check
 */

#define AS_CHECK(_as_)							\
  {									\
    if ((_as_) == NULL)							\
      return (ERROR_UNKNOWN);						\
  }

/*
 * enter
 */

#define AS_ENTER(_as_)							\
  {									\
    AS_CHECK((_as_));							\
									\
    STATS_BEGIN((_as_)->stats);						\
  }

/*
 * leave
 */

#define AS_LEAVE(_as_, _error_)						\
  {									\
    STATS_END((_as_)->stats, (_error_));				\
									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/as/as.c
 */

/*
 * ../../kaneton/as/as.c
 */

t_error			as_show(t_asid				asid);

t_error			as_dump(void);

t_error			as_clone(t_asid				old,
				 t_asid*			new);

t_error			as_reserve(t_asid*			asid);

t_error			as_release(t_asid			asid);

t_error			as_get(t_asid				asid,
			       o_as**				o);

t_error			as_init(void);

t_error			as_clean(void);


/*
 * eop
 */

#endif
