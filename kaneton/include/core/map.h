/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/map.h
 *
 * created       matthieu bucchianeri   [sun feb 26 12:23:01 2006]
 * updated       matthieu bucchianeri   [fri jul 28 17:30:01 2006]
 */

#ifndef CORE_MAP_H
#define CORE_MAP_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <core/id.h>
#include <core/types.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * flags
 */

#define MAP_OPT_NONE		0
#define MAP_OPT_FORCE		(1 << 0)
#define MAP_OPT_PRIVILEGED	(1 << 1)

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * map manager
 */

typedef struct
{
  i_stats	stats;
}		m_map;

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

void*			mmap(void*			start,
			     size_t			length,
			     int			prot,
			     int			flags,
			     int			fd,
			     off_t			offset);

int			munmap(void*			start,
			       size_t			length);

t_error			map_reserve(i_as		asid,
				    t_opts		opts,
				    t_vsize		size,
				    t_perms		perms,
				    t_vaddr*		addr);

t_error			map_release(i_as		asid,
				    t_vaddr		addr);

t_error			map_init(void);

t_error			map_clean(void);


/*
 * eop
 */

#endif
