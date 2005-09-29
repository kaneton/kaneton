/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * set_ll.c
 * 
 * path          /home/mycure/kaneton/core/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Wed Sep 28 19:44:00 2005   mycure
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this subpart of the set manager is used to build linked-list
 * data structures.
 *
 * note that this data structure is in fact a doubly linked-list.
 *
 * each set of this type can be used in two ways. the first one ask the
 * set manager to allocate and copy each object to add while the second
 * way tells the set manager to simply include the objects in the set.
 *
 * moreover, the linked-list data structure can be used either with the
 * sort option or without.
 *
 * options: SET_OPT_CONTAINER, SET_OPT_SORT, SET_OPT_ALLOC
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

/*
 * ---------- extern ---------------------------------------------------------
 */

extern m_set*		set;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function dumps set objects contained in a set.
 *
 * steps:
 *
 * 1) gets the set descriptor from its identifier.
 * 2) prints each objects' identifier.
 */

t_error			set_dump_ll(t_setid			setid)
{
  t_state		state;
  o_set*		o;
  t_iterator		i;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  cons_msg('#', "dumping %qd node(s) from the linked-list set %qu:\n",
	   o->size,
	   setid);

  set_foreach(SET_OPT_FORWARD, setid, &i, state)
    {
      t_set_ll_node*	n = i.u.ll.node;

      cons_msg('#', "  %qd <0x%x, 0x%x, 0x%x>\n",
	       *((t_id*)n->data), n->prv, n, n->nxt);
    }

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function returns an iterator on the first node of the list.
 *
 * if there is no node in the list, the function returns ERROR_UNKNOWN.
 */

t_error			set_head_ll(t_setid			setid,
				    t_iterator*			iterator)
{
  o_set*		o;

  SET_ENTER(set);

  memset(iterator, 0x0, sizeof(t_iterator));

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  if (o->size == 0)
    SET_LEAVE(set, ERROR_UNKNOWN);

  iterator->u.ll.node = o->u.ll.head;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function returns an iterator on the last node of the list.
 *
 * if there is no node in the list, the function returns ERROR_UNKNOWN.
 */

t_error			set_tail_ll(t_setid			setid,
				    t_iterator*			iterator)
{
  o_set*		o;

  SET_ENTER(set);

  memset(iterator, 0x0, sizeof(t_iterator));

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  if (o->size == 0)
    SET_LEAVE(set, ERROR_UNKNOWN);

  iterator->u.ll.node = o->u.ll.tail;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function returns an iterator on the previous node.
 */

t_error			set_prev_ll(t_setid			setid,
				    t_iterator			current,
				    t_iterator*			previous)
{
  t_set_ll_node*	c = current.u.ll.node;
  o_set*		o;

  SET_ENTER(set);

  memset(previous, 0x0, sizeof(t_iterator));

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  if (c->prv == NULL)
    SET_LEAVE(set, ERROR_UNKNOWN);

  previous->u.ll.node = c->prv;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function returns an iterator on the next node of the list.
 */

t_error			set_next_ll(t_setid			setid,
				    t_iterator			current,
				    t_iterator*			next)
{
  t_set_ll_node*	c = current.u.ll.node;
  o_set*		o;

  SET_ENTER(set);

  memset(next, 0x0, sizeof(t_iterator));

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  if (c->nxt == NULL)
    SET_LEAVE(set, ERROR_UNKNOWN);

  next->u.ll.node = c->nxt;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function inserts a new node at the head of the list.
 *
 * steps:
 *
 * 1) avoids bad identifiers.
 * 2) gets the set descriptor corresponding to the set identifier.
 * 3) if the sort option is enable, this operation is not allowed.
 * 4) allocates and initialises the new node.
 * 5) performs operations from the set options: alloc or not etc..
 * 6) inserts the new node and reorganises the list.
 * 7) increments the number of nodes in the list.
 */

