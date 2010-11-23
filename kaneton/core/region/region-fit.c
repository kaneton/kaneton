/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/region/region-fit.c
 *
 * created       matthieu bucchianeri   [tue jan 10 01:28:36 2006]
 * updated       matthieu bucchianeri   [tue feb  6 22:46:36 2007]
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

#if (REGION_ALGORITHM == REGION_ALGORITHM_FIT)

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

machine_include(region);

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * the region manager structure.
 */

extern m_region*       _region;

/*
 * ---------- functions -------------------------------------------------------
 */

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
 *
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

  REGION_ENTER(_region);

  /*
   * 1)
   */

  if (set_head(as->regions, &i) != ERROR_OK)
    {
      if (_region->size < size)
	REGION_LEAVE(_region, ERROR_KO);

      *address = _region->start;

      REGION_LEAVE(_region, ERROR_OK);
    }

  if (set_object(as->regions, i, (void**)&head) != ERROR_OK)
    REGION_LEAVE(_region, ERROR_KO);

  /*
   * 2)
   */

  if ((head->address - _region->start) >= size)
    {
      *address = _region->start;

      REGION_LEAVE(_region, ERROR_OK);
    }

  /*
   * 3)
   */

  set_foreach(SET_OPTION_FORWARD, as->regions, &i, state)
    {
      o_region*		next;
      t_iterator	j;

      if (set_object(as->regions, i, (void**)&current) !=
	  ERROR_OK)
	{
	  module_call(console, console_message,
		      '!', "region: cannot find the region object "
		      "corresponding to its identifier\n");

	  REGION_LEAVE(_region, ERROR_KO);
	}

      if (set_next(as->regions, i, &j) != ERROR_OK)
	break;

      if (set_object(as->regions, j, (void**)&next) != ERROR_OK)
	REGION_LEAVE(_region, ERROR_KO);

      if ((next->address - (current->address + current->size)) >= size)
	{
	  *address = current->address + current->size;

	  REGION_LEAVE(_region, ERROR_OK);
	}
    }

  /*
   * 4)
   */

  if (set_tail(as->regions, &i) != ERROR_OK)
    REGION_LEAVE(_region, ERROR_KO);

  if (set_object(as->regions, i, (void**)&tail) != ERROR_OK)
    REGION_LEAVE(_region, ERROR_KO);

  /*
   * 5)
   */

  if (((_region->start + _region->size) -
       (tail->address + tail->size)) >= size)
    {
      *address = tail->address + tail->size;

      REGION_LEAVE(_region, ERROR_OK);
    }

  REGION_LEAVE(_region, ERROR_KO);
}

/*
 * this function calls the good algorithm.
 */

t_error			region_space(i_as		asid,
				     t_vsize		size,
				     t_vaddr*		address)
{
  o_as*			as;

  REGION_ENTER(_region);

  assert(size != 0);
  assert(address != NULL);

  if (as_get(asid, &as) != ERROR_OK)
    REGION_LEAVE(_region, ERROR_KO);

  switch (REGION_FIT)
    {
      case FIT_FIRST:
	{
	  REGION_LEAVE(_region, region_first_fit(as, size, address));
	  break;
	}
      default:
	REGION_LEAVE(_region, ERROR_KO);
    }
}

#endif
