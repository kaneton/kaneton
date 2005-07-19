/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * as.h
 * 
 * path          /home/mycure/kaneton/core/include/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:44 2005   mycure
 * last update   Mon Jul 18 11:42:44 2005   mycure
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

/*
 * as object
 */

typedef struct
{
  t_asid			asid;

  t_setid			segments;
  t_setid			regions;
}				o_as;

/*
 * as manager
 */

typedef struct
{
  t_asid			id;

  o_as				kas;

  t_setid			ass;
}				m_as;

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * check
 */

#define as_check(_as_)							\
  {									\
    if ((_as_) == NULL)							\
      return (-1);							\
  }

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
