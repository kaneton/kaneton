/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/region/region.c
 *
 * created       julien quintard   [wed nov 23 09:19:43 2005]
 * updated       matthieu bucchianeri   [tue feb  6 22:40:42 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the region manager manages mapping of segments.
 *
 * reserving a region means to map virtual addresses to a segment.
 *
 * like for  segment, region identifiers are the  virtual address they
 * maps.
 *
 * unlike segments, regions  are proper to an address  space: there is
 * one set of region objects for each address space to prevent collisions.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

machine_include(region);

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the region manager structure.
 */

m_region*		_region = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function shows a region.
 *							 [block::show::comment]
 *
 * steps:
 *
 * 1) get the region object.
 * 2) display the entry.
 * 3) call machine dependent code.
 *						      [endblock::show::comment]
 */

t_error			region_show(i_as			asid,
				    i_region			regid)
{
  /*							       [block::show] */

  o_region*		o;

  /*
   * 1)
   */

  if (region_get(asid, regid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the region object");

  module_call(console, console_message,
	      '#', "  region %qd in address space %qd:\n",
	      regid,
	      asid);

  /*
   * 2)
   */

  module_call(console, console_message,
	      '#', "    0x%08x [%qd] (%u) %c\n",
	      o->address,
	      o->segment,
	      o->size,
	      (o->options & REGION_OPTION_PRIVILEGED) ? 'S' : 'U');

  /*
   * 3)
   */

  if (machine_call(region, region_show, asid, regid) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*							    [endblock::show] */

  CORE_LEAVE();
}

/*
 * this function dumps the regions.
 *							 [block::dump::comment]
 *
 * steps:
 *
 * 1) gets the size of the region set.
 * 2) prints the header message.
 * 3) for each entry in the region set, calls the region_show function.
 *						      [endblock::dump::comment]
 */

t_error			region_dump(i_as		asid)
{
  /*							       [block::dump] */

  t_state		state;
  o_as*			o;
  o_region*		data;
  t_setsz		size;
  t_iterator		i;

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  if (set_size(o->regions, &size) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the size of the address space's set "
		"of regions");

  /*
   * 3)
   */

  module_call(console, console_message,
	      '#', "dumping %qu regions(s) from the region set:\n", size);

  set_foreach(SET_OPTION_FORWARD, o->regions, &i, state)
    {
      if (set_object(o->regions, i, (void**)&data) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the region object corresponding "
		    "to its identifier\n");

      if (region_show(asid, data->id) != ERROR_OK)
	CORE_ESCAPE("unable to show the region");
    }

  /*							    [endblock::dump] */

  CORE_LEAVE();
}

/*
 * this function injects a region into an address space.
 *						       [block::inject::comment]
 *
 * steps:
 *
 * 1) get the as object.
 * 2) fill the region id field.
 * 3) add the region to the set.
 * 4) call machine code.
 *						    [endblock::inject::comment]
 */

t_error			region_inject(i_as		asid,
				      o_region*		o,
				      i_region*		regid)
{
  /*							     [block::inject] */

  o_as*			as;

  assert(o != NULL);
  assert(regid != NULL);
  assert((o->options & REGION_OPTION_INVALID) == 0);
  assert(o->size != 0);

  /*
   * 1)
   */

  if (as_get(asid, &as) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  o->id = (i_region)o->address;
  o->options &= REGION_OPTION_FORCE;

  *regid = o->id;

  /*
   * 3)
   */

  if (set_add(as->regions, o) != ERROR_OK)
    CORE_ESCAPE("unable to add the object to the address space's set of "
		"regions");

  /*
   * 4)
   */

  if (machine_call(region, region_inject, asid, o, regid) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*							  [endblock::inject] */

  CORE_LEAVE();
}

/*							      [block::split] */

/*
 * this function splits a region in two regions.
 *
 * steps:
 *
 * 1) get the region object and the associated segment.
 * 2) check if the sizes are correct.
 * 3) check if a valid segment exists and can be used.
 * 4) build the new region.
 * 5) adjust the old region size.
 * 6) set the new left and right identifiers.
 * 7) call the dependent code.
 */

t_error			region_split(i_as			asid,
				     i_region			regid,
				     t_vsize			size,
				     i_region*			left,
				     i_region*			right)
{
  i_region		useless;
  o_region*		reg;
  o_segment*		seg;
  o_region*		new;

  assert(size != 0);
  assert(left != NULL);
  assert(right != NULL);

  /*
   * 1)
   */

  if (region_get(asid, regid, &reg) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the region object");

  if (segment_get(reg->segment, &seg) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  /*
   * 2)
   */

  if (reg->size <= size)
    CORE_ESCAPE("the splitted size is larger than the current region's size");

  /*
   * 3)
   */

  if ((new = malloc(sizeof(o_region))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the region object");

  new->segment = reg->segment;
  new->offset = reg->offset + size;
  new->address = reg->address + size;
  new->options = reg->options;
  new->size = reg->size - size;

  new->id = (i_region)new->address;

  /*
   * 4)
   */

  if (region_inject(asid, new, &useless) != ERROR_OK)
    CORE_ESCAPE("unable to inject the object in the address space's "
		"set of regions");

  /*
   * 5)
   */

  reg->size = size;

  /*
   * 6
   */

  *left = reg->id;
  *right = new->id;

  /*
   * 7)
   */

  if (machine_call(region,
		   region_split,
		   asid,
		   regid,
		   size,
		   left,
		   right) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*							   [endblock::split] */

/*							     [block::resize] */

/*
 * this function resizes a region.
 *
 * steps:
 *
 * 1) check if there is enough space in the address space.
 * 2) check if the segment is large enough.
 * 3) the destination region is the same as the original one.
 * 4) call the machine dependent code.
 *   a) if the region is being expanded, a temporary region is injected so
 *      that the machine-dependent code does not reserve the expanded
 *      part of the future region. then the machine-dependent code is
 *      invoked and finally the temporary region is manually removed from
 *      the set of regions.
 *   b) otherwise, do nothing special.
 * 5) set the new size.
 */

t_error			region_resize(i_as			as,
				      i_region			old,
				      t_vsize			size,
				      i_region*			new)
{

  o_region*		reg = NULL;
  o_region*		next_reg;
  o_segment*		seg;
  o_as*			oas;
  t_iterator		i;
  t_iterator		next;
  t_state		state;

  assert(size != 0);
  assert(new != NULL);

  /*
   * 1)
   */

  if (as_get(as, &oas) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  if (set_locate(oas->regions, old, &i) != ERROR_OK)
    CORE_ESCAPE("unable to locate the object in the address space's "
		"set of regions");

  if (set_object(oas->regions, i, (void**)&reg) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the region object");

  if (reg->size == size)
    {
      *new = old;

      CORE_LEAVE();
    }

  if (set_next(oas->regions, i, &next) == ERROR_TRUE)
    {
      if (set_object(oas->regions, next, (void**)&next_reg) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the next region object");

      if (reg->address + size >= next_reg->address)
	CORE_ESCAPE("unable to resize the region since it would overlap with "
		    "the next region");
    }
  else
    {
      if (reg->address + size >= _region->base + _region->size)
	CORE_ESCAPE("unable to resize the region since it would reach the "
		    "end of the available region space");
    }

  /*
   * 2)
   */

  if (segment_get(reg->segment, &seg) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  if (size == 0)
    CORE_ESCAPE("unable to resize to a null size");

  if (size > seg->size)
    CORE_ESCAPE("unable to resize a region to a size larger than its segment");

  /*
   * 3)
   */

  *new = old;

  /*
   * 4)
   */

  if (reg->size < size)
    {
      /*
       * a)
       */

      o_region*		temporary;
      i_region		id;

      if ((temporary = malloc(sizeof(o_region))) == NULL)
	CORE_ESCAPE("unable to allocate memory for the region");

      temporary->segment = reg->segment;
      temporary->address = reg->address + reg->size;
      temporary->offset = reg->offset + reg->size;
      temporary->size = size - reg->size;
      temporary->options = reg->options;

      if (region_inject(as, temporary, &id) != ERROR_OK)
	CORE_ESCAPE("unable to inject the temporary region in the "
		    "address space's set of regions");

      if (machine_call(region,
		       region_resize,
		       as,
		       old,
		       size,
		       new) != ERROR_OK)
	CORE_ESCAPE("an error occured in the machine");

      if (set_remove(oas->regions, id) != ERROR_OK)
	CORE_ESCAPE("unable to remove the temporary region fro the address "
		    "space's set of regions");
    }
  else
    {
      /*
       * b)
       */

      if (machine_call(region,
		       region_resize,
		       as,
		       old,
		       size,
		       new) != ERROR_OK)
	CORE_ESCAPE("an error occured in the machine");
    }

  /*
   * 5)
   */

  reg->size = size;

  CORE_LEAVE();
}

/*							  [endblock::resize] */

/*							   [block::coalesce] */

/*
 * this function merges two adjacent regions.
 *
 * steps:
 *
 * 1) get both region objets.
 * 2) check if the operation is correct.
 * 3) get the as object.
 * 4) update the region size.
 * 5) remove the right region from the address space.
 * 6) call the machine dependent code.
 */

t_error			region_coalesce(i_as		asid,
					i_region	left,
					i_region	right,
					i_region*	regid)
{
  o_as*			as;
  o_region*		oleft;
  o_region*		oright;

  assert(left != right);
  assert(regid != NULL);

  /*
   * 1)
   */

  if (region_get(asid, left, &oleft) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the region object");

  if (region_get(asid, right, &oright) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the region object");

  /*
   * 2)
   */

  if (oleft->address + oleft->size != oright->address ||
      oleft->segment != oright->segment ||
      oleft->offset + oleft->size != oright->offset ||
      (oleft->options & REGION_OPTION_PRIVILEGED) !=
      (oright->options & REGION_OPTION_PRIVILEGED))
    CORE_ESCAPE("unable to coalesce non-adjacent regions and/or privileged "
		"regions");

  /*
   * 3)
   */

  if (as_get(asid, &as) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 4)
   */

  oleft->size += oright->size;

  /*
   * 5)
   */

  if (set_remove(as->regions, right) != ERROR_OK)
    CORE_ESCAPE("unable to remove the object from the address space's set "
		"of regions");

  /*
   * 6)
   */

  if (machine_call(region,
		   region_coalesce,
		   asid,
		   left,
		   right,
		   regid) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  *regid = left;

  CORE_LEAVE();
}

/*							[endblock::coalesce] */

/*
 * this function reserves a region given the desired segment.
 *						      [block::reserve::comment]
 *
 * steps:
 *
 * 1) gets the address space object given its identifier.
 * 2) find some place.
 * 3) builds the region.
 * 4) calls the machine dependent code.
 *						   [endblock::reserve::comment]
 */

t_error			region_reserve(i_as			asid,
				       i_segment		segid,
				       t_paddr			offset,
				       t_options		options,
				       t_vaddr			address,
				       t_vsize			size,
				       i_region*		regid)
{
  /*							    [block::reserve] */

  o_segment*		segment;
  o_as*			as;
  o_region*		o;
  t_state		state;
  t_iterator		i;
  o_region*		data;

  assert((options & REGION_OPTION_INVALID) == 0);
  assert(size != 0);
  assert(regid != NULL);

  /*
   * 1)
   */

  if (as_get(asid, &as) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  if (segment_get(segid, &segment) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  if (segment->size < (offset + size))
    CORE_ESCAPE("out-of-bound operation");

  if ((o = malloc(sizeof(o_region))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the region object");

  /*
   * 2)
   */

  if (options & REGION_OPTION_FORCE)
    {
      if (address < _region->base ||
	  address >= _region->base + _region->size)
	CORE_ESCAPE("the provided address is either too low or too high");

      set_foreach(SET_OPTION_FORWARD, as->regions, &i, state)
	{
	  if (set_object(as->regions, i, (void**)&data) != ERROR_OK)
	    CORE_ESCAPE("unable to retrieve the region object");

	  if (data->address <= address && data->address + data->size > address)
	    CORE_ESCAPE("unable to reserve the region at this precise "
			"location: seems to be already taken");
	}

      o->address = address;
    }
  else
    {
      if (region_space(asid, size, &o->address) != ERROR_OK)
	CORE_ESCAPE("unable to find available space for the region "
		    "reservation");
    }

  /*
   * 3)
   */

  address = o->address;

  o->id = (i_region)o->address;
  o->segment = segid;
  o->size = size;
  o->offset = offset;
  o->options = options;

  *regid = o->id;

  if (set_add(as->regions, o) != ERROR_OK)
    CORE_ESCAPE("unable to add the object to the address space's set of "
		"regions");

  /*
   * 4)
   */

  if (machine_call(region, region_reserve, asid, segid, offset,
		   options, address, size, regid) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*							 [endblock::reserve] */

  CORE_LEAVE();
}

/*
 * this function releases a region.
 *						      [block::release::comment]
 *
 * steps:
 *
 * 1) calls the machine dependent code.
 * 2) gets the region object.
 * 3) gets the as object from its identifier.
 * 4) removes the region from the address space.
 * 5) removes the region from the region set.
 *						   [endblock::release::comment]
 */

t_error			region_release(i_as			asid,
				       i_region			regid)
{
  /*							    [block::release] */

  o_as*			as;

  /*
   * 1)
   */

  if (machine_call(region, region_release, asid, regid) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  if (as_get(asid, &as) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 3)
   */

  if (set_remove(as->regions, regid) != ERROR_OK)
    CORE_ESCAPE("unable to remove the object from the address space's "
		"set of regions");

  /*							 [endblock::release] */

  CORE_LEAVE();
}

/*
 * this function removes every region that belongs to the address space
 * specified.
 *							[block::flush::comment]
 *
 * steps:
 *
 * 1) gets the address space object.
 * 2) releases each region in the set.
 * 3) calls the machine dependent code.
 *						     [endblock::flush::comment]
 */

t_error			region_flush(i_as			asid)
{
  /*							      [block::flush] */

  o_as*			as;
  t_iterator		it;
  i_region*		obj;

  /*
   * 1)
   */

  if (as_get(asid, &as) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  while (set_head(as->regions, &it) == ERROR_TRUE)
    {
      if (set_object(as->regions, it, (void**)&obj) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      if (region_release(asid, *obj) != ERROR_OK)
	CORE_ESCAPE("unable to release the region");
    }

  /*
   * 3)
   */

  if (machine_call(region, region_flush, asid) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*							   [endblock::flush] */

  CORE_LEAVE();
}

/*
 * this function returns true if the region object exists.
 */

t_error			region_exist(i_as			asid,
				     i_region			regid)
{
  o_as*			as;

  assert(as_get(asid, &as) == ERROR_OK);

  if (set_exist(as->regions, regid) != ERROR_TRUE)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function returns a region object.
 */

t_error			region_get(i_as				asid,
				   i_region			regid,
				   o_region**			o)
{
  /*								[block::get] */

  o_as*			as;

  assert(o != NULL);

  if (as_get(asid, &as) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  if (set_get(as->regions, regid, (void**)o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the region from the address space's set "
		"of regions");

  /*							     [endblock::get] */

  CORE_LEAVE();
}

/*
 * this function initializes the region manager from the init
 * variable containing regions to keep safe.
 *						   [block::initialize::comment]
 *
 * steps:
 *
 * 1) allocates and initializes the region manager structure.
 * 2) initializes the region manager structure fields from the init
 *    structure.
 * 3) calls the machine-dependent code.
 *						[endblock::initialize::comment]
 */

t_error			region_initialize(t_vaddr		base,
					  t_vsize		size)
{
  /*							 [block::initialize] */

  /*
   * 1)
   */

  assert(size != 0);

  if ((_region = malloc(sizeof(m_region))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the region manager's "
		"structure");

  memset(_region, 0x0, sizeof(m_region));

  /*
   * 2)
   */

  _region->base = base;
  _region->size = size;

  /*
   * 3)
   */

  if (machine_call(region, region_initialize, base, size) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*						      [endblock::initialize] */

  CORE_LEAVE();
}

/*
 * this function just reinitializes the region manager.
 *							[block::clean::comment]
 *
 * steps:
 *
 * 1) calls the machine-dependent code.
 * 2) releases the region set.
 *
 *						     [endblock::clean::comment]
 */

t_error			region_clean(void)
{
  /*							      [block::clean] */

  /*
   * 1)
   */

  if (machine_call(region, region_clean) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  free(_region);


  /*							   [endblock::clean] */

  CORE_LEAVE();
}
