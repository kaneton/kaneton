/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/set-array.h
 *
 * created       julien quintard   [wed jun  6 14:13:43 2007]
 * updated       julien quintard   [wed jun  6 14:25:38 2007]
 */

#ifndef GUARD_CORE_SET_ARRAY
#define GUARD_CORE_SET_ARRAY		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/set.h>

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
