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
 * last update   Mon Jun 20 21:36:11 2005   mycure
 */

#ifndef KANETON_SET_LL_H
#define KANETON_SET_LL_H	1

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

#define SET_TYPE_LL		0x01	/* linked-list */

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
  t_set_ll_node*		list;
}				t_set_ll;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/set/set_ll.c
 */

#endif