t_error			set_insert_head_ll(t_setid		setid,
					   void*		data)
{
  t_set_ll_node		*n;
  o_set*		o;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (*((t_id*)data) == ID_UNUSED)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (o->u.ll.opts & SET_OPT_SORT)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if ((n = malloc(sizeof(t_set_ll_node))) == NULL)
    SET_LEAVE(set, ERROR_UNKNOWN);

  memset(n, 0x0, sizeof(t_set_ll_node));

  /*
   * 5)
   */

  if (o->u.ll.opts & SET_OPT_ALLOC)
    {
      if ((n->data = malloc(o->u.ll.datasz)) == NULL)
	{
	  free(n);

	  SET_LEAVE(set, ERROR_UNKNOWN);
	}

      memcpy(n->data, data, o->u.ll.datasz);
    }
  else
    {
      n->data = data;
    }

  /*
   * 6)
   */

  n->prv = NULL;
  n->nxt = o->u.ll.head;

  if (n->nxt != NULL)
    n->nxt->prv = n;

  o->u.ll.head = n;

  if (o->u.ll.tail == NULL)
    o->u.ll.tail = n;

  /*
   * 7)
   */

  o->size++;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function inserts a new node at the tail of the list.
 *
 * steps:
 *
 * 1) avoid bad identifiers.
 * 2) gets the set descriptor corresponding to the set identifier.
 * 3) if the sort option is enable, this operation is not allowed.
 * 4) allocates and initialises the new node.
 * 5) performs operations from the set options: alloc or not etc..
 * 6) inserts the new node and reorganises the list.
 * 7) increments the number of nodes in the list.
 */

t_error			set_insert_tail_ll(t_setid		setid,
					   void*		data)
{
  t_set_ll_node		*n;
  o_set*		o;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (*((t_id*)data) == ID_UNUSED)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (o->u.ll.opts & SET_OPT_SORT)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if ((n = malloc(sizeof(t_set_ll_node))) == NULL)
    SET_LEAVE(set, ERROR_UNKNOWN);

  memset(n, 0x0, sizeof(t_set_ll_node));

  /*
   * 5)
   */

  if (o->u.ll.opts & SET_OPT_ALLOC)
    {
      if ((n->data = malloc(o->u.ll.datasz)) == NULL)
	{
	  free(n);

	  SET_LEAVE(set, ERROR_UNKNOWN);
	}

      memcpy(n->data, data, o->u.ll.datasz);
    }
  else
    {
      n->data = data;
    }

  /*
   * 6)
   */

  n->prv = o->u.ll.tail;
  n->nxt = NULL;

  if (n->prv != NULL)
    n->prv->nxt = n;

  o->u.ll.tail = n;

  if (o->u.ll.head == NULL)
    o->u.ll.head = n;

  /*
   * 7)
   */

  o->size++;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function inserts an object before the one referenced by the iterator.
 *
 * steps:
 *
 * 1) checks for a bad identifier.
 * 2) gets the descriptor of the set.
 * 3) does not allow this operation if the sort option is not set.
 * 4) allocates a new node and initialises it.
 * 5) if the alloc option is set, allocates space for the object.
 * 6) inserts the node, so reorganises the linked-list.
 * 7) increments the objects counter.
 */

t_error			set_insert_before_ll(t_setid		setid,
					     t_iterator		iterator,
					     void*		data)
{
  t_set_ll_node		*i = iterator.u.ll.node;
  t_set_ll_node		*n;
  o_set*		o;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (*((t_id*)data) == ID_UNUSED)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (o->u.ll.opts & SET_OPT_SORT)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if ((n = malloc(sizeof(t_set_ll_node))) == NULL)
    SET_LEAVE(set, ERROR_UNKNOWN);

  memset(n, 0x0, sizeof(t_set_ll_node));

  /*
   * 5)
   */

  if (o->u.ll.opts & SET_OPT_ALLOC)
    {
      if ((n->data = malloc(o->u.ll.datasz)) == NULL)
	{
	  free(n);

	  SET_LEAVE(set, ERROR_UNKNOWN);
	}

      memcpy(n->data, data, o->u.ll.datasz);
    }
  else
    {
      n->data = data;
    }

  /*
   * 6)
   */

  n->prv = i->prv;
  n->nxt = i;

  i->prv = n;

  if (n->prv != NULL)
    n->prv->nxt = n;
  else
    o->u.ll.head = n;

  /*
   * 7)
   */

  o->size++;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function inserts a new object after a existing one referenced
 * by the iterator.
 *
 * steps:
 *
 * 1) checks for a bad identifier.
 * 2) gets the descriptor of the set.
 * 3) does not allow this operation if the sort option is not set.
 * 4) allocates a new node and initialises it.
 * 5) if the alloc option is set, allocates space for the object.
 * 6) inserts the node, so reorganises the linked-list.
 * 7) increments the objects counter.
 */

