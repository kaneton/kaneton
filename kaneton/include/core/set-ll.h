/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/set-ll.h
 *
 * created       julien quintard   [sun jun 19 14:51:33 2005]
 * updated       julien quintard   [sat jul  8 03:17:12 2006]
 */

#ifndef CORE_SET_LL_H
#define CORE_SET_LL_H		1

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct s_set_ll_node	t_set_ll_node;
typedef struct s_set_ll		t_set_ll;
typedef struct s_iterator_ll	t_iterator_ll;

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/set.h>
#include <arch/machdep/machdep.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * a node of the linked-list
 */

struct				s_set_ll_node
{
  void*				data;

  struct s_set_ll_node*		prv;
  struct s_set_ll_node*		nxt;
};

/*
 * specific linked-list data
 */

struct				s_set_ll
{
  t_set_ll_node*		head;
  t_set_ll_node*		tail;
};

/*
 * linked-list iterator
 */

struct				s_iterator_ll
{
  t_set_ll_node*		node;
};

#endif
