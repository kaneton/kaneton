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

#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the map manager structure.
 */

m_map*		_map = NULL;

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
 *
 */

t_error			map_reserve(i_as		as,
				    t_options		options,
				    t_vsize		size,
				    t_permissions	permissions,
				    t_vaddr*		address)
{
  i_segment		segment;
  i_region		region;

  MAP_ENTER(_map);

  assert((options & MAP_OPTION_INVALID) == 0);
  assert((permissions & PERMISSION_INVALID) == 0);
  assert(size != 0);
  assert(address != NULL);

  /*
   * 1)
   */

  if (segment_reserve(as, size, permissions, &segment) != ERROR_OK)
    MAP_LEAVE(_map, ERROR_KO);

  /*
   * 2)
   */

  if (options & MAP_OPTION_FORCE)
    {
      /*
       * a)
       */

      if (region_reserve(as,
			 segment,
			 0,
			 REGION_OPTION_FORCE |
			 ((options & MAP_OPTION_PRIVILEGED) ?
			  REGION_OPTION_PRIVILEGED : 0),
			 *address,
			 size,
			 &region) != ERROR_OK)
	MAP_LEAVE(_map, ERROR_KO);
    }
  else
    {
      /*
       * b)
       */

      if (region_reserve(as,
			 segment,
			 0,
			 REGION_OPTION_NONE |
			 ((options & MAP_OPTION_PRIVILEGED) ?
			  REGION_OPTION_PRIVILEGED : 0),
			 0,
			 size,
			 &region) != ERROR_OK)
	MAP_LEAVE(_map, ERROR_KO);

      *address = (t_vaddr)region;
    }

  MAP_LEAVE(_map, ERROR_OK);
}

/*
 * release a virtual memory space.
 *
 * steps:
 *
 * 1) get the region object to extract the segment id.
 * 2) release the region.
 * 3) release the segment.
 *
 */

t_error			map_release(i_as		as,
				    t_vaddr		address)
{
  i_region		region;
  i_segment		segment;
  o_region*		o;

  MAP_ENTER(_map);

  /*
   * 1)
   */

  region = (i_region)address;

  if (region_get(as, region, &o) != ERROR_OK)
    MAP_LEAVE(_map, ERROR_KO);

  segment = o->segment;

  /*
   * 2)
   */

  if (region_release(as, region) != ERROR_OK)
    MAP_LEAVE(_map, ERROR_KO);

  /*
   * 3)
   */

  if (segment_release(segment) != ERROR_OK)
    MAP_LEAVE(_map, ERROR_KO);

  MAP_LEAVE(_map, ERROR_OK);
}

/*							     [block::resize] */

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

t_error			map_resize(i_as			as,
				   t_vaddr		old,
				   t_vsize		size,
				   t_vaddr*		new)
{
  t_options		options;
  t_paddr		offset;
  i_segment		segment;
  i_region		region;
  o_region*		o;

  MAP_ENTER(_map);

  assert(size != 0);
  assert(new != NULL);

  if (region_get(as, (i_region)old, &o) != ERROR_OK)
    MAP_LEAVE(_map, ERROR_KO);

  /*
   * 1)
   */

  if (segment_resize(o->segment, size, &segment) != ERROR_OK)
    MAP_LEAVE(_map, ERROR_KO);

  /*
   * A)
   */

  if (segment != o->segment)
    {
      offset = o->offset;
      options = o->options;

      /*
       * 2)
       */

      if (region_release(as, (i_region)old) != ERROR_OK)
	MAP_LEAVE(_map, ERROR_KO);

      /*
       * 3)
       */

      if (region_reserve(as,
			 segment,
			 offset,
			 options | REGION_OPTION_FORCE,
			 old,
			 size,
			 &region) != ERROR_OK)
	{
	  if (options & REGION_OPTION_FORCE)
	    MAP_LEAVE(_map, ERROR_KO);

	  /*
	   * 4)
	   */

	  if (region_reserve(as,
			     segment,
			     offset,
			     options,
			     0,
			     size,
			     &region) != ERROR_OK)
	    MAP_LEAVE(_map, ERROR_KO);
	}

      *new = (t_vaddr)region;
    }

  /*
   * B)
   */

  else
    {

      /*
       * 2)
       */

      if (region_resize(as, (i_region)old, size, &region) != ERROR_OK)
	MAP_LEAVE(_map, ERROR_KO);

      *new = (t_vaddr)region;
    }

  MAP_LEAVE(_map, ERROR_OK);
}

/*							  [endblock::resize] */

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

  if ((_map = malloc(sizeof(m_map))) == NULL)
    {
      module_call(console, console_message,
		  '!', "map: cannot allocate memory for the map "
		  "manager structure\n");

      return (ERROR_KO);
    }

  memset(_map, 0x0, sizeof(m_map));

  return (ERROR_OK);
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

  free(_map);

  return (ERROR_OK);
}
