/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/set/set-ll.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       julien quintard   [sun jan 30 20:34:59 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this set implementation provides a linked-list data structure. note that
 * this data structure is in fact a doubly linked-list.
 *
 * as for the 'array' implementation, this type of set can be used in
 * two ways. either objects are cloned and stored, in which case the
 * ALLOCATE option is required, or pointers to objects are stored directly,
 * implying that the objects have been allocated beforehand.
 *
 * note that the 'datasz' argument of the set_reserve() function is
 * meaningful only if the ALLOCATE option is set.
 *
 * options:
 *   SET_OPTION_CONTAINER
 *   SET_OPTION_SORT
 *   SET_OPTION_ALLOCATE
 *   SET_OPTION_FREE
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the set manager.
 */

extern m_set		_set;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function returns true if the given identifier is registered in
 * the set.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) walk through the objects and try to locate the object, otherwise
 *    return false.
 */

t_bool			set_exist_ll(i_set			setid,
				     t_id			id)
{
  s_set_ll_node*	tmp;
  o_set*		o;

  /*
   * 0)
   */

  assert(id != ID_UNUSED);

  /*
   * 1)
   */

  assert(set_descriptor(setid, &o) == STATUS_OK);

  /*
   * 2)
   */

  for (tmp = o->u.ll.head; tmp != NULL; tmp = tmp->next)
    {
      if (*((t_id*)tmp->data) == id)
	CORE_TRUE();
    }

  CORE_FALSE();
}

/*
 * this function shows a set's attributes.
 *
 * steps:
 *
 * 1) retrieve the set descriptor.
 * 2) build the options string.
 * 3) display general information.
 * 4) go through the objects and display them.
 */

t_status		set_show_ll(i_set			setid,
				    mt_margin			margin)
{
  char			options[5];
  t_state		state;
  o_set*		o;
  s_iterator		i;

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  if (o->options & SET_OPTION_CONTAINER)
    options[0] = 'c';
  else
    options[0] = '.';

  if (o->options & SET_OPTION_SORT)
    options[1] = 's';
  else
    options[1] = '.';

  if (o->options & SET_OPTION_ALLOCATE)
    options[2] = 'a';
  else
    options[2] = '.';

  if (o->options & SET_OPTION_FREE)
    options[3] = 'f';
  else
    options[3] = '.';

  options[4] = '\0';

  /*
   * 3)
   */

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "set: id(%qd) type(ll) size(%qd) datasz(%u) options(%s) "
	      "head(0x%x) tail(0x%x)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      o->id,
	      o->size,
	      o->datasz,
	      options,
	      o->u.ll.head,
	      o->u.ll.tail);

  /*
   * 4)
   */

  set_foreach(SET_OPTION_FORWARD, setid, &i, state)
    {
      s_set_ll_node*	n = i.u.ll.node;

      module_call(console, message,
		  '#',
		  MODULE_CONSOLE_MARGIN_FORMAT
		  "  object: id(%qd) previous(0x%x) "
		  "current(0x%x) next(0x%x)\n",
		  MODULE_CONSOLE_MARGIN_VALUE(margin),
		  *((t_id*)n->data),
		  n->previous,
		  n,
		  n->next);
    }

  CORE_LEAVE();
}

/*
 * this function returns an iterator on the first node of the list.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) if the set is empty, return false.
 * 3) set the iterator.
 */

t_bool			set_head_ll(i_set			setid,
				    s_iterator*			iterator)
{
  o_set*		o;

  /*
   * 0)
   */

  assert(iterator != NULL);

  /*
   * 1)
   */

  assert(set_descriptor(setid, &o) == STATUS_OK);

  /*
   * 2)
   */

  if (o->size == 0)
    CORE_FALSE();

  /*
   * 3)
   */

  iterator->u.ll.node = o->u.ll.head;

  CORE_TRUE();
}

/*
 * this function returns an iterator on the last node of the list.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) if the set is empty, return false.
 * 3) set the iterator.
 */

