/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * kaneton.h
 * 
 * path          /home/mycure/kaneton/core/include
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:44 2005   mycure
 * last update   Mon May 30 16:31:17 2005   mycure
 */

#ifndef KANETON_KANETON_H
#define KANETON_KANETON_H	1

#include <kaneton/errno.h>
#include <kaneton/suberrno.h>

/*
 * types
 */

typedef struct			s_module
{
  char*				name;
  t_paddr			address;
  t_psize			size;
}				t_module;

typedef struct			s_area
{
  t_paddr			address;
  t_psize			size;
}				t_area;

typedef struct			s_kaneton
{
  t_paddr			start;
  t_psize			size;

  t_module*			modules;
  t_uint32			nmodules;

  t_area*			areas;
  t_uint32			nareas;

  /* XXX machdep gdtr, cons(x,y), */
}				t_kaneton;

#endif
