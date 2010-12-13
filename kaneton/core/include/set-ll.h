/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/set-ll.h
 *
 * created       julien quintard   [wed jun  6 14:25:53 2007]
 * updated       julien quintard   [sun dec 12 22:44:29 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file supposes it is include by set.h
 */

#ifndef CORE_SET_LL_H
#define CORE_SET_LL_H			1

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * a node of the linked-list.
 */

typedef struct			s_set_ll_node
{
  void*				data;

  struct s_set_ll_node*		previous;
  struct s_set_ll_node*		next;
}				s_set_ll_node;

/*
 * set descriptor's linked-list-specific data.
 */

typedef struct
{
  s_set_ll_node*		head;
  s_set_ll_node*		tail;
}				s_set_ll;

/*
 * the linked-list iterator structure.
 */

typedef struct
{
  s_set_ll_node*		node;
}				s_iterator_ll;

#endif
