/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/as/as.c
 *
 * created       julien quintard   [tue dec 13 03:05:27 2005]
 * updated       julien quintard   [mon jan 31 13:58:09 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the address space manager provides functions for manipulating address
 * spaces i.e adding/removing segments and regions etc.
 *
 * an address space describes a process' useable memory. each address space
 * is composed of two sets.
 *
 * the first set describes the segments held by this address space, in other
 * words the physical memory. note that since every segment is unique to
 * the whole system, the first set actually contains the identifiers of
 * the segments belonging to this address space. therefore, whenever a
 * segment is reserved for this address space, the segment manager
 * automatically adds the segment identifiers to the address space's set
 * of segments.
 *
 * the second set describes the regions, the virtual areas which reference
 * some, or all, of the address space's segments. unlike segments, regions
 * live in a specific address space and have no meaning for other address
 * spaces or the system itself. therefore, the address space's set of
 * regions contains the actual region objects.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * include the machine-specific definitions required by the core.
 */

machine_include(as);

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the kernel manager.
 */

extern m_kernel		_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the address space manager.
 */

m_as			_as;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function shows a precise address space by displaying its segments
 * and regions.
 *
 * steps:
 *
 * 1) retrieve the address space object.
 * 2) display general information such as the task the address space
 *    lives in, the address space id etc.
 * 3) display the segments held by the address space.
 * 4) display the regions held by the address space.
 * 5) call machine.
 */

t_status		as_show(i_as				id,
				mt_margin			margin)
{
  i_segment*		segment;
  o_region*		region;
  t_setsz		size;
  t_state		st;
  s_iterator		i;
  o_as*			o;

  /*
   * 1)
   */

  if (as_get(id, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "address space:\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin));

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "  core: id(%qd) task(%qd) segments(%qd) regions(%qd)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      o->id,
	      o->task,
	      o->segments,
	      o->regions);

  /*
   * 3)
   */

  if (set_size(o->segments, &size) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the size of the set of segments");

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "    segments: id(%qd) size(%qd)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      o->segments,
	      size);

  set_foreach(SET_OPTION_FORWARD, o->segments, &i, st)
    {
      if (set_object(o->segments, i, (void**)&segment) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the segment identifier");

      if (segment_show(*segment,
		       margin + 3 * MODULE_CONSOLE_MARGIN_SHIFT) != STATUS_OK)
	CORE_ESCAPE("unable to show the segment");
    }

  /*
   * 4)
   */

  if (set_size(o->regions, &size) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the size of the set of regions");

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "    regions: id(%qd) size(%qd)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      o->regions,
	      size);

  set_foreach(SET_OPTION_FORWARD, o->regions, &i, st)
    {
      if (set_object(o->regions, i, (void**)&region) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      if (region_show(o->id,
		      region->id,
		      margin + 3 * MODULE_CONSOLE_MARGIN_SHIFT) != STATUS_OK)
	CORE_ESCAPE("unable to show the region");
    }

  /*
   * 5)
   */

  if (machine_call(as, show, id, margin) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function dumps the address spaces manager.
 *
 * steps:
 *
 * 1) display general information.
 * 2) show the identifier object.
 * 3) retrieve the set's size.
 * 4) for each address space held by the manager, show it.
 * 5) call the machine.
 */

