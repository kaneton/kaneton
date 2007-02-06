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

#include <klibc.h>
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
 * this function is a wrapper of mmap to map_reserve.
 */

void*			mmap(void*			start,
			     size_t			length,
			     int			prot,
			     int			flags,
			     int			fd,
			     off_t			offset)
{
  t_vaddr		addr;
  t_perms		perms = 0;

  if (flags)
    return MAP_FAILED;

  if (prot & PROT_READ)
    perms |= PERM_READ;
  if (prot & PROT_WRITE)
    perms |= PERM_WRITE;

  if (map_reserve(kasid,
		  MAP_OPT_NONE,
		  length,
		  perms,
		  &addr) != ERROR_NONE)
    return MAP_FAILED;

  return (void*)addr;
}

/*
 * this function is a wrapper to munmap.
 */

int			munmap(void*			start,
			       size_t			length)
{
  if (map_release(kasid, (t_vaddr)start) != ERROR_NONE)
    return -1;

  return 0;
}

/*
 * this is a wrapper to mremap.
 */

void*			mremap(void*			old_address,
			       size_t			old_size,
			       size_t			new_size,
			       unsigned long		flags)
{
  o_region*		o = NULL;
  t_opts		opts;
  t_vaddr		p;

  if (!(flags & MREMAP_MAYMOVE))
    {
      if (region_get(kasid, (i_region)(t_vaddr)old_address, &o) != ERROR_NONE)
	return MAP_FAILED;

      opts = o->opts;
      o->opts &= REGION_OPT_FORCE;
    }

  if (map_resize(kasid, old_address, new_size, &p) != ERROR_NONE)
    return MAP_FAILED;

  if (o)
    {
      o->opts = opts;
    }

  return (void*)p;
}

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
