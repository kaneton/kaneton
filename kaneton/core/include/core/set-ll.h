/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/set-ll.h
 *
 * created       julien quintard   [wed jun  6 14:25:53 2007]
 * updated       julien quintard   [wed jun  6 14:27:19 2007]
 */

#ifndef GUARD_CORE_SET_LL
#define GUARD_CORE_SET_LL		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/set.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * a node of the linked-list
 */

typedef struct
{
  void*				data;

  struct s_set_ll_node*		prv;
  struct s_set_ll_node*		nxt;
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
