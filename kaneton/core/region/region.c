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

#include <libc/libc.h>
#include <kaneton.h>

machine_include(region);

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the region manager structure.
 */

m_region*		region;

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

  REGION_ENTER(region);

  /*
   * 1)
   */

  if (region_get(asid, regid, &o) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  cons_msg('#', "  region %qd in address space %qd:\n",
	   regid,
	   asid);

  /*
   * 2)
   */

  cons_msg('#', "    0x%08x [%qd] (%u) %c\n",
	   o->address,
	   o->segid,
	   o->size,
	   (o->opts & REGION_OPT_PRIVILEGED) ? 'S' : 'U');

  /*
   * 3)
   */

  if (machine_call(region, region_show, asid, regid) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*							    [endblock::show] */

  REGION_LEAVE(region, ERROR_NONE);
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

  REGION_ENTER(region);

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (set_size(o->regions, &size) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 3)
   */

  cons_msg('#', "dumping %qu regions(s) from the region set:\n", size);

  set_foreach(SET_OPT_FORWARD, o->regions, &i, state)
    {
      if (set_object(o->regions, i, (void**)&data) != ERROR_NONE)
	{
	  cons_msg('!', "region: cannot find the region object "
		   "corresponding to its identifier\n");

	  REGION_LEAVE(region, ERROR_UNKNOWN);
	}

      if (region_show(asid, data->regid) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);
    }

  /*							    [endblock::dump] */

  REGION_LEAVE(region, ERROR_NONE);
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
 * 4) call machdep code.
 *						    [endblock::inject::comment]
 */

t_error			region_inject(i_as		asid,
				      o_region*		o,
				      i_region*		regid)
{
  /*							     [block::inject] */

  o_as*			as;

  REGION_ENTER(region);

  ASSERT(o != NULL);
  ASSERT(regid != NULL);
  ASSERT((o->opts & REGION_OPT_INVALID) == 0);
  ASSERT(o->size != 0);

  /*
   * 1)
   */

  if (as_get(asid, &as) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 2)
   */

  o->regid = (i_region)o->address;
  o->opts &= REGION_OPT_FORCE;
  *regid = o->regid;

  /*
   * 3)
   */

  if (set_add(as->regions, o) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (machine_call(region, region_inject, asid, o, regid) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*							  [endblock::inject] */

  REGION_LEAVE(region, ERROR_NONE);
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
 * 6) call the dependent code.
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
  t_perms		perms;

  REGION_ENTER(region);

  ASSERT(size != 0);
  ASSERT(left != NULL);
  ASSERT(right != NULL);

  /*
   * 1)
   */

  if (region_get(asid, regid, &reg) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if (segment_get(reg->segid, &seg) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  perms = seg->perms;

  /*
   * 2)
   */

  if (reg->size <= size)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (segment_get(reg->segid + reg->offset + size, &seg) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if (seg->asid != asid || seg->size < reg->size - size ||
      seg->perms != perms)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if ((new = malloc(sizeof(o_region))) == NULL)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  new->regid = regid + reg->size;
  new->segid = reg->segid + reg->offset + size;
  new->address = (t_vaddr)new->regid;
  new->offset = 0;
  new->opts = reg->opts;
  new->size = reg->size - size;

  if (region_inject(asid, new, &useless) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 5)
   */

  reg->size = size;

  /*
   * 6)
   */

  if (machine_call(region, region_split, asid, regid, size, left, right) !=
      ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  REGION_LEAVE(region, ERROR_NONE);
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

  REGION_ENTER(region);

  ASSERT(size != 0);
  ASSERT(new != NULL);

  /*
   * 1)
   */

  if (as_get(as, &oas) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  set_foreach(SET_OPT_FORWARD, oas->regions, &i, state)
    {
      if (set_object(oas->regions, i, (void**)&reg) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      if (reg->regid == old)
	break;
    }

  if (reg == NULL)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if (reg->size == size)
    {
      *new = old;
      REGION_LEAVE(region, ERROR_NONE);
    }

  if (set_next(oas->regions, i, &next) == ERROR_NONE)
    {
      if (set_object(oas->regions, next, (void**)&next_reg) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      if (reg->address + size >= next_reg->address)
	REGION_LEAVE(region, ERROR_UNKNOWN);
    }
  else
    {
      if (reg->address + size >= region->start + region->size)
	REGION_LEAVE(region, ERROR_UNKNOWN);
    }

  /*
   * 2)
   */

  if (segment_get(reg->segid, &seg) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if (!size || size > seg->size)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 3)
   */

  *new = old;

  /*
   * 4)
   */

  if (machine_call(region, region_resize, as, old, size, new) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 5)
   */

  reg->size = size;

  REGION_LEAVE(region, ERROR_NONE);
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

  REGION_ENTER(region);

  ASSERT(left != right);
  ASSERT(regid != NULL);

  /*
   * 1)
   */

  if (region_get(asid, left, &oleft) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if (region_get(asid, right, &oright) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (oleft->address + oleft->size != oright->address ||
      oleft->segid != oright->segid ||
      oleft->offset + oleft->size != oright->offset ||
      (oleft->opts & REGION_OPT_PRIVILEGED) !=
      (oright->opts & REGION_OPT_PRIVILEGED))
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (as_get(asid, &as) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 4)
   */

  oleft->size += oright->size;

  /*
   * 5)
   */

  if (set_remove(as->regions, right) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 6)
   */

  if (machine_call(region, region_coalesce, asid, left, right, regid) !=
      ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  *regid = left;

  REGION_LEAVE(region, ERROR_NONE);
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
				       t_opts			opts,
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

  REGION_ENTER(region);

  ASSERT((opts & REGION_OPT_INVALID) == 0);
  ASSERT(size != 0);
  ASSERT(regid != NULL);

  /*
   * 1)
   */

  if (as_get(asid, &as) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if (segment_get(segid, &segment) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if (segment->size < size + offset)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if ((o = malloc(sizeof(o_region))) == NULL)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (opts & REGION_OPT_FORCE)
    {
      if (address < region->start || address >= region->start + region->size)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      set_foreach(SET_OPT_FORWARD, as->regions, &i, state)
	{
	  if (set_object(as->regions, i, (void**)&data) != ERROR_NONE)
	    REGION_LEAVE(region, ERROR_UNKNOWN);

	  if (data->address <= address && data->address + data->size > address)
	    REGION_LEAVE(region, ERROR_UNKNOWN);
	}

      o->address = address;
    }
  else
    {
      if (region_space(asid, size, &o->address) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  address = o->address;
  *regid = o->regid = (i_region)o->address;
  o->segid = segid;
  o->size = size;
  o->offset = offset;
  o->opts = opts;

  if (set_add(as->regions, o) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (machine_call(region, region_reserve, asid, segid, offset,
		   opts, address, size, regid) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*							 [endblock::reserve] */

  REGION_LEAVE(region, ERROR_NONE);
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

  REGION_ENTER(region);

  /*
   * 1)
   */

  if (machine_call(region, region_release, asid, regid) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (as_get(asid, &as) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (set_remove(as->regions, regid) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*							 [endblock::release] */

  REGION_LEAVE(region, ERROR_NONE);
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

  REGION_ENTER(region);

  /*
   * 1)
   */

  if (as_get(asid, &as) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 2)
   */

  while (set_head(as->regions, &it) == ERROR_NONE)
    {
      if (set_object(as->regions, it, (void**)&obj) != ERROR_NONE)
	{
	  cons_msg('!', "region: cannot get region to release.\n");

	  REGION_LEAVE(region, ERROR_UNKNOWN);
	}

      if (region_release(asid, *obj) != ERROR_NONE)
	{
	  cons_msg('!', "region: cannot release a region.\n");

	  REGION_LEAVE(region, ERROR_UNKNOWN);
	}
    }

  /*
   * 3)
   */

  if (machine_call(region, region_flush, asid) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*							   [endblock::flush] */

  REGION_LEAVE(region, ERROR_NONE);
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

  REGION_ENTER(region);

  ASSERT(o != NULL);

  if (as_get(asid, &as) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if (set_get(as->regions, regid, (void**)o) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*							     [endblock::get] */

  REGION_LEAVE(region, ERROR_NONE);
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

t_error			region_initialize(t_vaddr		start,
					  t_vsize		size)
{
  /*							 [block::initialize] */

  /*
   * 1)
   */

  ASSERT(size != 0);

  if ((region = malloc(sizeof(m_region))) == NULL)
    {
      cons_msg('!', "region: cannot allocate memory for the region "
	       "manager structure\n");

      return (ERROR_UNKNOWN);
    }

  memset(region, 0x0, sizeof(m_region));

  /*
   * 2)
   */

  region->start = start;
  region->size = size;

  /*
   * 3)
   */

  if (machine_call(region, region_initialize, start, size) !=
      ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);

  /*						      [endblock::initialize] */

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

  if (machine_call(region, region_clean) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  free(region);


  /*							   [endblock::clean] */

  return (ERROR_NONE);
}
