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
 * updated       julien quintard   [wed jun  6 23:42:30 2007]
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
 * a node of the linked-list
 */

typedef struct			t_set_ll_node
{
  void*				data;

  struct t_set_ll_node*		prv;
  struct t_set_ll_node*		nxt;
}				t_set_ll_node;

/*
 * specific linked-list data
 */

typedef struct
{
  t_set_ll_node*		head;
  t_set_ll_node*		tail;
}				t_set_ll;

/*
 * linked-list iterator
 */

typedef struct
{
  t_set_ll_node*		node;
}				t_iterator_ll;

#endif
