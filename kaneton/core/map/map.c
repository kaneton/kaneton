/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/map/map.c
 *
 * created       matthieu bucchianeri   [sun feb 26 12:56:54 2006]
 * updated       julien quintard   [sat jul  8 02:28:41 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the map manager is used to manage memory in a simpler way.
 *
 * this manager only does two things: reserving a new map and releasing it.
 * a map is a couple of segment and region.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the map manager structure.
 */

m_map*		map;

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k3 */

/*
 * reserve virtual memory and map it.
 *
 * steps:
 *
 * 1) reserve a segment.
 * 2) reserve a region.
 *  a) force using the given virtual address.
 *  b) do not specify a virtual address.
 */

t_error			map_reserve(i_as		asid,
				    t_opts		opts,
				    t_vsize		size,
				    t_perms		perms,
				    t_vaddr*		addr)
{
  i_segment		segid;
  i_region		regid;

  MAP_ENTER(map);

  /*
   * 1)
   */

  if (segment_reserve(asid, size, perms, &segid) != ERROR_NONE)
    MAP_LEAVE(map, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (opts & MAP_OPT_FORCE)
    {
      /*
       * a)
       */

      if (region_reserve(asid, segid, 0, REGION_OPT_FORCE,
			 *addr, size, &regid) != ERROR_NONE)
	MAP_LEAVE(map, ERROR_UNKNOWN);
    }
  else
    {
      /*
       * b)
       */

      if (region_reserve(asid, segid, 0, REGION_OPT_NONE,
			 0, size, &regid) != ERROR_NONE)
	MAP_LEAVE(map, ERROR_UNKNOWN);

      *addr = regid;
    }

  MAP_LEAVE(map, ERROR_NONE);
}

/*
 * release a virtual memory space.
 *
 * steps:
 *
 * 1) get the region object to extract the segment id.
 * 2) release the region.
 * 3) release the segment.
 */

t_error			map_release(i_as		asid,
				    t_vaddr		addr)
{
  o_region*		reg;
  i_region		regid;
  i_segment		segid;

  MAP_ENTER(map);

  /*
   * 1)
   */

  regid = addr;

  if (region_get(asid, regid, &reg) != ERROR_NONE)
    MAP_LEAVE(map, ERROR_UNKNOWN);

  segid = reg->segid;

  /*
   * 2)
   */

  if (region_release(asid, regid) != ERROR_NONE)
    MAP_LEAVE(map, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (segment_release(segid) != ERROR_NONE)
    MAP_LEAVE(map, ERROR_UNKNOWN);

  MAP_LEAVE(map, ERROR_NONE);
}

/*
 * initialize the map manager.
 *
 * steps:
 *
 * 1) allocate the manager structure.
 * 2) reserve a statistic object.
 */

t_error			map_init(void)
{
  /*
   * 1)
   */

  if ((map = malloc(sizeof(m_map))) == NULL)
    {
      cons_msg('!', "map: cannot allocate memory for the map "
	       "manager structure\n");

      return (ERROR_UNKNOWN);
    }

  memset(map, 0x0, sizeof(m_map));

  /*
   * 2)
   */

  STATS_RESERVE("map", &map->stats);

  return (ERROR_NONE);
}

/*
 * release the map manager.
 *
 * steps:
 *
 * 1) release the stats object.
 * 2) free the manager structure.
 */

t_error			map_clean(void)
{
  /*
   * 1)
   */

  STATS_RELEASE(map->stats);

  /*
   * 2)
   */

  free(map);

  return (ERROR_NONE);
}

/*                                                                 [cut] /k3 */
