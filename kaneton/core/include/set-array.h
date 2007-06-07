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
 * updated       julien quintard   [wed jun  6 23:39:05 2007]
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
 * specific array data
 */

typedef struct
{
  t_size			initsz;

  void**			array;
  t_setsz			arraysz;
}				t_set_array;

/*
 * array iterator
 */

typedef struct
{
  t_setsz			i;
}				t_iterator_array;

#endif
