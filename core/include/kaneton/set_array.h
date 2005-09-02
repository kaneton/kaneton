/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * set_array.h
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sun Jun 19 14:51:33 2005   mycure
 * last update   Fri Sep  2 15:51:39 2005   mycure
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

  t_uint8*			array;
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
