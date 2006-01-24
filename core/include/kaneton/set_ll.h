/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/core/include/kaneton/set_ll.h
 *
 * created       julien quintard   [sun jun 19 14:51:33 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:28:19 2006]
 */

#ifndef KANETON_SET_LL_H
#define KANETON_SET_LL_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <kaneton/set.h>
#include <arch/machdep/machdep.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * a node of the linked-list
 */

typedef struct			s_set_ll_node
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
  t_opts			opts;

  t_size			datasz;

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