t_bool			set_tail_ll(i_set			setid,
				    s_iterator*			iterator)
{
  o_set*		o;

  /*
   * 0)
   */

  assert(iterator != NULL);

  /*
   * 1)
   */

  assert(set_descriptor(setid, &o) == STATUS_OK);

  /*
   * 2)
   */

  if (o->size == 0)
    CORE_FALSE();

  /*
   * 3)
   */

  iterator->u.ll.node = o->u.ll.tail;

  CORE_TRUE();
}

/*
 * this function returns an iterator on the previous node.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) if there is no previous object, return false.
 * 3) set the iterator.
 */

t_bool			set_previous_ll(i_set			setid,
					s_iterator		current,
					s_iterator*		previous)
{
  s_set_ll_node*	c = current.u.ll.node;
  o_set*		o;

  /*
   * 0)
   */

  assert(previous != NULL);

  /*
   * 1)
   */

  assert(set_descriptor(setid, &o) == STATUS_OK);

  /*
   * 2)
   */

  if (c->previous == NULL)
    CORE_FALSE();

  /*
   * 3)
   */

  previous->u.ll.node = c->previous;

  CORE_TRUE();
}

/*
 * this function returns an iterator on the next node of the list.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) if there is no previous object, return false.
 * 3) set the iterator.
 */

t_bool			set_next_ll(i_set			setid,
				    s_iterator			current,
				    s_iterator*			next)
{
  s_set_ll_node*	c = current.u.ll.node;
  o_set*		o;

  /*
   * 0)
   */

  assert(next != NULL);

  /*
   * 1)
   */

  assert(set_descriptor(setid, &o) == STATUS_OK);

  /*
   * 2)
   */

  if (c->next == NULL)
    CORE_FALSE();

  /*
   * 3)
   */

  next->u.ll.node = c->next;

  CORE_TRUE();
}

/*
 * this function inserts a new node at the head of the list.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) check that this operation is allowed for this set.
 * 3) allocate and initialize the new node.
 * 4) clone the object if required.
 * 5) insert the new node and arrange the neighbour nodes.
 * 6) update the set's size.
 */

