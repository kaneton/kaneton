/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * set.h
 * 
 * path          /home/mycure/kaneton/core/include
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sun Jun 19 14:51:33 2005   mycure
 * last update   Sun Jun 19 16:13:25 2005   mycure
 */

#ifndef KANETON_SET_H
#define KANETON_SET_H		1

/*
 * dependencies
 */

#include <arch/machdep/machdep.h>

/*
 * types
 */

/*
 * a set object
 */

typedef struct
{
  t_setid			setid;
}				o_set;

/*
 * the set manager
 */

typedef struct
{
  o_set*			sets;
}				m_set;

#endif