t_status		as_dump(void)
{
  t_setsz		size;
  o_as*			o;
  t_state		st;
  s_iterator		i;

  /*
   * 1)
   */

  module_call(console, message,
	      '#', "address space manager: ass(%qd)\n",
	      _as.ass);

  module_call(console, message,
	      '#', "  core: ass(%qd)\n",
	      _as.ass);

  /*
   * 2)
   */

  if (id_show(&_as.id,
	      2 * MODULE_CONSOLE_MARGIN_SHIFT) != STATUS_OK)
    CORE_ESCAPE("unable to show the identifier object");

  /*
   * 3)
   */

  if (set_size(_as.ass, &size) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the size of the set");

  /*
   * 4)
   */

  module_call(console, message,
	      '#', "    address spaces: id(%qd) size(%qd)\n",
	      _as.ass,
	      size);

  set_foreach(SET_OPTION_FORWARD, _as.ass, &i, st)
    {
      if (set_object(_as.ass, i, (void**)&o) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the address space object");

      if (as_show(o->id,
		  3 * MODULE_CONSOLE_MARGIN_SHIFT) != STATUS_OK)
	CORE_ESCAPE("unable to show the address space object");
    }

  /*
   * 5)
   */

  if (machine_call(as, dump) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function translates a physical address into its virtual counterpart
 * according to the address space's mappings.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the address space object.
 * 2) go through the regions in order to locate the mapped segment in which
 *    the given physical address lies.
 * 3) compute the physical address according to the region's offset and
 *    segment's address.
 * 4) call the machine.
 */

t_status		as_virtual(i_as				id,
				   t_paddr			physical,
				   t_vaddr*			virtual)
{
  o_as*			o;
  o_region*		region;
  o_segment*		segment;
  t_boolean		found;
  t_state		st;
  s_iterator		i;

  /*
   * 0)
   */

  if (virtual == NULL)
    CORE_ESCAPE("the 'virtual' argument is null");

  /*
   * 1)
   */

  if (as_get(id, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  found = BOOLEAN_FALSE;

  set_foreach(SET_OPTION_FORWARD, o->regions, &i, st)
    {
      if (set_object(o->regions, i, (void**)&region) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      if (segment_get(region->segment, &segment) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the segment object");

      if ((segment->address + region->offset) <= physical &&
	  physical < (segment->address + region->offset + region->size))
	{
	  found = BOOLEAN_TRUE;

	  break;
	}
    }

  if (found == BOOLEAN_FALSE)
    CORE_ESCAPE("unable to locate the segment corresponding to the given "
		"physical address");

  /*
   * 3)
   */

  *virtual = region->address +
    (physical - (segment->address + region->offset));

  /*
   * 4)
   */

  if (machine_call(as, virtual, id, physical, virtual) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function translates a virtual address into its physical counterpart
 * according to the given address space mappings.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the address space object.
 * 2) locate the region in which the given virtual address lies.
 * 3) retrieve the segment that the located region maps.
 * 4) compute the physical address.
 * 5) call the machine.
 */

t_status		as_physical(i_as			id,
				    t_vaddr			virtual,
				    t_paddr*			physical)
{
  o_region*		region;
  o_segment*		segment;
  t_uint32		found;
  o_as*			o;
  s_iterator		i;
  t_state		st;

  /*
   * 0)
   */

  if (physical == NULL)
    CORE_ESCAPE("the 'physical' argument is null");

  /*
   * 1)
   */

  if (as_get(id, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  found = BOOLEAN_FALSE;

  set_foreach(SET_OPTION_FORWARD, o->regions, &i, st)
    {
      if (set_object(o->regions, i, (void**)&region) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      if (region->address <= virtual &&
	  virtual < (region->address + region->size))
	{
	  found = BOOLEAN_TRUE;

	  break;
	}

      if (region->address > virtual)
	CORE_ESCAPE("unable to locate the region containing the given "
		    "virtual address");
    }

  if (found == BOOLEAN_FALSE)
    CORE_ESCAPE("unable to locate the region corresponding to the given "
		"virtual address");

  /*
   * 3)
   */

  if (segment_get(region->segment, &segment) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  /*
   * 4)
   */

  *physical = segment->address + region->offset + (virtual - region->address);

  /*
   * 5)
   */

  if (machine_call(as, physical, id, virtual, physical) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function reads data from a virtual location in the address space.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the address space object.
 * 2) retrieve the first region of the address space.
 * 3) locate the first region to read from according to the source virtual
 *    address.
 * 4) compute the offset and size to read from this first region.
 * 5) read data directly from the region's segment.
 * 6) loop throught the remaining regions, if required.
 *   a) retrieve the following region.
 *   b) compute the offset and size for this new region.
 *   c) read data directly from the region's segment.
 * 7) call the machine.
 */

