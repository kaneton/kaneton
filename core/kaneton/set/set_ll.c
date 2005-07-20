/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * set_ll.c
 * 
 * path          /home/mycure/kaneton/core/kaneton/set
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Wed Jul 20 21:26:44 2005   mycure
 */

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
 *
 * XXX ajouter sort
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function returns an iterator on the first node of the list.
 *
 * if there is no node in the list, the function returns -1.
 */

int			set_head_ll(t_setid			setid,
				    t_iterator*			iterator)
{
  o_set*		o;

  set_check(set);

  memset(iterator, 0x0, sizeof(t_iterator));

  if (set_descriptor(setid, &o) != 0)
    return (-1);

  if (o->size == 0)
    return (-1);

  *iterator = o->u.ll.head;

  return (0);
}

/*
 * this function returns an iterator on the last node of the list.
 *
 * if there is no node in the list, the function returns -1.
 */

int			set_tail_ll(t_setid			setid,
				    t_iterator*			iterator)
{
  o_set*		o;

  set_check(set);

  memset(iterator, 0x0, sizeof(t_iterator));

  if (set_descriptor(setid, &o) != 0)
    return (-1);

  if (o->size == 0)
    return (-1);

  *iterator = o->u.ll.tail;

  return (0);
}

/*
 * this function returns an iterator on the previous node.
 */

int			set_prev_ll(t_setid			setid,
				    t_iterator			current,
				    t_iterator*			previous)
{
  t_set_ll_node*	c = current;
  o_set*		o;

  set_check(set);

  memset(previous, 0x0, sizeof(t_iterator));

  if (set_descriptor(setid, &o) != 0)
    return (-1);

  if (c->prv == NULL)
    return (-1);

  *previous = c->prv;

  return (0);
}

/*
 * this function returns an iterator on the next node of the list.
 */

int			set_next_ll(t_setid			setid,
				    t_iterator			current,
				    t_iterator*			next)
{
  t_set_ll_node*	c = current;
  o_set*		o;

  set_check(set);

  memset(next, 0x0, sizeof(t_iterator));

  if (set_descriptor(setid, &o) != 0)
    return (-1);

  if (c->nxt == NULL)
    return (-1);

  *next = c->nxt;

  return (0);
}

/*
 * this function adds an object in the linked-list.
 *
 * steps:
 *
 * 1) checks whether the set manager was previously initialized.
 * 2) gets the set descriptor corresponding to the set identifier.
 * 3) allocates and initializes the new node for the new object.
 * 4) performs operations from the set options: alloc or not etc..
 * 5) inserts the new node in the list
 *   a) inserts the new node in the list.
 *     i) there is an identifier collision so the add operation
 *        is not possible.
 *     ii) inserts the new node in the linked-list.
 *   b) inserts the new node at the end of the list.
 *   c) the list is empty, so the new node becomes the list.
 * 6) increments the number of nodes in the list.
 */

int			set_add_ll(t_setid			setid,
				   void*			data)
{
  t_set_ll_node*	n;
  o_set*		o;

  /*
   * 1)
   */

  set_check(set);

  /*
   * 2)
   */

  if (set_descriptor(setid, &o) != 0)
    return (-1);

  /*
   * 3)
   */

  if ((n = malloc(sizeof(t_set_ll_node))) == NULL)
    return (-1);

  memset(n, 0x0, sizeof(t_set_ll_node));

  /*
   * 4)
   */

  if (o->u.ll.opts & SET_OPT_ALLOC)
    {
      if ((n->data = malloc(o->u.ll.datasz)) == NULL)
	{
	  free(n);

	  return (-1);
	}

      memcpy(n->data, data, o->u.ll.datasz);
    }
  else
    {
      n->data = data;
    }

  /*
   * 5)
   */

  if (o->u.ll.head != NULL)
    {
      t_set_ll_node	*tmp;

      /*
       * a)
       */

      for (tmp = o->u.ll.head; tmp != NULL; tmp = tmp->nxt)
	{
	  if (*((t_id*)n->data) == *((t_id*)tmp->data))
	    {
	      /*
	       * i)
	       */

	      cons_msg('!', "set: identifier collision detected "
		       "in the set %qu on the object identifier %qu\n",
		       o->id,
		       *((t_id*)n->data));

	      if (o->u.ll.opts & SET_OPT_ALLOC)
		free(n->data);

	      free(n);

	      return (-1);
	    }

	  if (*((t_id*)n->data) < *((t_id*)tmp->data))
	    {
	      /*
	       * ii)
	       */

	      n->prv = tmp->prv;
	      n->nxt = tmp;

	      if (n->prv != NULL)
		n->prv->nxt = n;
	      else
		o->u.ll.head = n;

	      if (n->nxt != NULL)
		n->nxt->prv = n;
	      else
		o->u.ll.tail = n;

	      break;
	    }
	}

      /*
       * b
       */

      if (tmp == NULL)
	{
	  n->prv = o->u.ll.tail;
	  n->nxt = NULL;

	  if (n->prv != NULL)
	    n->prv->nxt = n;
	  else
	    o->u.ll.head = n;

	  o->u.ll.tail = n;
	}
    }
  else
    {
      /*
       * c)
       */

      n->prv = NULL;
      n->nxt = NULL;

      o->u.ll.head = n;
      o->u.ll.tail = n;
    }

  /*
   * 6)
   */

  o->size++;

  return (0);
}

/*
 * this function removes a node from the list.
 *
 * steps:
 *
 * 1) XXX
 */

int			set_remove_ll(t_setid			setid,
				      t_id			id)
{
  /* XXX */

  return (0);
}

/*
 * this function returns an object given its iterator.
 */

int			set_get_ll(t_setid			setid,
				   t_iterator			iterator,
				   void**			data)
{
  t_set_ll_node*	n = iterator;

  set_check(set);

  *data = n->data;

  return (0);
}

/*
 * this function reserves a set.
 *
 * steps:
 *
 * 1) checks whether the set manager was previously initialized.
 * 2) initializes the set descriptor.
 * 3) if necessary, reserves an unused identifier for this new set.
 * 4) initializes the set descriptor fields.
 * 5) adds the set descriptor to the set container.
 */

int			set_rsv_ll(t_opts			opts,
				   t_size			datasz,
				   t_setid*			setid)
{
  o_set			o;

  /*
   * 1)
   */

  set_check(set);

  /*
   * 2)
   */

  memset(&o, 0x0, sizeof(o_set));

  /*
   * 3)
   */

  if (opts & SET_OPT_CONTAINER)
    {
      *setid = set->contid;
    }
  else
    {
      if (id_rsv(&set->id, setid) != 0)
	return (-1);
    }

  /*
   * 4)
   */

  o.id = *setid;
  o.size = 0;
  o.type = SET_TYPE_LL;

  o.u.ll.opts = opts;
  o.u.ll.datasz = datasz;
  o.u.ll.head = NULL;
  o.u.ll.tail = NULL;

  /*
   * 5)
   */

  if (set_new(&o) != 0)
    {
      id_rel(&set->id, *setid);

      return (-1);
    }

  return (0);
}

/*
 * this function releases a set.
 *
 * steps:
 *
 * 1) XXX
 */

int			set_rel_ll(t_setid			setid)
{
  /* XXX */

  return (0);
}
