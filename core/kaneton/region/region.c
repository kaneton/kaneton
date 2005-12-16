/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/core/kaneton/region/region.c
 *
 * created       julien quintard   [wed nov 23 09:19:43 2005]
 * updated       julien quintard   [thu dec 15 11:50:21 2005]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX revoir commentaires: description + steps
 *
 * il n'existe pas de conteneur d'objets regions pour tout le kernel.
 * en effet nous voulons retrouver les regions par leurs addresses et
 * pour eviter les collisions il nous faut donc creer des ensembles propres
 * a chaque address space. donc dans l'objet as, il y a un ensemble region
 * qui n'est pas un ensemble d'identifiants comme l'est l'ensemble
 * segment de l'objet as mais bon un ensemble d'objet region.
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * XXX
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

machdep_include(region);

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * the init variable, filled by the bootloader, containing in this case
 * the list of segments to mark used.
 */

extern t_init*		init;

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
 *
 * steps:
 *
 * 1) gets the region object.
 * 2) displays the entry.
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

  cons_msg('#', "    0x%08x [%qd]\n",
	   o->address,
	   o->segid);

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * this function tries to find free space in the region set via the
 * first fit algorithm.
 *
 * steps:
 *
 * 1) gets the first region.
 * 2) tries to find space before the first region.
 * 3) for each region, tries to find space after it.
 * 4) gets the last region.
 * 5) tries to find space after the last region.
 */

t_error			region_first_fit(o_as*			as,
					 t_vsize		size,
					 t_vaddr*		address)
{
  o_region*		current;
  t_state		state;
  o_region*		head;
  o_region*		tail;
  t_iterator		i;

  REGION_ENTER(region);

  /*
   * 1)
   */

  if (set_head(as->regions, &i) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if (set_object(as->regions, i, (void**)&head) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if ((head->address - region->start) >= size)
    {
      *address = head->address + head->size;

      REGION_LEAVE(region, ERROR_NONE);
    }

  /*
   * 3)
   */

  set_foreach(SET_OPT_FORWARD, as->regions, &i, state)
    {
      o_region*	next;
      t_iterator	j;

      if (set_object(as->regions, i, (void**)&current) !=
	  ERROR_NONE)
	{
	  cons_msg('!', "region: cannot find the region object "
		   "corresponding to its identifier\n");

	  REGION_LEAVE(region, ERROR_UNKNOWN);
	}

      if (set_next(as->regions, i, &j) != ERROR_NONE)
	break;

      if (set_object(as->regions, j, (void**)&next) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      if ((next->address - (current->address + current->size)) >= size)
	{
	  *address = current->address + current->size;

	  REGION_LEAVE(region, ERROR_NONE);
	}
    }

  /*
   * 4)
   */

  if (set_tail(as->regions, &i) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if (set_object(as->regions, i, (void**)&tail) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 5)
   */

  if (((region->start + region->size) -
       (tail->address + tail->size)) >= size)
    {
      *address = tail->address + tail->size;

      REGION_LEAVE(region, ERROR_NONE);
    }

  REGION_LEAVE(region, ERROR_UNKNOWN);
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
				       t_opts			opts,
				       t_vaddr			address,
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

  /*
   * 2)
   */

  switch (opts)
    {
    case REGION_OPT_FORCE:
      {
	/* XXX verifier que cette adresse est bonne */

	o.address = address;

	break;
      }
    case REGION_OPT_NONE:
      {
	switch (region->fit)
	  {
	  case FIT_FIRST:
	    {
	      if (segment_first_fit(as, segment->size,
				    &o.address) != ERROR_NONE)
		SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	      break;
	    }

	  default:
	    {
	      REGION_LEAVE(region, ERROR_UNKNOWN);
	    }
	  }

	break;
      }
    default:
      {
	REGION_LEAVE(region, ERROR_UNKNOWN);
      }
    }

  /*
   * 3)
   */

  o.regid = (t_regid)o.address;
  o.segid = segid;
  o.size = segment->size;

  if (set_add(as->regions, &o) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (machdep_call(region, region_reserve, asid, segid,
		   opts, address, regid) != ERROR_NONE)
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
 */

t_error			region_flush(t_asid			asid)
{
  o_as*			as;

  REGION_ENTER(region);

  if (machdep_call(region, region_flush, asid) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if (as_get(asid, &as) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if (set_release(as->regions) != ERROR_NONE)
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
 * 3) reserves the region set which will contain the system's regions.
 * 4) tries to reserve a statistics object.
 * 5) calls the machine-dependent code.
 * 6) if needed, dumps the regions.
 */

t_error			region_init(t_fit			fit,
				    t_vaddr			start,
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

  region->fit = fit;
  region->start = start;
  region->size = size;

  /*
   * 3)
   */

  STATS_RESERVE("region", &region->stats);

  /*
   * 4)
   */

  if (machdep_call(region, region_init, fit, start, size) != ERROR_NONE)
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
 * 4) frees the region manager structure's memory.
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