t_error			set_insert_after_ll(t_setid		setid,
					    t_iterator		iterator,
					    void*		data)
{
  t_set_ll_node		*i = iterator.u.ll.node;
  t_set_ll_node		*n;
  o_set*		o;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (*((t_id*)data) == ID_UNUSED)
    SET_LEAVE(set, ERROR_UNKNOWN);

 /*
   * 2)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (o->u.ll.opts & SET_OPT_SORT)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if ((n = malloc(sizeof(t_set_ll_node))) == NULL)
    SET_LEAVE(set, ERROR_UNKNOWN);

  memset(n, 0x0, sizeof(t_set_ll_node));

  /*
   * 5)
   */

  if (o->u.ll.opts & SET_OPT_ALLOC)
    {
      if ((n->data = malloc(o->u.ll.datasz)) == NULL)
	{
	  free(n);

	  SET_LEAVE(set, ERROR_UNKNOWN);
	}

      memcpy(n->data, data, o->u.ll.datasz);
    }
  else
    {
      n->data = data;
    }

  /*
   * 6)
   */

  n->nxt = i->nxt;
  n->prv = i;

  i->nxt = n;

  if (n->nxt != NULL)
    n->nxt->prv = n;
  else
    o->u.ll.tail = n;

  /*
   * 7)
   */

  o->size++;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function adds an object in the linked-list.
 *
 * steps:
 *
 * 1) avoids bad identifiers.
 * 2) gets the set descriptor corresponding to the set identifier.
 * 3) allocates and initialises the new node for the new object.
 * 4) performs operations from the set options: alloc or not etc..
 * 5) inserts the new node in the list.
 *   A) sorts the nodes.
 *     a) inserts the new node in the list.
 *       i) there is an identifier collision so the add operation
 *          is not possible.
 *       ii) inserts the new node in the linked-list.
 *     b) inserts the new node at the end of the list.
 *     c) the list is empty, so the new node becomes the list.
 *   B) does not sort the nodes.
 * 6) increments the number of nodes in the list.
 */

t_error			set_add_ll(t_setid			setid,
				   void*			data)
{
  t_set_ll_node*	n;
  o_set*		o;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (*((t_id*)data) == ID_UNUSED)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if ((n = malloc(sizeof(t_set_ll_node))) == NULL)
    SET_LEAVE(set, ERROR_UNKNOWN);

  memset(n, 0x0, sizeof(t_set_ll_node));

  /*
   * 4)
   */

  if (o->u.ll.opts & SET_OPT_ALLOC)
    {
      if ((n->data = malloc(o->u.ll.datasz)) == NULL)
	{
	  free(n);

	  SET_LEAVE(set, ERROR_UNKNOWN);
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

  if (o->u.ll.opts & SET_OPT_SORT)
    {
      /*
       * A)
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
			   o->setid,
			   *((t_id*)n->data));

		  if (o->u.ll.opts & SET_OPT_ALLOC)
		    free(n->data);

		  free(n);

		  SET_LEAVE(set, ERROR_UNKNOWN);
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
    }
  else
    {
      /*
       * B)
       */

      n->prv = NULL;
      n->nxt = o->u.ll.head;

      if (n->nxt != NULL)
	n->nxt->prv = n;

      o->u.ll.head = n;

      if (o->u.ll.tail == NULL)
	o->u.ll.tail = n;
    }

  /*
   * 6)
   */

  o->size++;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function removes a node from the list.
 *
 * steps:
 *
 * 1) gets the set descriptor given its set identifier.
 * 2) tries to locate the looked for node given its identifier.
 * 3) reorganises the linked-list.
 * 4) if needed, frees the object's memory.
 * 5) frees the node's memory.
 * 6) decrements the number of nodes in the linked-list data structure.
 */

