/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/as/as.c
 *
 * created       julien quintard   [tue dec 13 03:05:27 2005]
 * updated       matthieu bucchianeri   [tue apr  3 14:53:38 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the address space manager manages address spaces.
 *
 * an address space describes process' useable memory. each address space
 * is composed of two sets.
 *
 * the first describes the segments held by this address space, in other
 * words the physical memory.
 *
 * the latter describes the regions, the virtual areas which reference
 * some segments.
 *
 * a task can give its address space to another with as_give().
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

machine_include(as);

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*	_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the address space manager variable.
 */

m_as*			_as = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function shows a precise address space.
 *
 * steps:
 *
 * 1) get the address space object from its identifier.
 * 2) display the segments held by the address space.
 * 3) display the regions held by the address space.
 * 4) call machine dependent code.
 */

t_error			as_show(i_as				id)
{
  i_segment		segment;
  i_region		region;
  t_state		st;
  t_iterator		i;
  o_as*			o;

  /*
   * 1)
   */

  if (as_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  module_call(console, console_message, '#',
	      "  address space %qd in task %qd:\n",
	      id,
	      o->task);

  /*
   * 2)
   */

  module_call(console, console_message,
	      '#', "    segments:\n");

  set_foreach(SET_OPTION_FORWARD, o->segments, &i, st)
    {
      if (set_object(o->segments, i, (void**)&segment) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the segment object");

      module_call(console, console_message,
		  '#', "      %qu\n", segment);
    }

  /*
   * 3)
   */

  module_call(console, console_message,
	      '#', "    regions:\n");

  set_foreach(SET_OPTION_FORWARD, o->regions, &i, st)
    {
      if (set_object(o->regions, i, (void**)&region) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      module_call(console, console_message,
		  '#', "      %qu\n", region);
    }

  /*
   * 4)
   */

  if (machine_call(as, as_show, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function dumps the address spaces managed by the kernel.
 *
 * steps:
 *
 * 1) gets the set's size.
 * 2) for each address space hold by the address space set, dumps
 *    the address space identifier.
 */

t_error			as_dump(void)
{
  t_setsz		size;
  o_as*			o;
  t_state		st;
  t_iterator		i;

  /*
   * 1)
   */

  if (set_size(_as->ass, &size) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the size of the set");

  /*
   * 2)
   */

  module_call(console, console_message,
	      '#', "dumping %qu address space(s):\n", size);

  set_foreach(SET_OPTION_FORWARD, _as->ass, &i, st)
    {
      if (set_object(_as->ass, i, (void**)&o) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the address space object");

      if (as_show(o->id) != ERROR_OK)
	CORE_ESCAPE("unable to show the address space object");
    }

  CORE_LEAVE();
}

/*
 * this function gives an address space from a task to another.
 *
 * steps:
 *
 * 1) gets the address space object given its identifier.
 * 2) gets the source task object.
 * 3) gets the destination task object.
 * 4) swaps the address space identifier.
 * 5) calls the machine-dependent code.
 */

t_error			as_give(i_as				id,
				i_task				task)
{
  o_task*		from;
  o_task*		to;
  o_as*			o;

  /*
   * 1)
   */

  if (as_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  if (task_get(o->task, &from) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 3)
   */

  if (task_get(task, &to) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  if (to->as != ID_UNUSED)
    CORE_ESCAPE("the task already possesses an address space");

  /*
   * 4)
   */

  from->as = ID_UNUSED;
  to->as = id;

  o->task = task;

  /*
   * 5)
   */

  if (machine_call(as, as_give, id, task) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function translate a physical address to its virtual address.
 */

t_error			as_vaddr(i_as				id,
				 t_paddr			physical,
				 t_vaddr*			virtual)
{
  o_as*			o;
  o_region*		region;
  o_segment*		segment;
  t_boolean		found;
  t_state		st;
  t_iterator		i;

  assert(virtual != NULL);

  /*
   * 1)
   */

  if (as_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  found = BOOLEAN_FALSE;

  set_foreach(SET_OPTION_FORWARD, o->regions, &i, st)
    {
      if (set_object(o->regions, i, (void**)&region) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      if (segment_get(region->segment, &segment) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the segment object");

      if (segment->address + region->offset <= physical &&
	  physical < segment->address + region->offset + region->size)
	{
	  found = BOOLEAN_TRUE;

	  break;
	}
    }

  /*
   * 3)
   */

  if (found == BOOLEAN_FALSE)
    CORE_ESCAPE("unable to locate the segment corresponding to the given "
		"physical address");

  /*
   * 4)
   */

  *virtual = region->address - region->offset + (physical - segment->address);

  /*
   * 5)
   */

  if (machine_call(as, as_vaddr, id, physical, virtual) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function translate a virtual address to its physical one.
 *
 * steps:
 *
 * 1) get the region object.
 * 2) look for the good region.
 * 3) check if the region was found.
 * 4) compute the physical address.
 * 5) call dependent code.
 */

t_error			as_paddr(i_as				id,
				 t_vaddr			virtual,
				 t_paddr*			physical)
{
  o_region*		region;
  o_segment*		segment;
  t_uint32		found;
  o_as*			o;
  t_iterator		i;
  t_state		st;

  assert(physical != NULL);

  /*
   * 1)
   */

  if (as_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  found = BOOLEAN_FALSE;

  set_foreach(SET_OPTION_FORWARD, o->regions, &i, st)
    {
      if (set_object(o->regions, i, (void**)&region) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      if (region->address <= virtual &&
	  virtual < region->address + region->size)
	{
	  found = BOOLEAN_TRUE;

	  break;
	}

      if (region->address > virtual)
	CORE_ESCAPE("unable to locate the region containing the given "
		    "virtual address");
    }

  /*
   * 3)
   */

  if (found == BOOLEAN_FALSE)
    CORE_ESCAPE("unable to locate the region corresponding to the given "
		"virtual address");

  /*
   * 4)
   */

  if (segment_get(region->segment, &segment) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  *physical = segment->address + region->offset + (virtual - region->address);

  /*
   * 5)
   */

  if (machine_call(as, as_paddr, id, virtual, physical) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * read data from an as. this function supports contiguous regions
 * mapped on non-contiguous segments.
 *
 * steps:
 *
 * 1) get the address space object.
 * 2) get the first region of the as.
 * 3) look for the first region to copy.
 * 4) compute the offset and size.
 * 5) read data from the segment.
 * 6) loop throught the remaining regions (if needed).
 * 7) get the next region (contiguously).
 * 8) compute the offset and size.
 * 9) read data from the segment.
 */

t_error			as_read(i_as				id,
				t_vaddr				source,
				t_vsize				size,
				void*				destination)
{
  t_uint8*		buffer = destination;
  o_region*		region;
  o_region*		previous;
  t_paddr		offset;
  t_iterator		next;
  t_psize		copy;
  o_as*			o;
  t_iterator		i;

  assert(size != 0);

  /*
   * 1)
   */

  if (as_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  if (set_head(o->regions, &i) != ERROR_TRUE)
    CORE_ESCAPE("unable to locate the set's head");

  if (set_object(o->regions, i, (void**)&region) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the region object");

  /*
   * 3)
   */

  while (region->address + region->size <= source)
    {
      if (set_next(o->regions, i, &next) != ERROR_TRUE)
	CORE_ESCAPE("unable to locate the next set's object");

      if (set_object(o->regions, next, (void**)&region) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      memcpy(&i, &next, sizeof (t_iterator));
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

  if (segment_read(region->segment, offset, buffer, copy) != ERROR_OK)
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
       * 7)
       */

      if (set_next(o->regions, next, &i) != ERROR_TRUE)
	CORE_ESCAPE("unable to retrieve the next set's object");

      if (set_object(o->regions, i, (void**)&region) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      if (region->address != previous->address + previous->size)
	CORE_ESCAPE("unable to read from non-adjacent regions");

      /*
       * 8)
       */

      offset = region->offset;

      copy = region->size;

      if (copy > size)
	copy = size;

      /*
       * 9)
       */

      if (segment_read(region->segment, offset, buffer, copy) != ERROR_OK)
	CORE_ESCAPE("unable to read from the segment");

      buffer += copy;
      size -= copy;

      if (size == 0)
	break;

      memcpy(&next, &i, sizeof (t_iterator));
    }

  CORE_LEAVE();
}

/*
 * write data to an as. this function supports contiguous regions
 * mapped on non-contiguous segments.
 *
 * steps:
 *
 * 1) get the address space object.
 * 2) get the first region of the as.
 * 3) look for the first region to copy.
 * 4) compute the offset and size.
 * 5) write data to the segment.
 * 6) loop throught the remaining regions (if needed).
 * 7) get the next region (contiguously).
 * 8) compute the offset and size.
 * 9) write data to the segment.
 */

t_error			as_write(i_as				id,
				 const void*			source,
				 t_vsize			size,
				 t_vaddr			destination)
{
  const t_uint8*	buffer = source;
  o_region*		region;
  o_region*		previous;
  t_paddr		offset;
  t_iterator		next;
  t_psize		copy;
  o_as*			o;
  t_iterator		i;

  assert(size != 0);

  /*
   * 1)
   */

  if (as_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  if (set_head(o->regions, &i) != ERROR_TRUE)
    CORE_ESCAPE("unable to locate the set's head");

  if (set_object(o->regions, i, (void**)&region) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the region object");

  /*
   * 3)
   */

  while (region->address + region->size <= destination)
    {
      if (set_next(o->regions, i, &next) != ERROR_TRUE)
	CORE_ESCAPE("unable to locate the next set's object");

      if (set_object(o->regions, next, (void**)&region) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      memcpy(&i, &next, sizeof (t_iterator));
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

  if (segment_write(region->segment, offset, buffer, copy) != ERROR_OK)
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
       * 7)
       */

      if (set_next(o->regions, next, &i) != ERROR_TRUE)
	CORE_ESCAPE("unable to locate the next set's object");

      if (set_object(o->regions, i, (void**)&region) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      if (region->address != previous->address + previous->size)
	CORE_ESCAPE("unable to write to non-adjacent regions");

      /*
       * 8)
       */

      offset = region->offset;

      copy = region->size;

      if (copy > size)
	copy = size;

      /*
       * 9)
       */

      if (segment_write(region->segment, offset, buffer, copy) != ERROR_OK)
	CORE_ESCAPE("unable to write to the segment");

      buffer += copy;
      size -= copy;

      if (size == 0)
	break;

      memcpy(&next, &i, sizeof (t_iterator));
    }

  CORE_LEAVE();
}

/*
 * this function copies from an as to another. this function supports
 * contiguous regions mapped on non-contiguous segments.
 *
 * steps:
 *
 * 1) get address space object.
 * 2) look for the first region of the source area.
 * 3) look for the first region of the destination area.
 * 4) compute start offsets and size to copy.
 * 5) do the copy.
 * 6) goto the next source region if necessary.
 * 7) goto the next destination region if necessary.
 */

t_error			as_copy(i_as			source_id,
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
  t_iterator		source_i;
  t_iterator		source_next;
  t_iterator		destination_i;
  t_iterator		destination_next;
  t_paddr		source_offset;
  t_paddr		destination_offset;
  t_psize		copy;
  t_psize		source_copy;
  t_psize		destination_copy;
  o_region*		previous;
  t_iterator		next;

  assert(size != 0);
  assert(source_id != destination_id);

  /*
   * 1)
   */

  if (as_get(source_id, &source_o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  if (as_get(destination_id, &destination_o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  if (set_head(source_o->regions, &source_i) != ERROR_TRUE)
    CORE_ESCAPE("unable to locate the set's head");

  if (set_object(source_o->regions,
		 source_i,
		 (void**)&source_region) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the region object");

  while (source_region->address + source_region->size <= source_address)
    {
      if (set_next(source_o->regions,
		   source_i,
		   &source_next) != ERROR_TRUE)
	CORE_ESCAPE("unable to locate the next set's object");

      if (set_object(source_o->regions,
		     source_next,
		     (void**)&source_region) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      memcpy(&source_i, &source_next, sizeof (t_iterator));
    }

  if (source_region->address > source_address)
    CORE_ESCAPE("the located region's address is higher the source "
		"virtual address");

  if (segment_get(source_region->segment, &source_segment) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  /*
   * 3)
   */

  if (set_head(destination_o->regions, &destination_i) != ERROR_TRUE)
    CORE_ESCAPE("unable to locate the set's head");

  if (set_object(destination_o->regions,
		 destination_i,
		 (void**)&destination_region) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the region object");

  while (destination_region->address + destination_region->size <=
	 destination_address)
    {
      if (set_next(destination_o->regions,
		   destination_i,
		   &destination_next) != ERROR_TRUE)
	CORE_ESCAPE("unable to locate the next set's object");

      if (set_object(destination_o->regions,
		     destination_next,
		     (void**)&destination_region) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      memcpy(&destination_i, &destination_next, sizeof (t_iterator));
    }

  if (destination_region->address > destination_address)
    CORE_ESCAPE("the located region's address is higher than the "
		"destination virtual address");

  if (segment_get(destination_region->segment,
		  &destination_segment) != ERROR_OK)
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
       * 5)
       */

      if (segment_copy(destination_region->segment,
		       destination_offset,
		       source_region->segment,
		       source_offset,
		       copy) != ERROR_OK)
	CORE_ESCAPE("unable to copy from one segment to another");

      source_offset += copy;
      destination_offset += copy;

      size -= copy;

      source_copy -= copy;
      destination_copy -= copy;

      if (!size)
	break;

      /*
       * 6)
       */

      if (source_copy == 0)
	{
	  previous = source_region;

	  if (set_next(source_o->regions, source_i, &next) != ERROR_TRUE)
	    CORE_ESCAPE("unable to locate the next set's object");

	  if (set_object(source_o->regions,
			 next,
			 (void**)&source_region) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve the region object");

	  if (source_region->address != previous->address + previous->size)
	    CORE_ESCAPE("unable to copy from non-adjacent regions");

	  memcpy(&source_i, &next, sizeof (t_iterator));

	  if (segment_get(source_region->segment,
			  &source_segment) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve the segment object");

	  source_offset = source_region->offset;

	  source_copy = source_region->size;
	}

      /*
       * 7)
       */


      if (destination_copy == 0)
	{
	  previous = destination_region;

	  if (set_next(destination_o->regions,
		       destination_i,
		       &next) != ERROR_TRUE)
	    CORE_ESCAPE("unable to locate the next set's object");

	  if (set_object(destination_o->regions,
			 next,
			 (void**)&destination_region) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve the region object");

	  if (destination_region->address !=
	      previous->address + previous->size)
	    CORE_ESCAPE("unable to copy to non-adjacent regions");

	  memcpy(&destination_i, &next, sizeof (t_iterator));

	  if (segment_get(destination_region->segment,
			  &destination_segment) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve the segment object");

	  destination_offset = destination_region->offset;
	  destination_copy = destination_region->size;
	}
    }

  CORE_LEAVE();
}

/*
 * this function clones an address space.
 *
 * steps:
 *
 * 1) gets the source address space object given its identifier.
 * 2) gets the task object.
 * 3) reserves the cloned address space object.
 * 4) gets the destination address space object previously reserved.
 * 5) prepare a temporary set for the mapping between segments and regions.
 * 6) clones the segment set from the source address space object.
 * 7) clones the region set from the source address space object.
 * 8) calls the machine-dependent code.
 */

t_error			as_clone(i_as				id,
				 i_task				task,
				 i_as*				as)
{
  i_segment		pool[2];
  i_segment*		map = pool;
  o_task*		target;
  o_as*			from;
  o_as*			to;
  t_state		st;
  t_iterator		i;
  t_setsz		setsz;
  i_set			mapping;

  assert(as != NULL);

  /*
   * 1)
   */

  if (as_get(id, &from) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  if (task_get(task, &target) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  if (target->as != ID_UNUSED)
    CORE_ESCAPE("the task already possesses an address space");

  /*
   * 3)
   */

  if (as_reserve(task, as) != ERROR_OK)
    CORE_ESCAPE("unable to reserve an address space");

  /*
   * 4)
   */

  if (as_get(*as, &to) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 5)
   */

  if (set_size(from->segments, &setsz) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the size of the set");

  if (set_reserve(array, SET_OPTION_ALLOC, setsz,
		  sizeof (pool), &mapping) != ERROR_OK)
    CORE_ESCAPE("unable to reserve a set");

  /*
   * 6)
   */

  set_foreach(SET_OPTION_FORWARD, from->segments, &i, st)
    {
      i_segment		needless;
      i_segment*	data;
      o_segment*	o;

      if (set_object(from->segments, i, (void**)&data) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the segment identifier from the set");

      if (segment_get(*data, &o) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the segment object");

      if (o->type == SEGMENT_TYPE_SYSTEM)
	continue;

      if (segment_clone(to->id, *data, &needless) != ERROR_OK)
	CORE_ESCAPE("unable to clone the segment");

      map[0] = *data;
      map[1] = needless;

      if (set_add(mapping, map) != ERROR_OK)
	CORE_ESCAPE("unable to add the segment to the set of segments");
    }

  /*
   * 7)
   */

  set_foreach(SET_OPTION_FORWARD, from->regions, &i, st)
    {
      i_region		needless;
      o_region*		data;

      if (set_object(from->regions, i, (void**)&data) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the region identifier from the set");

      if (set_exist(mapping, data->segment) == ERROR_FALSE)
	continue;

      if (set_get(mapping, data->segment, (void**)&map) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the segment identifier");

      if (region_reserve(to->id,
			 map[1],
			 data->offset,
			 data->options | REGION_OPTION_FORCE,
			 data->address,
			 data->size,
			 &needless) != ERROR_OK)
	CORE_ESCAPE("unable to reserve the region");
    }

  /*
   * 8)
   */

  if (machine_call(as, as_clone, id, task, as) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function reserves an address space.
 *
 * steps:
 *
 * 1) gets the task object.
 * 2) initializes the address space object.
 * 3) reserves an identifier for the address space object.
 * 4) reserves the set of segments for the new address space object.
 * 5) reserves the set of regions for the new address space object.
 * 6) adds the new address space object in the address space set.
 * 7) sets the address space into the task object.
 * 8) calls the machine-dependent code.
 */

t_error			as_reserve(i_task			task,
				   i_as*			id)
{
  o_task*		target;
  o_as			o;

  assert(id != NULL);

  /*
   * 1)
   */

  if (task_get(task, &target) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  if (target->as != ID_UNUSED)
    CORE_ESCAPE("the task alread possesses an address space");

  /*
   * 2)
   */

  memset(&o, 0x0, sizeof(o_as));

  o.task = task;

  o.segments = ID_UNUSED;
  o.regions = ID_UNUSED;

  /*
   * 3)
   */

  if (id_reserve(&_as->id, id) != ERROR_OK)
    CORE_ESCAPE("unable to reserve an identifier");

  o.id = *id;

  /*
   * 4)
   */

  if (set_reserve(array,
		  SET_OPTION_SORT | SET_OPTION_ALLOC,
		  AS_SEGMENTS_INITSZ,
		  sizeof(i_segment),
		  &o.segments) != ERROR_OK)
    CORE_ESCAPE("unable to reserve the set of segments");

  /*
   * 5)
   */

  if (set_reserve(array,
		  SET_OPTION_SORT | SET_OPTION_FREE,
		  AS_REGIONS_INITSZ,
		  sizeof(o_region),
		  &o.regions) != ERROR_OK)
    CORE_ESCAPE("unable to reserve the set of regions");

  /*
   * 6)
   */

  if (set_add(_as->ass, &o) != ERROR_OK)
    CORE_ESCAPE("unable to add the object to the set of address spaces");

  /*
   * 7)
   */

  target->as = *id;

  /*
   * 8)
   */

  if (machine_call(as, as_reserve, task, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function releases an address space.
 *
 * steps:
 *
 * 1) gets the address space object given its identifier.
 * 2) gets the task object and removes the address space from it.
 * 3) releases the address space object identifier.
 * 4) releases the address space object's set of regions.
 * 5) releases the address space object's set of segments.
 * 6) calls the machine-dependent code.
 * 7) removes the address space object from the address space set.
 */

t_error			as_release(i_as			id)
{
  o_task*		task;
  o_as*			o;

  /*
   * 1)
   */

  if (as_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  if (task_get(o->task, &task) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  task->as = ID_UNUSED;

  /*
   * 3)
   */

  if (id_release(&_as->id, o->id) != ERROR_OK)
    CORE_ESCAPE("unable to release the identifier");

  /*
   * 4)
   */

  if (region_flush(o->id) != ERROR_OK)
    CORE_ESCAPE("unable to flush the regions");

  if (set_release(o->regions) != ERROR_OK)
    CORE_ESCAPE("unable to release the set of regions");

  /*
   * 5)
   */

  if (segment_flush(o->id) != ERROR_OK)
    CORE_ESCAPE("unable to flush the segments");

  if (set_release(o->segments) != ERROR_OK)
    CORE_ESCAPE("unable to release the set of segments");

  /*
   * 6)
   */

  if (machine_call(as, as_release, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 7)
   */

  if (set_remove(_as->ass, o->id) != ERROR_OK)
    CORE_ESCAPE("unable to remove the object from the set of address spaces");

  CORE_LEAVE();
}

/*
 * this function returns true if the given address space object exists.
 */

t_error			as_exist(i_as				id)
{
  if (set_exist(_as->ass, id) != ERROR_TRUE)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function gets an address space object from the address space
 * set according to its identifier.
 */

t_error			as_get(i_as				id,
			       o_as**				o)
{
  assert(o != NULL);

  if (set_get(_as->ass, id, (void**)o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the object from the set of "
		"address spaces");

  CORE_LEAVE();
}

/*
 * this function initializes the address space manager.
 *
 * this function takes care of initializing and builing the kernel
 * address space.
 *
 * steps:
 *
 * 1) allocates and initializes the address space manager structure.
 * 2) initializes the identifier object to be able to generate
 *    the address space identifiers.
 * 3) reserves the addres space set which will contain the address space
 *    built later.
 * 4) calls the machine-dependent code.
 */

t_error			as_initialize(void)
{
  /*
   * 1)
   */

  if ((_as = malloc(sizeof(m_as))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the address space "
		"manager's structure");

  memset(_as, 0x0, sizeof(m_as));

  /*
   * 2)
   */

  if (id_build(&_as->id) != ERROR_OK)
    CORE_ESCAPE("unable to initialize the identifier object");

  /*
   * 3)
   */

  if (set_reserve(ll,
		  SET_OPTION_ALLOC | SET_OPTION_SORT,
		  sizeof(o_as),
		  &_as->ass) != ERROR_OK)
    CORE_ESCAPE("unable to reserve the set of address spaces");

  /*
   * 4)
   */

  if (machine_call(as, as_initialize) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function just reinitializes the address space manager.
 *
 * steps:
 *
 * 1) calls the machine-dependent code.
 * 2) releases the address space's set.
 * 3) destroys the id object.
 * 4) frees the address space manager structure's memory.
 */

t_error			as_clean(void)
{
  o_as*			o;
  t_iterator		i;

  /*
   * 1)
   */

  if (machine_call(as, as_clean) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  while (set_head(_as->ass, &i) == ERROR_TRUE)
    {
      if (set_object(_as->ass, i, (void**)&o) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the object from the set of "
		    "address spaces");

      if (o->id == _kernel->as)
	set_remove(_as->ass, o->id);
      else
	if (as_release(o->id) != ERROR_OK)
	  CORE_ESCAPE("unable to release the address space object");
    }

  if (set_release(_as->ass) != ERROR_OK)
    CORE_ESCAPE("unable to release the set of address spaces");

  /*
   * 3)
   */

  if (id_destroy(&_as->id) != ERROR_OK)
    CORE_ESCAPE("unable to destroy the identifier object");

  /*
   * 4)
   */

  free(_as);

  CORE_LEAVE();
}
