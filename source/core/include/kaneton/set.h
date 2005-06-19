/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * set.h
 * 
 * path          /home/mycure/kaneton/core/include/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sun Jun 19 14:51:33 2005   mycure
 * last update   Sun Jun 19 20:13:19 2005   mycure
 */

/*
 * ---------- info ------------------------------------------------------------
 *
 * XXX
 */

#ifndef KANETON_SET_H
#define KANETON_SET_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * the set of segments cannot be computed like other sets. indeed
 * the set manager needs the function malloc to manage the data structure
 * containing set descriptors.
 *
 * for this reason the segments set is handle at the hand.
 *
 * the kernel address space has to be treated like the segments. so the
 * XXX
 */

#define SETID_SEGMENTS		0x0

#define SETID_KSEGMENTS		0x1
#define SETID_KREGIONS		0x2

/*
 * ---------- types -----------------------------------------------------------
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
  o_set				segments;
  o_set				

  o_set*			sets;
}				m_set;

/*
 * ---------- prototypes ------------------------------------------------------
 */

/*
 * prototypes:     ../../kaneton/set/set.c
 */

/*
 * eop
 */

#endif
