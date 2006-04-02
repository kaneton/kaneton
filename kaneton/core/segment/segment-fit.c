/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/segment/segment-fit.c
 *
 * created       matthieu bucchianeri   [tue jan 10 01:03:46 2006]
 * updated       julien quintard   [sun apr  2 13:09:02 2006]
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

/*
 * ---------- assignments -----------------------------------------------------
 *
 * students  have  to develop  at  least  the  first-fit algorithm  to
 * allocate physical memory.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

machdep_include(segment);

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * the segment manager structure.
 */

extern m_segment*	segment;

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

/*
 * this function dispatchs to the good fitting method.
 */

t_error			segment_fit(o_as*		as,
				    t_psize		size,
				    t_paddr*		address)
{
  SEGMENT_ENTER(segment);

  switch (segment->fit)
    {
      case FIT_FIRST:
	SEGMENT_LEAVE(segment, segment_first_fit(as, size, address));
	break;
      default:
	SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
    }
}

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
 */

t_error			segment_first_fit(o_as*			as,
					  t_psize		size,
					  t_paddr*		address)
{
  o_segment*		current;
  t_state		state;
  o_segment*		head;
  o_segment*		tail;
  t_iterator		i;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (set_head(segment->segments, &i) != ERROR_NONE)
    {
      if (segment->size < size)
	SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

      *address = segment->start;

      SEGMENT_LEAVE(segment, ERROR_NONE);
    }

  if (set_object(segment->segments, i, (void**)&head) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if ((head->address - segment->start) >= size)
    {
      *address = segment->start;

      SEGMENT_LEAVE(segment, ERROR_NONE);
    }

  /*
   * 3)
   */

  set_foreach(SET_OPT_FORWARD, segment->segments, &i, state)
    {
      o_segment*	next;
      t_iterator	j;

      if (set_object(segment->segments, i, (void**)&current) !=
	  ERROR_NONE)
	{
	  cons_msg('!', "segment: cannot find the segment object "
		   "corresponding to its identifier\n");

	  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
	}

      if (set_next(segment->segments, i, &j) != ERROR_NONE)
	break;

      if (set_object(segment->segments, j, (void**)&next) != ERROR_NONE)
	SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

      if ((next->address - (current->address + current->size)) >= size)
	{
	  *address = current->address + current->size;

	  SEGMENT_LEAVE(segment, ERROR_NONE);
	}
    }

  /*
   * 4)
   */

  if (set_tail(segment->segments, &i) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if (set_object(segment->segments, i, (void**)&tail) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 5)
   */

  if (((segment->start + segment->size) -
       (tail->address + tail->size)) >= size)
    {
      *address = tail->address + tail->size;

      SEGMENT_LEAVE(segment, ERROR_NONE);
    }

  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
}

/*                                                                 [cut] /k2 */
