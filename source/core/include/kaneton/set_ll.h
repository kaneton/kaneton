/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * set_ll.h
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sun Jun 19 14:51:33 2005   mycure
 * last update   Mon Jul  4 21:36:00 2005   mycure
 */

#ifndef KANETON_SET_LL_H
#define KANETON_SET_LL_H	1

/*
 * ---------- information -----------------------------------------------------
 *
 * this subpart of set is used to build linked-list data structures.
 *
 * note that this data structure is in fact a doubly linked-list.
 *
 * each set of this type can be used in two ways. the first one ask the
 * set manager to allocate and copy each object to add while the second
 * way tells the set manager to simply include the objects in the set.
 */

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * set type
 */

#define SET_TYPE_LL		0x01

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct			s_set_ll_node
{
  void*				data;

  struct s_set_ll_node*		prv;
  struct s_set_ll_node*		nxt;
}				t_set_ll_node;

typedef struct
{
  t_opts			opts;

  t_size			datasz;

  t_set_ll_node*		ll;
}				t_set_ll;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/set/set_ll.c
 */

#endif
