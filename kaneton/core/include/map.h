/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/map.h
 *
 * created       julien quintard   [wed jun  6 13:31:27 2007]
 * updated       julien quintard   [fri jan 14 20:18:43 2011]
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
 * the flags that can be passed at the map reservation time.
 *
 * for more information, please refer to the region-related flags.
 */

#define MAP_OPTION_NONE		0
#define MAP_OPTION_FORCE	(1 << 0)
#define MAP_OPTION_SYSTEM	(1 << 1)

#define MAP_OPTION_INVALID	(~(MAP_OPTION_FORCE | MAP_OPTION_SYSTEM))

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the map manager structure.
 */

typedef struct
{
  machine_data(m_map);
}				m_map;

/*
 * the map dispatcher.
 */

typedef struct
{
  t_status		(*map_reserve)(i_as,
					       t_options,
					       t_vsize,
					       t_permissions,
					       t_vaddr*);
  t_status		(*map_release)(i_as,
					       t_vaddr);
  t_status		(*map_resize)(i_as,
					      t_vaddr,
					      t_vsize,
					      t_vaddr*);
  t_status		(*map_initialize)(void);
  t_status		(*map_clean)(void);
}				d_map;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/map/map.c
 */

/*
 * ../../core/map/map.c
 */

t_status		map_reserve(i_as			as,
				    t_options			options,
				    t_vsize			size,
				    t_permissions		permissions,
				    t_vaddr*			address);

t_status		map_release(i_as			as,
				    t_vaddr			address);

t_status		map_resize(i_as				as,
				   t_vaddr			old,
				   t_vsize			size,
				   t_vaddr*			new);

t_status		map_initialize(void);

t_status		map_clean(void);


/*
 * eop
 */

#endif
