/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/region/region-fit.c
 *
 * created       matthieu bucchianeri   [tue jan 10 01:28:36 2006]
 * updated       matthieu bucchianeri   [wed dec 13 14:19:16 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements simple  fitting algorithms for virtual memory
 * management.
 *
 * you can define which algorithm to use with the macro REGION_FIT.
 *
 *  - FIT_FIRST: first fit algorithm - the first large enough space is taken
 *  - FIT_BEST: best fit algorithm - the smaller space is taken
 *  - ...
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
 * the region manager structure.
 */

extern m_region*       region;

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

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

static t_error		region_first_fit(o_as*			as,
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
    {
      if (region->size < size)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      *address = region->start;

      REGION_LEAVE(region, ERROR_NONE);
    }

  if (set_object(as->regions, i, (void**)&head) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if ((head->address - region->start) >= size)
    {
      *address = region->start;

      REGION_LEAVE(region, ERROR_NONE);
    }

  /*
   * 3)
   */

  set_foreach(SET_OPT_FORWARD, as->regions, &i, state)
    {
      o_region*		next;
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
 * this function calls the good algorithm.
 */

t_error			region_space(o_as*		as,
				     t_vsize		size,
				     t_vaddr*		address)
{
  REGION_ENTER(region);

  switch (region->lookup)
    {
      case FIT_FIRST:
	{
	  REGION_LEAVE(region, region_first_fit(as, size, address));
	  break;
	}
      default:
	REGION_LEAVE(region, ERROR_UNKNOWN);
    }
}

/*                                                                 [cut] /k2 */
