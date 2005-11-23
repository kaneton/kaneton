/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * region.h
 * 
 * path          /home/mycure/kaneton/core/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:44 2005   mycure
 * last update   Sun Oct 30 23:07:28 2005   mycure
 */

#ifndef KANETON_REGION_H
#define KANETON_REGION_H	1

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
 * XXX
 */

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * region object
 */

typedef struct
{
  t_vaddr			address;

  t_segid			segid;
}				o_region;

/*
 * region manager
 */

typedef struct
{
  o_id				id;

  t_staid			stats;
}				m_region;

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * check
 */

#define REGION_CHECK(_region_)						\
  {									\
    if ((_region_) == NULL)						\
      return (ERROR_UNKNOWN);						\
  }

/*
 * enter
 */

#define REGION_ENTER(_region_)						\
  {									\
    REGION_CHECK((_region_));						\
									\
    STATS_BEGIN((_region_)->stats);					\
  }

/*
 * leave
 */

#define REGION_LEAVE(_region_, _error_)					\
  {									\
    STATS_END((_region_)->stats, (_error_));				\
									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/region/region.c
 */

/*
 * ../../kaneton/region/region.c
 */


/*
 * eop
 */

#endif
