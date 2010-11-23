/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.NEW/kaneton/core/include/map.h
 *
 * created       julien quintard   [wed jun  6 13:31:27 2007]
 * updated       julien quintard   [mon nov 22 10:12:08 2010]
 */

#ifndef CORE_MAP_H
#define CORE_MAP_H			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/error.h>
#include <core/id.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * flags
 */

#define MAP_OPTION_NONE		0
#define MAP_OPTION_FORCE	(1 << 0)
#define MAP_OPTION_USER		(0 << 1)
#define MAP_OPTION_PRIVILEGED	(1 << 1)

#define MAP_OPTION_INVALID	(~((1 << 1) | (1 << 0)))

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * map manager
 */

typedef struct
{
}				m_map;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define MAP_CHECK(_map_)						\
  {									\
    if ((_map_) == NULL)						\
      return (ERROR_KO);						\
  }

/*
 * enter
 */

#define MAP_ENTER(_map_)						\
  {									\
    MAP_CHECK((_map_));							\
  }

/*
 * leave
 */

#define MAP_LEAVE(_map_, _error_)					\
  {									\
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

t_error			map_reserve(i_as		as,
				    t_options		options,
				    t_vsize		size,
				    t_permissions	permissions,
				    t_vaddr*		address);

t_error			map_release(i_as		as,
				    t_vaddr		address);

t_error			map_resize(i_as			as,
				   t_vaddr		old,
				   t_vsize		size,
				   t_vaddr*		new);

t_error			map_initialize(void);

t_error			map_clean(void);


/*
 * eop
 */

#endif
