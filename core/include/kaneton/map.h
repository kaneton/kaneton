/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/core/include/kaneton/map.h
 *
 * created       matthieu bucchianeri   [sun feb 26 12:23:01 2006]
 * updated       matthieu bucchianeri   [mon feb 27 12:09:20 2006]
 */

#ifndef KANETON_MAP_H
#define KANETON_MAP_H	1

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
 * flags
 */

#define MAP_OPT_NONE		(1 << 0)
#define MAP_OPT_FORCE		(1 << 1)

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * map manager
 */

typedef struct
{
  t_staid	stats;
}		m_map;

/*
 * ---------- macros ----------------------------------------------------------
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
 *      ../../kaneton/map/map.c
 */

/*
 * ../../kaneton/map/map.c
 */

t_error			map_reserve(t_asid		asid,
				    t_opts		opts,
				    t_vaddr*		addr,
				    t_vsize		size,
				    t_perms		perms);

t_error			map_release(t_asid		asid,
				    t_vaddr		addr);

t_error			map_init(void);

t_error			map_clean(void);


/*
 * eop
 */

#endif