t_status		set_insert_ll(i_set			setid,
				      void*			data)
{
  s_set_ll_node*	n;
  o_set*		o;

  /*
   * 0)
   */

  if (data == NULL)
    CORE_ESCAPE("the 'data' argument is null");

  if (*((t_id*)data) == ID_UNUSED)
    CORE_ESCAPE("the provided object must begin with a valid identifier");

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  if (o->options & SET_OPTION_SORT)
    CORE_ESCAPE("unable to insert an object in a sorted set");

  /*
   * 3)
   */

  if ((n = malloc(sizeof (s_set_ll_node))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the linked-list node");

  memset(n, 0x0, sizeof (s_set_ll_node));

  /*
   * 4)
   */

  if (o->options & SET_OPTION_ALLOCATE)
    {
      if ((n->data = malloc(o->datasz)) == NULL)
	{
	  free(n);

	  CORE_ESCAPE("unable to allocate memory for the object's copy");
	}

      memcpy(n->data, data, o->datasz);
    }
  else
    {
      n->data = data;
    }

  /*
   * 5)
   */

  n->previous = NULL;
  n->next = o->u.ll.head;

  if (n->next != NULL)
    n->next->previous = n;

  o->u.ll.head = n;

  if (o->u.ll.tail == NULL)
    o->u.ll.tail = n;

  /*
   * 6)
   */

  o->size++;

  CORE_LEAVE();
}

/*
 * this function inserts a new node at the end of the list.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) check that this operation is allowed for this set.
 * 3) allocate and initialize the new node.
 * 4) clone the object if required.
 * 5) insert the new node and arrange the neighbour nodes.
 * 6) update the set's size.
 */

t_status		set_append_ll(i_set			setid,
				      void*			data)
{
  s_set_ll_node*	n;
  o_set*		o;

  /*
   * 0)
   */

  if (data == NULL)
    CORE_ESCAPE("the 'data' argument is null");

  if (*((t_id*)data) == ID_UNUSED)
    CORE_ESCAPE("the provided object must begin with a valid identifier");

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  if (o->options & SET_OPTION_SORT)
    CORE_ESCAPE("unable to append an object to a sorted set");

  /*
   * 3)
   */

  if ((n = malloc(sizeof (s_set_ll_node))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the linked-list node");

  memset(n, 0x0, sizeof (s_set_ll_node));

  /*
   * 4)
   */

  if (o->options & SET_OPTION_ALLOCATE)
    {
      if ((n->data = malloc(o->datasz)) == NULL)
	{
	  free(n);

	  CORE_ESCAPE("unable to allocate memory for the object's copy");
	}

      memcpy(n->data, data, o->datasz);
    }
  else
    {
      n->data = data;
    }

  /*
   * 5)
   */

  n->previous = o->u.ll.tail;
  n->next = NULL;

  if (n->previous != NULL)
    n->previous->next = n;

  o->u.ll.tail = n;

  if (o->u.ll.head == NULL)
    o->u.ll.head = n;

  /*
   * 6)
   */

  o->size++;

  CORE_LEAVE();
}

/*
 * this function inserts an object before the one referenced by the iterator.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) check that this operation is allowed for this set.
 * 3) allocate and initialize the new node.
 * 4) clone the object if required.
 * 5) insert the new node and arrange the neighbour nodes.
 * 6) update the set's size.
 */

t_status		set_before_ll(i_set			setid,
				      s_iterator		iterator,
				      void*			data)
{
  s_set_ll_node*	i = iterator.u.ll.node;
  s_set_ll_node*	n;
  o_set*		o;

  /*
   * 0)
   */

  if (data == NULL)
    CORE_ESCAPE("the 'data' argument is null");

  if (*((t_id*)data) == ID_UNUSED)
    CORE_ESCAPE("the provided object must begin with a valid identifier");

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  if (o->options & SET_OPTION_SORT)
    CORE_ESCAPE("unable to insert at a precise location in a sorted set");

  /*
   * 3)
   */

  if ((n = malloc(sizeof (s_set_ll_node))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the linked-list node");

  memset(n, 0x0, sizeof (s_set_ll_node));

  /*
   * 4)
   */

  if (o->options & SET_OPTION_ALLOCATE)
    {
      if ((n->data = malloc(o->datasz)) == NULL)
	{
	  free(n);

	  CORE_ESCAPE("unable to allocate memory for the object's copy");
	}

      memcpy(n->data, data, o->datasz);
    }
  else
    {
      n->data = data;
    }

  /*
   * 5)
   */

  n->previous = i->previous;
  n->next = i;

  i->previous = n;

  if (n->previous != NULL)
    n->previous->next = n;
  else
    o->u.ll.head = n;

  /*
   * 6)
   */

  o->size++;

  CORE_LEAVE();
}

/*
 * this function inserts a new object after a existing one referenced
 * by the iterator.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) check that this operation is allowed for this set.
 * 3) allocate and initialize the new node.
 * 4) clone the object if required.
 * 5) insert the new node and arrange the neighbour nodes.
 * 6) update the set's size.
 */

t_status		set_after_ll(i_set			setid,
				     s_iterator			iterator,
				     void*			data)
{
  s_set_ll_node*	i = iterator.u.ll.node;
  s_set_ll_node*	n;
  o_set*		o;

  /*
   * 0)
   */

  if (data == NULL)
    CORE_ESCAPE("the 'data' argument is null");

  if (*((t_id*)data) == ID_UNUSED)
    CORE_ESCAPE("the provided object must begin with a valid identifier");

 /*
   * 1)
   */

  if (set_descriptor(setid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  if (o->options & SET_OPTION_SORT)
    CORE_ESCAPE("unable to insert at a precise location in a sorted set");

  /*
   * 3)
   */

  if ((n = malloc(sizeof (s_set_ll_node))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the linked-list node");

  memset(n, 0x0, sizeof (s_set_ll_node));

  /*
   * 4)
   */

  if (o->options & SET_OPTION_ALLOCATE)
    {
      if ((n->data = malloc(o->datasz)) == NULL)
	{
	  free(n);

	  CORE_ESCAPE("unable to allocate memory for the object's copy");
	}

      memcpy(n->data, data, o->datasz);
    }
  else
    {
      n->data = data;
    }

  /*
   * 5)
   */

  n->next = i->next;
  n->previous = i;

  i->next = n;

  if (n->next != NULL)
    n->next->previous = n;
  else
    o->u.ll.tail = n;

  /*
   * 6)
   */

  o->size++;

  CORE_LEAVE();
}

/*
 * this function adds an object in the linked-list.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) allocate and initialize the new node.
 * 3) clone the object if required.
 * 4) insert the new node and arrange the neighbour nodes depending on
 *    the options.
 *   A) if the SORT option has been activated...
 *     a) insert the new node in the list.
 *       i) there is an identifier collision; the operation is cancelled.
 *       ii) try to insert the new node in the list i.e before a node
 *           with a higher identifier.
 *     b) if no such node has been found, insert the node at the end
 *        of the list.
 *     c) if the list is empty: the new node becomes the list.
 *   B) otherwise...
 *     a) insert the node at the head of the list.
 * 5) update the set's size.
 */

t_status		set_add_ll(i_set			setid,
				   void*			data)
{
  s_set_ll_node*	n;
  o_set*		o;

  /*
   * 0)
   */

  if (data == NULL)
    CORE_ESCAPE("the 'data' argument is null");

  if (*((t_id*)data) == ID_UNUSED)
    CORE_ESCAPE("the provided object must begin with a valid identifier");

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  if ((n = malloc(sizeof (s_set_ll_node))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the linked-list node");

  memset(n, 0x0, sizeof (s_set_ll_node));

  /*
   * 3)
   */

  if (o->options & SET_OPTION_ALLOCATE)
    {
      if ((n->data = malloc(o->datasz)) == NULL)
	{
	  free(n);

	  CORE_ESCAPE("unable to allocate memory for the object's copy");
	}

      memcpy(n->data, data, o->datasz);
    }
  else
    {
      n->data = data;
    }

  /*
   * 4)
   */

  if (o->options & SET_OPTION_SORT)
    {
      /*
       * A)
       */

      if (o->u.ll.head != NULL)
	{
	  s_set_ll_node*	tmp;

	  /*
	   * a)
	   */

	  for (tmp = o->u.ll.head; tmp != NULL; tmp = tmp->next)
	    {
	      if (*((t_id*)n->data) == *((t_id*)tmp->data))
		{
		  /*
		   * i)
		   */

		  if ((o->options & SET_OPTION_ALLOCATE) ||
		      (o->options & SET_OPTION_FREE))
		    free(n->data);

		  free(n);

		  CORE_ESCAPE("identifier collision detected in the set "
			      "%qd on the object identifier %qd",
			      o->id,
			      *((t_id*)n->data));
		}

	      if (*((t_id*)n->data) < *((t_id*)tmp->data))
		{
		  /*
		   * ii)
		   */

		  n->previous = tmp->previous;
		  n->next = tmp;

		  if (n->previous != NULL)
		    n->previous->next = n;
		  else
		    o->u.ll.head = n;

		  if (n->next != NULL)
		    n->next->previous = n;
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
	      n->previous = o->u.ll.tail;
	      n->next = NULL;

	      if (n->previous != NULL)
		n->previous->next = n;
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

	  n->previous = NULL;
	  n->next = NULL;

	  o->u.ll.head = n;
	  o->u.ll.tail = n;
	}
    }
  else
    {
      /*
       * B)
       */

      /*
       * a)
       */

      n->previous = NULL;
      n->next = o->u.ll.head;

      if (n->next != NULL)
	n->next->previous = n;

      o->u.ll.head = n;

      if (o->u.ll.tail == NULL)
	o->u.ll.tail = n;
    }

  /*
   * 5)
   */

  o->size++;

  CORE_LEAVE();
}

/*
 * this function removes a node from the list.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) locate the object to remove.
 * 3) save a pointer on the node.
 * 4) arrange the list so that the node no longer gets referenced.
 * 5) release the object's memory, if required.
 * 6) release the node's memory.
 * 7) update the set's size.
 */

t_status		set_remove_ll(i_set			setid,
				      t_id			id)
{
  s_set_ll_node*	tmp;
  o_set*		o;
  s_iterator		i;

  /*
   * 0)
   */

  if (id == ID_UNUSED)
    CORE_ESCAPE("invalid identifier");

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  if (set_locate(setid, id, &i) != STATUS_OK)
    CORE_ESCAPE("unable to locate the object in the set");

  /*
   * 3)
   */

  tmp = i.u.ll.node;

  /*
   * 4)
   */

  if (tmp->previous != NULL)
    tmp->previous->next = tmp->next;
  else
    o->u.ll.head = tmp->next;

  if (tmp->next != NULL)
    tmp->next->previous = tmp->previous;
  else
    o->u.ll.tail = tmp->previous;

  /*
   * 5)
   */

  if ((o->options & SET_OPTION_ALLOCATE) ||
      (o->options & SET_OPTION_FREE))
    free(tmp->data);

  /*
   * 6)
   */

  free(tmp);

  /*
   * 7)
   */

  o->size--;

  CORE_LEAVE();
}

/*
 * this function deletes an element whose location is given by the iterator.
 *
 * steps:
 *
 * 1) retrieve the set descriptor.
 * 2) save the node's address.
 * 3) if the iterator is invalid, return an error.
 * 4) arrange the list so that the node no longer gets referenced.
 * 5) release the object's memory, if required.
 * 6) release the node's memory.
 * 7) update the set's size.
 */

t_status		set_delete_ll(i_set			setid,
				      s_iterator		iterator)
{
  s_set_ll_node*	n;
  o_set*		o;

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  n = iterator.u.ll.node;

  /*
   * 3)
   */

  if (n == NULL)
    CORE_ESCAPE("the iterator's internal pointer is invalid");

  /*
   * 4)
   */

  if (n->previous)
    n->previous->next = n->next;
  else
    o->u.ll.head = n->next;

  if (n->next)
    n->next->previous = n->previous;
  else
    o->u.ll.tail = n->previous;

  /*
   * 5)
   */

  if (o->options & SET_OPTION_FREE ||
      o->options & SET_OPTION_ALLOCATE)
    free(n->data);

  /*
   * 6)
   */

  free(n);

  /*
   * 7)
   */

  o->size--;

  CORE_LEAVE();
}

/*
 * this function flushes the set.
 *
 * steps
 *
 * 1) retrieve the set descriptor.
 * 2) go through the objects.
 *   a) if required, release the object's memory.
 *   b) release the node's memory.
 * 3) re-initialize the head and tail pointers.
 * 4) re-set the size to zero.
 */

t_status		set_flush_ll(i_set			setid)
{
  s_set_ll_node*	tmp;
  o_set*		o;

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  for (tmp = o->u.ll.tail; tmp != NULL; )
    {
      s_set_ll_node*	t = tmp->previous;

      if ((o->options & SET_OPTION_ALLOCATE) ||
	  (o->options & SET_OPTION_FREE))
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

  CORE_LEAVE();
}

/*
 * this function tries to locate an object according to its identifier
 * and build a corresponding iterator.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) go through the objects and return an iterator if found.
 * 3) otherwise, return an error.
 */

t_status		set_locate_ll(i_set			setid,
				      t_id			id,
				      s_iterator*		iterator)
{
  s_set_ll_node*	tmp;
  o_set*		o;

  /*
   * 0)
   */

  if (iterator == NULL)
    CORE_ESCAPE("the 'iterator' argument is null");

  if (id == ID_UNUSED)
    CORE_ESCAPE("invalid object identifier");

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  for (tmp = o->u.ll.head; tmp != NULL; tmp = tmp->next)
    {
      if (*((t_id*)tmp->data) == id)
	{
	  iterator->u.ll.node = tmp;

	  CORE_LEAVE();
	}
    }

  /*
   * 3)
   */

  CORE_ESCAPE("unable to find the given object identifier in the set");
}

/*
 * this function returns an object given its iterator.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) return the object.
 */

t_status		set_object_ll(i_set			setid,
				      s_iterator		iterator,
				      void**			data)
{
  s_set_ll_node*	n = iterator.u.ll.node;
  o_set*		o;

  /*
   * 0)
   */

  if (data == NULL)
    CORE_ESCAPE("the 'data' argument is null");

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  *data = n->data;

  CORE_LEAVE();
}

/*
 * this function reserves a set according to several options.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) assign an identifier, depending on the options: either this is
 *    the set container or not. if it is, take the set container identifier
 *    that is in the set manager.
 * 2) initialize and fill the set descriptor.
 * 3) register the set descriptor.
 */

t_status		set_reserve_ll(t_options		options,
				       t_size			datasz,
				       i_set*			id)
{
  o_set			o;

  /*
   * 0)
   */

  if (datasz < sizeof (t_id))
    CORE_ESCAPE("unable to reserve a set for objects smaller than "
		"an identifier");

  if (id == NULL)
    CORE_ESCAPE("the 'id' argument is null");

  if (options & SET_OPTION_ORGANISE)
    CORE_ESCAPE("unable to set the organise option since linked-list sets "
		"are implicitly organised");

  if ((options & SET_OPTION_ALLOCATE) && (options & SET_OPTION_FREE))
    CORE_ESCAPE("unable to reserve a set with both alloc and free options");

  /*
   * 1)
   */

  if (options & SET_OPTION_CONTAINER)
    {
      *id = _set.sets;
    }
  else
    {
      if (id_reserve(&_set.id, id) != STATUS_OK)
	CORE_ESCAPE("unable to reserve the set identifier");
    }

  /*
   * 2)
   */

  memset(&o, 0x0, sizeof (o_set));

  o.id = *id;
  o.size = 0;
  o.type = SET_TYPE_LL;
  o.options = options;
  o.datasz = datasz;

  o.u.ll.head = NULL;
  o.u.ll.tail = NULL;

  /*
   * 3)
   */

  if (set_new(&o) != STATUS_OK)
    {
      if (!(options & SET_OPTION_CONTAINER))
	id_release(&_set.id, o.id);

      CORE_ESCAPE("unable to register the set descriptor");
    }

  CORE_LEAVE();
}

/*
 * this function releases a set.
 *
 * steps:
 *
 * 1) retrieve the set descriptor.
 * 2) flush the set.
 * 3) release the set identifier.
 * 4) remove the set descriptor from the set container.
 */

t_status		set_release_ll(i_set			setid)
{
  o_set			*o;

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  if (set_flush(setid) != STATUS_OK)
    CORE_ESCAPE("unable to flush the set");

  /*
   * 3)
   */

  if (id_release(&_set.id, o->id) != STATUS_OK)
    CORE_ESCAPE("unable to release the set identifier");

  /*
   * 4)
   */

  if (set_destroy(o->id) != STATUS_OK)
    CORE_ESCAPE("unable to destroy the set descriptor");

  CORE_LEAVE();
}

/*
 * this function just returns an error because the ll set does not
 * support this operation.
 */

t_status		set_push_ll(i_set			setid,
				    void*			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error because the ll set does not
 * support this operation.
 */

t_status		set_pop_ll(i_set			setid)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error because the ll set does not
 * support this operation.
 */

t_status		set_pick_ll(i_set			setid,
				    void**			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}