t_status		as_read(i_as				id,
				t_vaddr				source,
				t_vsize				size,
				void*				destination)
{
  t_uint8*		buffer = destination;
  o_region*		region;
  o_region*		previous;
  t_paddr		offset;
  s_iterator		next;
  t_psize		copy;
  o_as*			o;
  s_iterator		i;

  /*
   * 0)
   */

  if (size == 0)
    CORE_LEAVE();

  if (destination == NULL)
    CORE_ESCAPE("the 'destination' argument is null");

  /*
   * 1)
   */

  if (as_get(id, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  if (set_head(o->regions, &i) != TRUE)
    CORE_ESCAPE("unable to locate the set's head");

  if (set_object(o->regions, i, (void**)&region) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the region object");

  /*
   * 3)
   */

  while (region->address + region->size <= source)
    {
      if (set_next(o->regions, i, &next) != TRUE)
	CORE_ESCAPE("unable to locate the next set's object");

      if (set_object(o->regions, next, (void**)&region) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      memcpy(&i, &next, sizeof (s_iterator));
    }

  if (region->address > source)
    CORE_ESCAPE("the located region's address is higher than the "
		"source address");

  /*
   * 4)
   */

  offset = source - region->address;

  copy = region->size - offset;

  if (copy > size)
    copy = size;

  offset += region->offset;

  /*
   * 5)
   */

  if (segment_read(region->segment, offset, buffer, copy) != STATUS_OK)
    CORE_ESCAPE("unable to read from the segment");

  buffer += copy;
  size -= copy;

  if (size == 0)
    CORE_LEAVE();

  /*
   * 6)
   */

  for (;;)
    {
      previous = region;

      /*
       * a)
       */

      if (set_next(o->regions, next, &i) != TRUE)
	CORE_ESCAPE("unable to retrieve the next set's object");

      if (set_object(o->regions, i, (void**)&region) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      if (region->address != previous->address + previous->size)
	CORE_ESCAPE("unable to read from non-adjacent regions");

      /*
       * b)
       */

      offset = region->offset;

      copy = region->size;

      if (copy > size)
	copy = size;

      /*
       * c)
       */

      if (segment_read(region->segment, offset, buffer, copy) != STATUS_OK)
	CORE_ESCAPE("unable to read from the segment");

      buffer += copy;
      size -= copy;

      if (size == 0)
	break;

      memcpy(&next, &i, sizeof (s_iterator));
    }

  /*
   * 7)
   */

  if (machine_call(as, read, id, source, size, destination) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function writes data to an address space.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the address space object.
 * 2) retrieve the first region of the adress space.
 * 3) locate the the first region to write data to.
 * 4) compute the offset and size for this first region.
 * 5) write data to the underlying segment.
 * 6) loop throught the remaining regions, if required.
 *   a) retrieve the following region.
 *   b) compute the offset and size for this new region.
 *   c) write data to the region's segment.
 * 7) call the machine.
 */

t_status		as_write(i_as				id,
				 const void*			source,
				 t_vsize			size,
				 t_vaddr			destination)
{
  const t_uint8*	buffer = source;
  o_region*		region;
  o_region*		previous;
  t_paddr		offset;
  s_iterator		next;
  t_psize		copy;
  o_as*			o;
  s_iterator		i;

  /*
   * 0)
   */

  if (size == 0)
    CORE_LEAVE();

  if (source == NULL)
    CORE_ESCAPE("the 'source' argument is null");

  /*
   * 1)
   */

  if (as_get(id, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  if (set_head(o->regions, &i) != TRUE)
    CORE_ESCAPE("unable to locate the set's head");

  if (set_object(o->regions, i, (void**)&region) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the region object");

  /*
   * 3)
   */

  while (region->address + region->size <= destination)
    {
      if (set_next(o->regions, i, &next) != TRUE)
	CORE_ESCAPE("unable to locate the next set's object");

      if (set_object(o->regions, next, (void**)&region) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      memcpy(&i, &next, sizeof (s_iterator));
    }

  if (region->address > destination)
    CORE_ESCAPE("the located region's address is higher than the "
		"destination address");

  /*
   * 4)
   */

  offset = destination - region->address;

  copy = region->size - offset;

  if (copy > size)
    copy = size;

  offset += region->offset;

  /*
   * 5)
   */

  if (segment_write(region->segment, offset, buffer, copy) != STATUS_OK)
    CORE_ESCAPE("unable to write to the segment");

  buffer += copy;
  size -= copy;

  if (size == 0)
    CORE_LEAVE();

  /*
   * 6)
   */

  for (;;)
    {
      previous = region;

      /*
       * a)
       */

      if (set_next(o->regions, next, &i) != TRUE)
	CORE_ESCAPE("unable to locate the next set's object");

      if (set_object(o->regions, i, (void**)&region) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      if (region->address != previous->address + previous->size)
	CORE_ESCAPE("unable to write to non-adjacent regions");

      /*
       * b)
       */

      offset = region->offset;

      copy = region->size;

      if (copy > size)
	copy = size;

      /*
       * c)
       */

      if (segment_write(region->segment, offset, buffer, copy) != STATUS_OK)
	CORE_ESCAPE("unable to write to the segment");

      buffer += copy;
      size -= copy;

      if (size == 0)
	break;

      memcpy(&next, &i, sizeof (s_iterator));
    }

  /*
   * 7)
   */

  if (machine_call(as, write, id, source, size, destination) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function copies data from an address space to another.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the address space object.
 * 2) locate the first region to copy data from.
 * 3) locate the first region to copy data to.
 * 4) compute the start offsets and sizes for both the source and
 *    destination regions.
 *   a) perform the copy.
 *   b) proceed to the following source region, if necessary.
 *   c) proceed to the followgin destination region, if necessary.
 * 5) call the machine.
 */