t_error			set_remove_ll(t_setid			setid,
				      t_id			id)
{
  t_set_ll_node*	tmp;
  o_set*		o;
  t_iterator		i;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (set_locate(setid, id, &i) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  tmp = i.u.ll.node;

  /*
   * 3)
   */

  if (tmp->prv != NULL)
    tmp->prv->nxt = tmp->nxt;
  else
    o->u.ll.head = tmp->nxt;

  if (tmp->nxt != NULL)
    tmp->nxt->prv = tmp->prv;
  else
    o->u.ll.tail = tmp->prv;

  /*
   * 4)
   */

  if (o->u.ll.opts & SET_OPT_ALLOC)
    free(tmp->data);

  /*
   * 5)
   */

  free(tmp);

  /*
   * 6)
   */

  o->size--;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function flushes the set, freeing each element.
 *
 * steps
 *
 * 1) gets the descriptor given its identifier.
 * 2) frees each object of the set.
 * 3) resets the linked-list variables.
 * 4) resets the linked-list counter.
 */

t_error			set_flush_ll(t_setid			setid)
{
  t_set_ll_node*	tmp;
  o_set*		o;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  for (tmp = o->u.ll.tail; tmp != NULL; )
    {
      t_set_ll_node*	t = tmp->prv;

      if (o->u.ll.opts & SET_OPT_ALLOC)
	free(tmp->data);

      free(tmp);

      tmp = t;
    }

  /*
   * 3)
   */

  o->u.ll.head = NULL;
  o->u.ll.tail = NULL;

  /*
   * 4)
   */

  o->size = 0;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function tries to find an object with its identifier and build
 * a corresponding iterator.
 *
 * steps:
 *
 * 1) checks if the identifier is a correct one.
 * 2) gets the set object corresponding to the set identifier.
 * 3) tries to find the identifier looked for in the set object's elements.
 */

t_error			set_locate_ll(t_setid			setid,
				      t_id			id,
				      t_iterator*		iterator)
{
  t_set_ll_node*	tmp;
  o_set*		o;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (id == ID_UNUSED)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  for (tmp = o->u.ll.head; tmp != NULL; tmp = tmp->nxt)
    {
      if (*((t_id*)tmp->data) == id)
	{
	  iterator->u.ll.node = tmp;

	  SET_LEAVE(set, ERROR_NONE);
	}
    }

  SET_LEAVE(set, ERROR_UNKNOWN);
}

/*
 * this function returns an object given its iterator.
 */

t_error			set_object_ll(t_setid			setid,
				      t_iterator		iterator,
				      void**			data)
{
  t_set_ll_node*	n = iterator.u.ll.node;

  SET_ENTER(set);

  *data = n->data;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function reserves a set.
 *
 * steps:
 *
 * 1) initialises the set descriptor.
 * 2) avoids bad options.
 * 3) if necessary, reserves an unused identifier for this new set.
 * 4) initialises the set descriptor fields.
 * 5) adds the set descriptor to the set container.
 */

t_error			set_rsv_ll(t_opts			opts,
				   t_size			datasz,
				   t_setid*			setid)
{
  o_set			o;

  SET_ENTER(set);

  /*
   * 1)
   */

  memset(&o, 0x0, sizeof(o_set));

  /*
   * 2)
   */

  if (opts & SET_OPT_ORGANISE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (opts & SET_OPT_CONTAINER)
    {
      *setid = set->setid;
    }
  else
    {
      if (id_rsv(&set->id, setid) != ERROR_NONE)
	SET_LEAVE(set, ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  o.setid = *setid;
  o.size = 0;
  o.type = SET_TYPE_LL;

  o.u.ll.opts = opts;
  o.u.ll.datasz = datasz;
  o.u.ll.head = NULL;
  o.u.ll.tail = NULL;

  /*
   * 5)
   */

  if (set_new(&o) != ERROR_NONE)
    {
      if (!(opts & SET_OPT_CONTAINER))
	id_rel(&set->id, o.setid);

      SET_LEAVE(set, ERROR_UNKNOWN);
    }

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function releases a set.
 *
 * steps:
 *
 * 1) gets the set given its set identifier.
 * 2) flushs the set to release every objects contained in it.
 * 3) cannot release the set container.
 * 4) if needed, releases the set identifier.
 * 5) then, removes the set from the set container.
 */

t_error			set_rel_ll(t_setid			setid)
{
  o_set			*o;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (set_flush(setid) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (setid == set->setid)
    {
      cons_msg('!', "set: cannot release the set container\n");

      SET_LEAVE(set, ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  if (!(o->u.ll.opts & SET_OPT_CONTAINER))
    if (id_rel(&set->id, o->setid) != ERROR_NONE)
      SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 5)
   */

  if (set_delete(o->setid) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  SET_LEAVE(set, ERROR_NONE);
}
