/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * as.h
 * 
 * path          /home/mycure/kaneton/core/include
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:44 2005   mycure
 * last update   Sun Jun 19 22:04:07 2005   mycure
 */

#ifndef KANETON_AS_H
#define KANETON_AS_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  t_asid			asid;
}				t_as;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/as/as.c
 */

/*
 * ../../kaneton/as/as.c
 */

void			as_dump(void);


int			as_init(void);

/*
 * eop
 */

#endif
