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
 * last update   Fri Jul 22 15:21:26 2005   mycure
 */

#ifndef KANETON_AS_H
#define KANETON_AS_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <kaneton/id.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

#define AS_SEGMENTS_INITSZ	0x4
#define AS_REGIONS_INITSZ	0x4

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
  o_id				id;

  t_asid			kas;

  t_setid			container;
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

int			as_dump(void);

int			as_rsv(t_asid*				asid);

int			as_rel(t_asid				asid);

int			as_get(t_asid				asid,
			       o_as**				o);

int			as_init(void);


/*
 * eop
 */

#endif
