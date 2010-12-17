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
 * the map manager provides a layer on top of the segment and region managers.
 *
 * a map is an abstraction which couples a segment with a region. therefore,
 * reserving a map implies reserving a segment of the map's size but also
 * reserving a region mapping the segment.
 *
 * this manager offers three basic functionalities: reserving, releasing
 * and resizing maps.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * include the machine-specific definitions required by the core.
 */

machine_include(map);

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the map manager's structure.
 */

m_map*		_map = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function reserve a map i.e a segment mapped through a region.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) reserve a segment.
 * 2) reserve a region depending on the options.
 *   A) the region must be reserved at a precise location...
 *     a) either force the region at a specific location or;
 *   B) otherwise...
 *     a) reserve a region anywhere.
 *     b) retrieve the new region object.
 *     c) set the virtual address.
 * 3) call the machine.
 */

t_error			map_reserve(i_as			as,
				    t_options			options,
				    t_vsize			size,
				    t_permissions		permissions,
				    t_vaddr*			address)
{
  i_segment		segment;
  i_region		region;
  o_region*		o;

  /*
   * 0)
   */

  if (options & MAP_OPTION_INVALID)
    CORE_ESCAPE("the 'options' argument is invalid");

  if (permissions & PERMISSION_INVALID)
    CORE_ESCAPE("the 'permissions' argument is invalid");

  if (address == NULL)
    CORE_ESCAPE("the 'address' argument is null");

  if (size == 0)
    CORE_LEAVE();

  /*
   * 1)
   */

  if (segment_reserve(as, size, permissions, &segment) != ERROR_OK)
    CORE_ESCAPE("unable to reserve a segment");

  /*
   * 2)
   */

  if (options & MAP_OPTION_FORCE)
    {
      /*
       * A)
       */

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
	CORE_ESCAPE("unable to reserve the region");
    }
  else
    {
      /*
       * B)
       */

      /*
       * a)
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
	CORE_ESCAPE("unable to reserve a region");

      /*
       * b)
       */

      if (region_get(as, region, &o) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      /*
       * c)
       */

      *address = o->address;
    }

  /*
   * 3)
   */

  if (machine_call(map, reserve,
		   as, options, size, permissions, address) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function releases a map i.e the couple of segment and region.
 *
 * steps:
 *
 * 1) call the machine.
 * 2) locate the region in which the given address lies.
 * 3) retrieve the region object.
 * 4) save the segment associated with this region.
 * 5) release the region.
 * 6) release the segment.
 */

t_error			map_release(i_as			as,
				    t_vaddr			address)
{
  i_region		region;
  i_segment		segment;
  o_region*		o;

  /*
   * 1)
   */

  if (machine_call(map, release, as, address) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  if (region_locate(as, address, &region) == ERROR_FALSE)
    CORE_ESCAPE("unable to locate the region in which the address lies");

  /*
   * 3)
   */

  if (region_get(as, region, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the region object");

  /*
   * 4)
   */

  segment = o->segment;

  /*
   * 5)
   */

  if (region_release(as, region) != ERROR_OK)
    CORE_ESCAPE("unable to release the region");

  /*
   * 6)
   */

  if (segment_release(segment) != ERROR_OK)
    CORE_ESCAPE("unable to release the segment");

  CORE_LEAVE();
}

/*							     [block::resize] */

/*
 * this function resizes a given map identified by the map's virtual address.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) locate the region in which the given address lies.
 * 2) retrieve the region object.
 * 3) resize the segment mapped by the region.
 * 4) resize the region depending on the fact that the segment has been
 *    relocated.
 *   A) if the segment has been relocated...
 *     a) save the current region's offset and options.
 *     b) the mapping is now invalid, so release the region.
 *     c) try to remap the region at the same address.
 *       i) if it is impossible and that mapping at this address was part of
 *          the request, then return an error.
 *       ii) otherwise, try to reserve another region anywhere.
 *     d) retrieve the reserved region object.
 *     e) set the new virtual address.
 *   B) otherwise...
 *     a) resize the region, possibly leading to a new region being reserved.
 *     b) retrieve the new region object, which could very much be the same
 *        as the previous one.
 *     c) set the new virtual address.
 * 5) call the machine.
 */

t_error			map_resize(i_as				as,
				   t_vaddr			old,
				   t_vsize			size,
				   t_vaddr*			new)
{
  t_options		options;
  t_paddr		offset;
  i_segment		segment;
  i_region		region;
  o_region*		o;

  /*
   * 0)
   */

  if (new == NULL)
    CORE_ESCAPE("the 'new' argument is null");

  if (size == 0)
    CORE_ESCAPE("unable to resize a map to a size of zero");

  /*
   * 1)
   */

  if (region_locate(as, old, &region) == ERROR_FALSE)
    CORE_ESCAPE("unable to locate the region in which the address lies");

  /*
   * 2)
   */

  if (region_get(as, region, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the region object");

  /*
   * 3)
   */

  if (segment_resize(o->segment,
		     size,
		     SEGMENT_OPTION_NONE,
		     &segment) != ERROR_OK)
    CORE_ESCAPE("unable to resize the segment");

  /*
   * 4)
   */

  if (segment != o->segment)
    {
      /*
       * A)
       */

      /*
       * a)
       */

      offset = o->offset;
      options = o->options;

      /*
       * b)
       */

      if (region_release(as, o->id) != ERROR_OK)
	CORE_ESCAPE("unable to release the region");

      /*
       * c)
       */

      if (region_reserve(as,
			 segment,
			 offset,
			 options | REGION_OPTION_FORCE,
			 old,
			 size,
			 &region) != ERROR_OK)
	{
	  /*
	   * i)
	   */

	  if (options & REGION_OPTION_FORCE)
	    CORE_ESCAPE("unable to resize while maintaining the region at "
			"the previous location");

	  /*
	   * ii)
	   */

	  if (region_reserve(as,
			     segment,
			     offset,
			     options,
			     0,
			     size,
			     &region) != ERROR_OK)
	    CORE_ESCAPE("unable to reserve a region");
	}

      /*
       * d)
       */

      if (region_get(as, region, &o) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      /*
       * e)
       */

      *new = o->address;
    }
  else
    {
      /*
       * B)
       */

      /*
       * a)
       */

      if (region_resize(as, o->id, size, &region) != ERROR_OK)
	CORE_ESCAPE("unable to resize the region");

      /*
       * b)
       */

      if (region_get(as, region, &o) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      /*
       * c)
       */

      *new = o->address;
    }

  /*
   * 5)
   */

  if (machine_call(map, resize, as, old, size, new) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*							  [endblock::resize] */

/*
 * this function initialize the map manager.
 *
 * steps:
 *
 * 1) display a message.
 * 2) allocate and initialize the map manager's structure.
 * 3) call the machine.
 */

t_error			map_initialize(void)
{
  /*
   * 1)
   */

  module_call(console, message,
	      '+', "initializing the map manager\n");

  /*
   * 2)
   */

  if ((_map = malloc(sizeof (m_map))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the map manager's structure");

  memset(_map, 0x0, sizeof (m_map));

  /*
   * 3)
   */

  if (machine_call(map, initialize) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function release the map manager.
 *
 * steps:
 *
 * 1) display a message.
 * 2) call the machine.
 * 3) free the manager's structure.
 */

t_error			map_clean(void)
{
  /*
   * 1)
   */

  module_call(console, message,
	      '+', "cleaning the map manager\n");

  /*
   * 2)
   */

  if (machine_call(map, clean) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 3)
   */

  free(_map);

  CORE_LEAVE();
}