t_status		as_copy(i_as			source_id,
				t_vaddr			source_address,
				i_as			destination_id,
				t_vaddr			destination_address,
				t_vsize			size)
{
  o_as*			source_o;
  o_region*		source_region;
  o_segment*		source_segment;
  o_as*			destination_o;
  o_region*		destination_region;
  o_segment*		destination_segment;
  s_iterator		source_i;
  s_iterator		source_next;
  s_iterator		destination_i;
  s_iterator		destination_next;
  t_paddr		source_offset;
  t_paddr		destination_offset;
  t_psize		copy;
  t_psize		source_copy;
  t_psize		destination_copy;
  o_region*		previous;
  s_iterator		next;

  /*
   * 0)
   */

  if (size == 0)
    CORE_LEAVE();

  /*
   * 1)
   */

  if (as_get(source_id, &source_o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  if (as_get(destination_id, &destination_o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  if (set_head(source_o->regions, &source_i) != TRUE)
    CORE_ESCAPE("unable to locate the set's head");

  if (set_object(source_o->regions,
		 source_i,
		 (void**)&source_region) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the region object");

  while (source_region->address + source_region->size <= source_address)
    {
      if (set_next(source_o->regions,
		   source_i,
		   &source_next) != TRUE)
	CORE_ESCAPE("unable to locate the next set's object");

      if (set_object(source_o->regions,
		     source_next,
		     (void**)&source_region) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      memcpy(&source_i, &source_next, sizeof (s_iterator));
    }

  if (source_region->address > source_address)
    CORE_ESCAPE("the located region's address is higher the source "
		"virtual address");

  if (segment_get(source_region->segment, &source_segment) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  /*
   * 3)
   */

  if (set_head(destination_o->regions, &destination_i) != TRUE)
    CORE_ESCAPE("unable to locate the set's head");

  if (set_object(destination_o->regions,
		 destination_i,
		 (void**)&destination_region) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the region object");

  while (destination_region->address + destination_region->size <=
	 destination_address)
    {
      if (set_next(destination_o->regions,
		   destination_i,
		   &destination_next) != TRUE)
	CORE_ESCAPE("unable to locate the next set's object");

      if (set_object(destination_o->regions,
		     destination_next,
		     (void**)&destination_region) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      memcpy(&destination_i, &destination_next, sizeof (s_iterator));
    }

  if (destination_region->address > destination_address)
    CORE_ESCAPE("the located region's address is higher than the "
		"destination virtual address");

  if (segment_get(destination_region->segment,
		  &destination_segment) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  /*
   * 4)
   */

  source_copy = source_region->size -
    (source_address - source_region->address);
  destination_copy = destination_region->size -
    (destination_address - destination_region->address);
  source_offset = source_region->offset +
    (source_address - source_region->address);
  destination_offset = destination_region->offset +
    (destination_address - destination_region->address);

  for (;;)
    {
      if (source_copy < destination_copy)
	copy = source_copy;
      else
	copy = destination_copy;

      if (copy > size)
	copy = size;

      /*
       * a)
       */

      if (segment_copy(destination_region->segment,
		       destination_offset,
		       source_region->segment,
		       source_offset,
		       copy) != STATUS_OK)
	CORE_ESCAPE("unable to copy from one segment to another");

      source_offset += copy;
      destination_offset += copy;

      size -= copy;

      source_copy -= copy;
      destination_copy -= copy;

      if (!size)
	break;

      /*
       * b)
       */

      if (source_copy == 0)
	{
	  previous = source_region;

	  if (set_next(source_o->regions, source_i, &next) != TRUE)
	    CORE_ESCAPE("unable to locate the next set's object");

	  if (set_object(source_o->regions,
			 next,
			 (void**)&source_region) != STATUS_OK)
	    CORE_ESCAPE("unable to retrieve the region object");

	  if (source_region->address != previous->address + previous->size)
	    CORE_ESCAPE("unable to copy from non-adjacent regions");

	  memcpy(&source_i, &next, sizeof (s_iterator));

	  if (segment_get(source_region->segment,
			  &source_segment) != STATUS_OK)
	    CORE_ESCAPE("unable to retrieve the segment object");

	  source_offset = source_region->offset;

	  source_copy = source_region->size;
	}

      /*
       * c)
       */


      if (destination_copy == 0)
	{
	  previous = destination_region;

	  if (set_next(destination_o->regions,
		       destination_i,
		       &next) != TRUE)
	    CORE_ESCAPE("unable to locate the next set's object");

	  if (set_object(destination_o->regions,
			 next,
			 (void**)&destination_region) != STATUS_OK)
	    CORE_ESCAPE("unable to retrieve the region object");

	  if (destination_region->address !=
	      previous->address + previous->size)
	    CORE_ESCAPE("unable to copy to non-adjacent regions");

	  memcpy(&destination_i, &next, sizeof (s_iterator));

	  if (segment_get(destination_region->segment,
			  &destination_segment) != STATUS_OK)
	    CORE_ESCAPE("unable to retrieve the segment object");

	  destination_offset = destination_region->offset;
	  destination_copy = destination_region->size;
	}
    }

  /*
   * 5)
   */

  if (machine_call(as, copy,
		   source_id, source_address,
		   destination_id, destination_address,
		   size) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function reserves an address space for the given task and returns
 * the freshly created address space's identifier.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) reserve an identifier for the address space object.
 * 2) retrieve the task object.
 * 3) return an error if the task already possesses an address space.
 * 4) assign the address space to the task.
 * 5) initialize the address space object.
 * 6) reserve a set of segments.
 * 7) reserve a set of regions.
 * 8) add the new address space object into the set of address spaces
 * 9) call the machine.
 */

t_status		as_reserve(i_task			task,
				   i_as*			id)
{
  o_task*		target;
  o_as			o;

  /*
   * 0)
   */

  if (id == NULL)
    CORE_ESCAPE("the 'id' argument is null");

  /*
   * 1)
   */

  if (id_reserve(&_as.id, id) != STATUS_OK)
    CORE_ESCAPE("unable to reserve an identifier");

  /*
   * 2)
   */

  if (task_get(task, &target) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 3)
   */

  if (target->as != ID_UNUSED)
    CORE_ESCAPE("the task already possesses an address space");

  /*
   * 4)
   */

  target->as = *id;

  /*
   * 5)
   */

  memset(&o, 0x0, sizeof (o_as));

  o.id = *id;
  o.task = target->id;

  /*
   * 6)
   */

  if (set_reserve(array,
		  SET_OPTION_SORT | SET_OPTION_ALLOCATE,
		  AS_SEGMENTS_INITSZ,
		  sizeof (i_segment),
		  &o.segments) != STATUS_OK)
    CORE_ESCAPE("unable to reserve the set of segments");

  /*
   * 7)
   */

  if (set_reserve(array,
		  SET_OPTION_SORT | SET_OPTION_FREE,
		  AS_REGIONS_INITSZ,
		  sizeof (o_region),
		  &o.regions) != STATUS_OK)
    CORE_ESCAPE("unable to reserve the set of regions");

  /*
   * 8)
   */

  if (set_add(_as.ass, &o) != STATUS_OK)
    CORE_ESCAPE("unable to add the object to the set of address spaces");

  /*
   * 9)
   */

  if (machine_call(as, reserve, task, id) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function releases an address space.
 *
 * steps:
 *
 * 1) call the machine.
 * 2) retrieve the address space object.
 * 3) retrieve the address space's task object and reset the task's
 *    address space.
 * 4) release the address space object identifier.
 * 5) flush and release the set of regions.
 * 6) flush and release the set of segments.
 * 7) removes the address space object from the address space set.
 */

