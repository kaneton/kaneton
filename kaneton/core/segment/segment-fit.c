/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...TETON/kaneton/core/segment/segment-fit.c
 *
 * created       julien quintard   [sun jun 10 17:17:15 2007]
 * updated       julien quintard   [sat nov 27 19:30:38 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements simple  fitting algorithms for physical memory
 * management.
 *
 * you can define which algorithm to use with the macro SEGMENT_FIT.
 *
 *  - FIT_FIRST: first fit algorithm - the first large enough space is taken
 *  - FIT_BEST: best fit algorithm - the smaller space is taken
 *  - ...
 */

#if (SEGMENT_ALGORITHM == SEGMENT_ALGORITHM_FIT)

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

machine_include(segment);

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the segment manager structure.
 */

extern m_segment*	_segment;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function tries to find free space in the segment set via the
 * first fit algorithm.
 *
 * steps:
 *
 * 1) gets the first segment.
 * 2) tries to find space before the first segment.
 * 3) for each segment, tries to find space after it.
 * 4) gets the last segment.
 * 5) tries to find space after the last segment.
 *
 */

t_error			segment_fit_first(i_as			asid,
					  t_psize		size,
					  t_paddr*		address)
{
  o_segment*		current;
  t_state		state;
  o_segment*		head;
  o_segment*		tail;
  t_iterator		i;
  o_as*			as;

  assert(size != 0);
  assert(address != NULL);

  if (as_get(asid, &as) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 1)
   */

  if (set_head(_segment->segments, &i) == ERROR_FALSE)
    {
      if (_segment->size < size)
	CORE_ESCAPE("there is not enough memory to satisfy the segment "
		    "reservation");

      *address = _segment->start;

      CORE_LEAVE();
    }

  if (set_object(_segment->segments, i, (void**)&head) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the very first segment object");

  /*
   * 2)
   */

  if ((head->address - _segment->start) >= size)
    {
      *address = _segment->start;

      CORE_LEAVE();
    }

  /*
   * 3)
   */

  set_foreach(SET_OPTION_FORWARD, _segment->segments, &i, state)
    {
      o_segment*	next;
      t_iterator	j;

      if (set_object(_segment->segments,
		     i,
		     (void**)&current) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the segment object corresponding "
		    "to its identifier");

      if (set_next(_segment->segments, i, &j) == ERROR_FALSE)
	break;

      if (set_object(_segment->segments, j, (void**)&next) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the next segment in the set");

      if ((next->address - (current->address + current->size)) >= size)
	{
	  *address = current->address + current->size;

	  CORE_LEAVE();
	}
    }

  /*
   * 4)
   */

  if (set_tail(_segment->segments, &i) == ERROR_FALSE)
    CORE_ESCAPE("unable to locate the last segment in the set");

  if (set_object(_segment->segments, i, (void**)&tail) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  /*
   * 5)
   */

  if (((_segment->start + _segment->size) -
       (tail->address + tail->size)) >= size)
    {
      *address = tail->address + tail->size;

      CORE_LEAVE();
    }

  CORE_ESCAPE("unable to locate enough space between the existing segments "
	      "to satisfy the reservation request");
}

/*
 * this function calls the good algorithm.
 */

t_error			segment_space(i_as			asid,
				      t_psize			size,
				      t_paddr*			address)
{
  o_as*			as;

  assert(size != 0);
  assert(address != NULL);

  switch (SEGMENT_FIT)
    {
    case FIT_FIRST:
      {
	if (segment_fit_first(asid, size, address) != ERROR_OK)
	  CORE_ESCAPE("unable to locate space through the first-fit "
		      "algorithm");

	break;
      }
    default:
      CORE_ESCAPE("unknown segment algorithm '%u'",
		  SEGMENT_FIT);
    }

  CORE_LEAVE();
}

#endif
