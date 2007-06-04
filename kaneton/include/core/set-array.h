/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/set_array.h
 *
 * created       julien quintard   [sun jun 19 14:51:33 2005]
 * updated       julien quintard   [sun apr  2 13:50:56 2006]
 */

#ifndef CORE_SET_ARRAY_H
#define CORE_SET_ARRAY_H	1

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct s_set_array	t_set_array;
typedef struct s_iterator_array	t_iterator_array;

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/set.h>
#include <arch/machdep/machdep.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * specific array data
 */

struct				s_set_array
{
  t_size			initsz;

  void**			array;
  t_setsz			arraysz;
};

/*
 * array iterator
 */

struct				s_iterator_array
{
  t_setsz			i;
};

#endif