t_status		as_release(i_as				id)
{
  o_task*		task;
  o_as*			o;

  /*
   * 1)
   */

  if (machine_call(as, release, id) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  if (as_get(id, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 3)
   */

  if (task_get(o->task, &task) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  task->as = ID_UNUSED;

  /*
   * 4)
   */

  if (id_release(&_as.id, o->id) != STATUS_OK)
    CORE_ESCAPE("unable to release the identifier");

  /*
   * 5)
   */

  if (region_flush(o->id) != STATUS_OK)
    CORE_ESCAPE("unable to flush the regions");

  if (set_release(o->regions) != STATUS_OK)
    CORE_ESCAPE("unable to release the set of regions");

  /*
   * 6)
   */

  if (segment_flush(o->id) != STATUS_OK)
    CORE_ESCAPE("unable to flush the segments");

  if (set_release(o->segments) != STATUS_OK)
    CORE_ESCAPE("unable to release the set of segments");

  /*
   * 7)
   */

  if (set_remove(_as.ass, o->id) != STATUS_OK)
    CORE_ESCAPE("unable to remove the object from the set of address spaces");

  CORE_LEAVE();
}

/*
 * this function returns true if the given address space object exists.
 */

t_bool			as_exist(i_as				id)
{
  if (set_exist(_as.ass, id) != TRUE)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function retrieves an address space object from the set of
 * address spaces.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the object from the set of address spaces.
 */

t_status		as_get(i_as				id,
			       o_as**				object)
{
  /*
   * 0)
   */

  if (object == NULL)
    CORE_ESCAPE("the 'object' argument is null");

  /*
   * 1)
   */

  if (set_get(_as.ass, id, (void**)object) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the object from the set of "
		"address spaces");

  CORE_LEAVE();
}

/*
 * this function initializes the address space manager.
 *
 * steps:
 *
 * 1) display a message.
 * 2) initialize the address space manager's structure.
 * 3) initialize the identifier object in order to be able to generate
 *    the address spaces' identifier.
 * 4) reserve the set which will contain the future address spaces.
 * 5) calls the machine.
 */

t_status		as_initialize(void)
{
  /*
   * 1)
   */

  module_call(console, message,
	      '+', "initializing the address space manager\n");

  /*
   * 2)
   */

  memset(&_as, 0x0, sizeof (m_as));

  /*
   * 3)
   */

  if (id_build(&_as.id) != STATUS_OK)
    CORE_ESCAPE("unable to initialize the identifier object");

  /*
   * 4)
   */

  if (set_reserve(ll,
		  SET_OPTION_ALLOCATE | SET_OPTION_SORT,
		  sizeof (o_as),
		  &_as.ass) != STATUS_OK)
    CORE_ESCAPE("unable to reserve the set of address spaces");

  /*
   * 5)
   */

  if (machine_call(as, initialize) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function cleans the address space manager.
 *
 * steps:
 *
 * 1) display a message.
 * 2) call the machine.
 * 3) flush the address spaces. note that the kernel address space is
 *    never released in order to keep the kernel running.
 * 4) release the set of address spaces.
 * 5) destroy the identifier object.
 */

t_status		as_clean(void)
{
  o_as*			o;
  s_iterator		i;

  /*
   * 1)
   */

  module_call(console, message,
	      '+', "cleaning the address space manager\n");

  /*
   * 2)
   */

  if (machine_call(as, clean) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 3)
   */

  while (set_head(_as.ass, &i) == TRUE)
    {
      if (set_object(_as.ass, i, (void**)&o) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the object from the set of "
		    "address spaces");

      if (o->id == _kernel.as)
	{
	  if (set_remove(_as.ass, o->id) != STATUS_OK)
	    CORE_ESCAPE("unable to remove the kernel address space identifier "
			"from the set of address spaces");
	}
      else
	{
	  if (as_release(o->id) != STATUS_OK)
	    CORE_ESCAPE("unable to release the address space object");
	}
    }

  /*
   * 4)
   */

  if (set_release(_as.ass) != STATUS_OK)
    CORE_ESCAPE("unable to release the set of address spaces");

  /*
   * 5)
   */

  if (id_destroy(&_as.id) != STATUS_OK)
    CORE_ESCAPE("unable to destroy the identifier object");

  CORE_LEAVE();
}
