/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/include/kaneton/set_array.h
 *
 * created       julien quintard   [sun jun 19 14:51:33 2005]
 * updated       matthieu bucchianeri   [mon dec 12 11:38:04 2005]
 */

#ifndef KANETON_SET_ARRAY_H
#define KANETON_SET_ARRAY_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <kaneton/set.h>
#include <arch/machdep/machdep.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * specific array data
 */

typedef struct
{
  t_opts			opts;

  t_size			datasz;
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
