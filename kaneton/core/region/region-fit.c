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
 */

t_error			region_fit_first(i_as			asid,
					 t_vsize		size,
					 t_vaddr*		address)
{
  o_region*		current;
  t_state		state;
  o_region*		head;
  o_region*		tail;
  o_as*			as;
  t_iterator		i;

  /*
   * XXX
   */

  if (as_get(asid, &as) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 1)
   */

  if (set_head(as->regions, &i) != ERROR_TRUE)
    {
      if (_region->size < size)
	CORE_ESCAPE("there is not enough memory to satisfy the reservation "
		    "request");

      *address = _region->base;

      CORE_LEAVE();
    }

  if (set_object(as->regions, i, (void**)&head) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the address space's very first region");

  /*
   * 2)
   */

  if ((head->address - _region->base) >= size)
    {
      *address = _region->base;

      CORE_LEAVE();
    }

  /*
   * 3)
   */

  set_foreach(SET_OPTION_FORWARD, as->regions, &i, state)
    {
      o_region*		next;
      t_iterator	j;

      if (set_object(as->regions, i, (void**)&current) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      if (set_next(as->regions, i, &j) != ERROR_TRUE)
	break;

      if (set_object(as->regions, j, (void**)&next) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the next region object");

      if ((next->address - (current->address + current->size)) >= size)
	{
	  *address = current->address + current->size;

	  CORE_LEAVE();
	}
    }

  /*
   * 4)
   */

  if (set_tail(as->regions, &i) != ERROR_TRUE)
    CORE_ESCAPE("unable to locate the address space's last region");

  if (set_object(as->regions, i, (void**)&tail) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the region object");

  /*
   * 5)
   */

  if (((_region->base + _region->size) -
       (tail->address + tail->size)) >= size)
    {
      *address = tail->address + tail->size;

      CORE_LEAVE();
    }

  CORE_ESCAPE("unable to locate enough space between the existing regions "
	      "to satisfy the reservation request");
}

/*
 * this function calls the good algorithm.
 */

t_error			region_space(i_as		asid,
				     t_vsize		size,
				     t_vaddr*		address)
{
  o_as*			as;

  assert(size != 0);
  assert(address != NULL);

  switch (REGION_FIT)
    {
    case FIT_FIRST:
      {
	if (region_fit_first(asid, size, address) != ERROR_OK)
	  CORE_ESCAPE("unable to locate space through the first-fit "
		      "algorithm");

	break;
      }
    default:
      CORE_ESCAPE("unknown region algorithm '%u'",
		  REGION_FIT);
    }

  CORE_LEAVE();
}

#endif
