/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * set.h
 * 
 * path          /home/mycure/source
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sun Jun 19 14:51:33 2005   mycure
 * last update   Mon Jun 20 08:43:29 2005   mycure
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

  o_set*			sets;
}				m_set;

/*
 * ---------- macros ----------------------------------------------------------
 */

#define set_rsv(_type_, ...)						\
  set_rsv_##_type_(...)

#define set_rel(_type_, ...)						\
  set_rel_##_type_(...)

#define set_add(_type_, ...)						\
  set_add_##_type_(...)

#define set_remove(_type_, ...)						\
  set_remove_##_type_(...)

#define set_insert(_type_, ...)						\
  set_insert_##_type_(...)

#define set_insert_head(_type_, ...)					\
  set_insert_head_##_type_(...)

#define set_insert_tail(_type_, ...)					\
  set_insert_tail_##_type_(...)

#define set_insert_before(_type_, ...)					\
  set_insert_before_##_type_(...)

#define set_insert_after(_type_, ...)					\
  set_insert_after_##_type_(...)

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/set/set.c
 */

/*
 * ../../kaneton/set/set.c
 */

int			set_init(void);

/*
 * eop
 */

#endif
