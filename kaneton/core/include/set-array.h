/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/set-array.h
 *
 * created       julien quintard   [wed jun  6 14:13:43 2007]
 * updated       julien quintard   [sun dec 12 22:44:06 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file supposes it is included by the set.h header file.
 */

#ifndef CORE_SET_ARRAY_H
#define CORE_SET_ARRAY_H		1

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * this structure is included in the set descriptor and provides
 * specific information for the set implementation.
 */

typedef struct
{
  t_size			initsz;

  void**			array;
  t_setsz			arraysz;
}				s_set_array;

/*
 * the implementation-specific iterator.
 */

typedef struct
{
  t_setsz			i;
}				s_iterator_array;

#endif
