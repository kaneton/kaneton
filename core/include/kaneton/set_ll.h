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
 * last update   Fri Jan 29 06:43:50 1999   mycure
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

  t_set_ll_node*		head;
  t_set_ll_node*		tail;
}				t_set_ll;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/set/set_ll.c
 */

/*
 * ../../kaneton/set/set_ll.c
 */

int			set_head_ll(t_setid			setid,
				    t_iterator*			iterator);

int			set_tail_ll(t_setid			setid,
				    t_iterator*			iterator);

int			set_prev_ll(t_setid			setid,
				    t_iterator			current,
				    t_iterator*			previous);

int			set_next_ll(t_setid			setid,
				    t_iterator			current,
				    t_iterator*			next);

int			set_insert_head_ll(t_setid		setid,
					   void*		data);

int			set_insert_tail_ll(t_setid		setid,
					   void*		data);

int			set_insert_before_ll(t_setid		setid,
					     t_iterator		iterator,
					     void*		data);

int			set_insert_after_ll(t_setid		setid,
					    t_iterator		iterator,
					    void*		data);

int			set_add_ll(t_setid			setid,
				   void*			data);

int			set_remove_ll(t_setid			setid,
				      t_id			id);

int			set_object_ll(t_setid			setid,
				      t_iterator		iterator,
				      void**			data);

int			set_rsv_ll(t_opts			opts,
				   t_size			datasz,
				   t_setid*			setid);

int			set_rel_ll(t_setid			setid);


/*
 * eop
 */

#endif
