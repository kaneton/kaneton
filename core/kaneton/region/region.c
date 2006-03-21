/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/core/kaneton/region/region.c
 *
 * created       julien quintard   [wed nov 23 09:19:43 2005]
 * updated       matthieu bucchianeri   [tue mar 21 16:34:51 2006]
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
 * ---------- assignments -----------------------------------------------------
 *
 * students have to develop  entirely this manager, including the code
 * for one architecture (ia32-virtual or ia32-segment).
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

machdep_include(region);

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

/*                                                                  [cut] k2 */

/*
 * this function shows a region.
 *
 * steps:
 *
 * 1) get the region object.
 * 2) display the entry.
 * 3) call machine dependent code.
 */

t_error			region_show(t_asid			asid,
				    t_regid			regid)
{
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

  cons_msg('#', "    0x%08x [%qd] (%u)\n",
	   o->address,
	   o->segid,
	   o->size);

  /*
   * 3)
   */

  if (machdep_call(region, region_show, asid, regid) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * this function dumps the regions.
 *
 * steps:
 *
 * 1) gets the size of the region set.
 * 2) prints the header message.
 * 3) for each entry in the region set, calls the region_show function.
 */

t_error			region_dump(t_asid		asid)
{
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

  cons_msg('#', "dumping %qu regions(s) from the region container:\n", size);

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

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * this function injects a region into an address space.
 *
 * steps:
 *
 * 1) get the as object.
 * 2) fill the region id field.
 * 3) add the region to the container.
 * 4) call machdep code.
 */

t_error			region_inject(t_asid		asid,
				      o_region*		o)
{
  o_as*			as;

  REGION_ENTER(region);

  /*
   * 1)
   */

  if (as_get(asid, &as) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 2)
   */

  o->regid = (t_regid)o->address;

  /*
   * 3)
   */

  if (set_add(as->regions, o) != ERROR_NONE)
    REGION_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (machdep_call(region, region_inject, asid, o) != ERROR_NONE)
    REGION_LEAVE(as, ERROR_UNKNOWN);

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * this function reserves a region given the desired segment.
 *
 * steps:
 *
 * 1) gets the address space object given its identifier.
 * 2) chooses the correct fit.
 * 3) builds the region.
 * 4) calls the machine dependent code.
 */

t_error			region_reserve(t_asid			asid,
				       t_segid			segid,
				       t_paddr			offset,
				       t_opts			opts,
				       t_vaddr			address,
				       t_vsize			size,
				       t_regid*			regid)
{
  o_segment*		segment;
  o_as*			as;
  o_region		o;

  REGION_ENTER(region);

  /*
   * 1)
   */

  if (as_get(asid, &as) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if (segment_get(segid, &segment) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if (opts & REGION_OPT_MAPALL)
    {
      offset = 0;
      size = segment->size;
      opts &= ~REGION_OPT_MAPALL;
    }

  /*
   * 2)
   */

  if (opts & REGION_OPT_FORCE)
    {
      if (address < region->start || address >= region->start + region->size)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      o.address = address;
    }
  else
    if (opts & REGION_OPT_NONE)
      {
	if (region_fit(as, size, &o.address) != ERROR_NONE)
	  REGION_LEAVE(region, ERROR_UNKNOWN);
      }
    else
      REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 3)
   */

  *regid = o.regid = (t_regid)o.address;
  o.segid = segid;
  o.size = size;
  o.offset = offset;

  if (set_add(as->regions, &o) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (machdep_call(region, region_reserve, asid, segid, offset,
		   opts, address, size, regid) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * this function releases a region.
 *
 * steps:
 *
 * 1) calls the machine dependent code.
 * 2) gets the region object.
 * 3) gets the as object from its identifier.
 * 4) removes the region from the address space.
 * 5) removes the region from the region container.
 */

t_error			region_release(t_asid			asid,
				       t_regid			regid)
{
  o_as*			as;

  REGION_ENTER(region);

  /*
   * 1)
   */

  if (machdep_call(region, region_release, asid, regid) != ERROR_NONE)
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

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * this function removes every region that belongs to the address space
 * specified.
 *
 * steps:
 *
 * 1) gets the address space object.
 * 2) releases each region in the set.
 * 3) calls the machine dependent code.
 */

t_error			region_flush(t_asid			asid)
{
  o_as*			as;
  t_state		state;
  t_iterator		it;
  t_regid*		obj;

  REGION_ENTER(region);

  /*
   * 1)
   */

  if (as_get(asid, &as) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 2)
   */

  set_foreach(SET_OPT_FORWARD, as->regions, &it, state)
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

  if (machdep_call(region, region_flush, asid) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * this function returns a region object.
 */

t_error			region_get(t_asid			asid,
				   t_regid			regid,
				   o_region**			o)
{
  o_as*			as;

  REGION_ENTER(region);

  if (as_get(asid, &as) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if (set_get(as->regions, regid, (void**)o) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * this function initialises the region manager from the init
 * variable containing regions to keep safe.
 *
 * steps:
 *
 * 1) allocates and initialises the region manager structure.
 * 2) initialises the region manager structure fields from the init
 *    structure.
 * 3) tries to reserve a statistics object.
 * 4) calls the machine-dependent code.
 */

t_error			region_init(t_vaddr			start,
				    t_vsize			size)
{
  /*
   * 1)
   */

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

  region->fit = REGION_FIT;
  region->start = start;
  region->size = size;

  /*
   * 3)
   */

  STATS_RESERVE("region", &region->stats);

  /*
   * 4)
   */

  if (machdep_call(region, region_init, REGION_FIT, start, size) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  return (ERROR_NONE);
}

/*
 * this function just reinitialises the region manager.
 *
 * steps:
 *
 * 1) calls the machine-dependent code.
 * 2) releases the stats object.
 * 3) releases the region container.
 */

t_error			region_clean(void)
{
  /*
   * 1)
   */

  if (machdep_call(region, region_clean) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 2)
   */

  STATS_RELEASE(region->stats);

  /*
   * 3)
   */

  free(region);

  return (ERROR_NONE);
}

/*                                                                 [cut] /k2 */
