/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/map.h
 *
 * created       julien quintard   [wed jun  6 13:31:27 2007]
 * updated       julien quintard   [wed jun  6 15:56:26 2007]
 */

#ifndef GUARD_CORE_MAP
#define GUARD_CORE_MAP			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/core.h>
#include <core/stats.h>
#include <core/as.h>
#include <core/error.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * flags
 */

#define MAP_OPT_NONE		0
#define MAP_OPT_FORCE		(1 << 0)
#define MAP_OPT_USER		(0 << 1)
#define MAP_OPT_PRIVILEGED	(1 << 1)

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * map manager
 */

typedef struct
{
  i_stats			stats;
}				m_stats;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define MAP_CHECK(_map_)						\
  {									\
    if ((_map_) == NULL)						\
      return (ERROR_UNKNOWN);						\
  }

/*
 * enter
 */

#define MAP_ENTER(_map_)						\
  {									\
    MAP_CHECK((_map_));							\
									\
    STATS_BEGIN((_map_)->stats);					\
  }

/*
 * leave
 */

#define MAP_LEAVE(_map_, _error_)					\
  {									\
    STATS_END((_map_)->stats, (_error_));				\
									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/map/map.c
 */

/*
 * ../../core/map/map.c
 */

t_error			map_reserve(i_as		asid,
				    t_opts		opts,
				    t_vsize		size,
				    t_perms		perms,
				    t_vaddr*		addr);

t_error			map_release(i_as		asid,
				    t_vaddr		addr);

t_error			map_resize(i_as			asid,
				   t_vaddr		old,
				   t_vsize		size,
				   t_vaddr*		new);

t_error			map_init(void);

t_error			map_clean(void);


/*
 * eop
 */

#endif
