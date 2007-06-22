/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/map/map.c
 *
 * created       matthieu bucchianeri   [sun feb 26 12:56:54 2006]
 * updated       matthieu bucchianeri   [tue feb  6 22:39:11 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the map manager is used to manage memory in a simpler way.
 *
 * this manager only does two things: reserving a new map and releasing it.
 * a map is a couple of segment and region.
 *
 * in future implementation, resizing of maps will be possible.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the map manager structure.
 */

m_map*		map;

/*                                                                  [cut] k1 */

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * the kernal as id.
 */

extern i_as	kasid;

/*
 * ---------- functions -------------------------------------------------------
 */

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

      if (region_reserve(asid, segid, 0,
			 REGION_OPT_FORCE | ((opts & MAP_OPT_PRIVILEGED) ?
					     REGION_OPT_PRIVILEGED : 0),
			 *addr, size, &regid) != ERROR_NONE)
	MAP_LEAVE(map, ERROR_UNKNOWN);
    }
  else
    {
      /*
       * b)
       */

      if (region_reserve(asid, segid, 0,
			 REGION_OPT_NONE | ((opts & MAP_OPT_PRIVILEGED) ?
					    REGION_OPT_PRIVILEGED : 0),
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
 * this function remap some memory.
 *
 * steps:
 *
 * 1) resize the segment.
 *  A) if the segment was relocated.
 *   2) the mapping is now invalid, so release the region.
 *   3) try to remap the region at the same address.
 *   4) if it  is impossible, then we also relocate the region (unless
 *      the address was forced)
 *  B) otherwise, the segment kept the same address, mapping is valid.
 *   2) only resize the region, shrinking or growing the mapped area.
 */

t_error			map_resize(i_as			asid,
				   t_vaddr		old,
				   t_vsize		size,
				   t_vaddr*		new)
{
  o_region*		o;
  i_segment		s;
  i_region		r;
  t_paddr		offset;
  t_opts		opts;

  MAP_ENTER(map);

  if (region_get(asid, (i_region)old, &o) != ERROR_NONE)
    MAP_LEAVE(map, ERROR_UNKNOWN);

  /*
   * 1)
   */

  if (segment_resize(o->segid, size, &s) != ERROR_NONE)
    MAP_LEAVE(map, ERROR_UNKNOWN);

  /*
   * A)
   */

  if (s != o->segid)
    {
      offset = o->offset;
      opts = o->opts;

      /*
       * 2)
       */

      if (region_release(asid, (i_region)old) != ERROR_NONE)
	MAP_LEAVE(map, ERROR_UNKNOWN);

      /*
       * 3)
       */

      if (region_reserve(asid,
			 s,
			 offset,
			 opts | REGION_OPT_FORCE,
			 old,
			 size,
			 &r) != ERROR_NONE)
	{
	  if (opts & REGION_OPT_FORCE)
	    MAP_LEAVE(map, ERROR_UNKNOWN);

	  /*
	   * 4)
	   */

	  if (region_reserve(asid,
			     s,
			     offset,
			     opts,
			     0,
			     size,
			     &r) != ERROR_NONE)
	    MAP_LEAVE(map, ERROR_UNKNOWN);
	}

      *new = r;
    }
  /*
   * B)
   */
  else
    {
      /*
       * 2)
       */

      if (region_resize(asid, (i_region)old, size, &r) != ERROR_NONE)
	MAP_LEAVE(map, ERROR_UNKNOWN);

      *new = r;
    }

  MAP_LEAVE(map, ERROR_NONE);
}

/*                                                                 [cut] /k1 */

/*
 * initialize the map manager.
 *
 * steps:
 *
 * 1) allocate the manager structure.
 */

t_error			map_initialize(void)
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

  return (ERROR_NONE);
}

/*
 * release the map manager.
 *
 * steps:
 *
 * 1) free the manager structure.
 */

t_error			map_clean(void)
{
  /*
   * 1)
   */

  free(map);

  return (ERROR_NONE);
}
