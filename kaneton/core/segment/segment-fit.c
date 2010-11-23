/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.NEW/kaneton/core/segment/segment-fit.c
 *
 * created       julien quintard   [sun jun 10 17:17:15 2007]
 * updated       julien quintard   [mon nov 22 20:37:28 2010]
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

t_error			segment_space(i_as		asid,
				      t_psize		size,
				      t_paddr*		address)
{
  o_segment*		current;
  t_state		state;
  o_segment*		head;
  o_segment*		tail;
  t_iterator		i;
  o_as*			as;

  SEGMENT_ENTER(_segment);

  assert(size != 0);
  assert(address != NULL);

  if (as_get(asid, &as) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 1)
   */

  if (set_head(_segment->segments, &i) != ERROR_OK)
    {
      if (_segment->size < size)
	SEGMENT_LEAVE(_segment, ERROR_KO);

      *address = _segment->start;

      SEGMENT_LEAVE(_segment, ERROR_OK);
    }

  if (set_object(_segment->segments, i, (void**)&head) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 2)
   */

  if ((head->address - _segment->start) >= size)
    {
      *address = _segment->start;

      SEGMENT_LEAVE(_segment, ERROR_OK);
    }

  /*
   * 3)
   */

  set_foreach(SET_OPTION_FORWARD, _segment->segments, &i, state)
    {
      o_segment*	next;
      t_iterator	j;

      if (set_object(_segment->segments, i, (void**)&current) !=
	  ERROR_OK)
	{
	  module_call(console, console_message,
		      '!', "segment: cannot find the segment object "
		      "corresponding to its identifier\n");

	  SEGMENT_LEAVE(_segment, ERROR_KO);
	}

      if (set_next(_segment->segments, i, &j) != ERROR_OK)
	break;

      if (set_object(_segment->segments, j, (void**)&next) != ERROR_OK)
	SEGMENT_LEAVE(_segment, ERROR_KO);

      if ((next->address - (current->address + current->size)) >= size)
	{
	  *address = current->address + current->size;

	  SEGMENT_LEAVE(_segment, ERROR_OK);
	}
    }

  /*
   * 4)
   */

  if (set_tail(_segment->segments, &i) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  if (set_object(_segment->segments, i, (void**)&tail) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 5)
   */

  if (((_segment->start + _segment->size) -
       (tail->address + tail->size)) >= size)
    {
      *address = tail->address + tail->size;

      SEGMENT_LEAVE(_segment, ERROR_OK);
    }

  SEGMENT_LEAVE(_segment, ERROR_KO);
}

#endif
