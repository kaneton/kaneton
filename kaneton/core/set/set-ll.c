/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/set/set-ll.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [tue feb  6 22:58:58 2007]
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
 * moreover the option free can be used to tell the set manager to call
 * the free() function each time an object is released. this option
 * means that objects passed to the set manager was previously allocated
 * with the malloc() functions suite.
 *
 * moreover, the linked-list data structure can be used either with the
 * sort option or without.
 *
 * the datasz argument of the set_reserve() function is meaningful only in the
 * case the allocate or free options are set.
 *
 * options: SET_OPTION_CONTAINER, SET_OPTION_SORT, SET_OPTION_ALLOC,
 *   SET_OPTION_FREE
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_set*		_set;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function tells if the set object is a linked-list set.
 */

t_error			set_type_ll(i_set			setid)
{
  o_set*		o;

  if (set_descriptor(setid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  if (o->type != SET_TYPE_LL)
    CORE_ESCAPE("invalid set type");

  CORE_LEAVE();
}

/*
 * this function tries to find an object with its identifier and returns
 * true or false.
 *
 * steps:
 *
 * 1) checks if the identifier is a correct one.
 * 2) gets the set object corresponding to the set identifier.
 * 3) tries to find the identifier looked for in the set object's elements.
 */

t_error			set_exist_ll(i_set			setid,
				     t_id			id)
{
  t_set_ll_node*	tmp;
  o_set*		o;

  /*
   * 1)
   */

  assert(id != ID_UNUSED);

  /*
   * 2)
   */

  assert(set_descriptor(setid, &o) == ERROR_OK);

  /*
   * 3)
   */

  for (tmp = o->u.ll.head; tmp != NULL; tmp = tmp->next)
    {
      if (*((t_id*)tmp->data) == id)
	CORE_TRUE();
    }

  CORE_FALSE();
}

/*
 * this function shows set objects contained in a set.
 *
 * steps:
 *
 * 1) gets the set descriptor from its identifier.
 * 2) prints each objects' identifier.
 */

t_error			set_show_ll(i_set			setid)
{
  t_state		state;
  o_set*		o;
  t_iterator		i;

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  module_call(console, message,
	      '#', "  %qd node(s) from the linked-list set %qd\n",
	      o->size,
	      setid);

  set_foreach(SET_OPTION_FORWARD, setid, &i, state)
    {
      t_set_ll_node*	n = i.u.ll.node;

      module_call(console, message,
		  '#', "    %qd <0x%x, 0x%x, 0x%x>\n",
		  *((t_id*)n->data), n->previous, n, n->next);
    }

  CORE_LEAVE();
}

/*
 * this function returns an iterator on the first node of the list.
 *
 * if there is no node in the list, the function returns ERROR_FALSE.
 */

t_error			set_head_ll(i_set			setid,
				    t_iterator*			iterator)
{
  o_set*		o;

  assert(iterator != NULL);

  memset(iterator, 0x0, sizeof(t_iterator));

  assert(set_descriptor(setid, &o) == ERROR_OK);

  if (o->size == 0)
    CORE_FALSE();

  iterator->u.ll.node = o->u.ll.head;

  CORE_TRUE();
}

/*
 * this function returns an iterator on the last node of the list.
 *
 * if there is no node in the list, the function returns ERROR_FALSE.
 */

t_error			set_tail_ll(i_set			setid,
				    t_iterator*			iterator)
{
  o_set*		o;

  assert(iterator != NULL);

  memset(iterator, 0x0, sizeof(t_iterator));

  assert(set_descriptor(setid, &o) == ERROR_OK);

  if (o->size == 0)
    CORE_FALSE();

  iterator->u.ll.node = o->u.ll.tail;

  CORE_TRUE();
}

/*
 * this function returns an iterator on the previous node.
 */

t_error			set_previous_ll(i_set			setid,
					t_iterator		current,
					t_iterator*		previous)
{
  t_set_ll_node*	c = current.u.ll.node;
  o_set*		o;

  assert(previous != NULL);

  memset(previous, 0x0, sizeof(t_iterator));

  assert(set_descriptor(setid, &o) == ERROR_OK);

  if (c->previous == NULL)
    CORE_FALSE();

  previous->u.ll.node = c->previous;

  CORE_TRUE();
}

/*
 * this function returns an iterator on the next node of the list.
 */

t_error			set_next_ll(i_set			setid,
				    t_iterator			current,
				    t_iterator*			next)
{
  t_set_ll_node*	c = current.u.ll.node;
  o_set*		o;

  assert(next != NULL);

  memset(next, 0x0, sizeof(t_iterator));

  assert(set_descriptor(setid, &o) == ERROR_OK);

  if (c->next == NULL)
    CORE_FALSE();

  next->u.ll.node = c->next;

  CORE_TRUE();
}

/*
 * this function inserts a new node at the head of the list.
 *
 * steps:
 *
 * 1) avoids bad identifiers.
 * 2) gets the set descriptor corresponding to the set identifier.
 * 3) if the sort option is enable, this operation is not allowed.
 * 4) allocates and initializes the new node.
 * 5) performs operations from the set options: alloc or not etc.
 * 6) inserts the new node and reorganises the list.
 * 7) increments the number of nodes in the list.
 */

t_error			set_insert_ll(i_set			setid,
				      void*			data)
{
  t_set_ll_node		*n;
  o_set*		o;

  assert(data != NULL);

  /*
   * 1)
   */

  if (*((t_id*)data) == ID_UNUSED)
    CORE_ESCAPE("the provided object must begin with a valid identifier");

  /*
   * 2)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 3)
   */

  if (o->options & SET_OPTION_SORT)
    CORE_ESCAPE("unable to insert an object in a sorted set");

  /*
   * 4)
   */

  if ((n = malloc(sizeof(t_set_ll_node))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the linked-list node");

  memset(n, 0x0, sizeof(t_set_ll_node));

  /*
   * 5)
   */

  if (o->options & SET_OPTION_ALLOC)
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
   * 6)
   */

  n->previous = NULL;
  n->next = o->u.ll.head;

  if (n->next != NULL)
    n->next->previous = n;

  o->u.ll.head = n;

  if (o->u.ll.tail == NULL)
    o->u.ll.tail = n;

  /*
   * 7)
   */

  o->size++;

  CORE_LEAVE();
}

/*
 * this function inserts a new node at the tail of the list.
 *
 * steps:
 *
 * 1) avoid bad identifiers.
 * 2) gets the set descriptor corresponding to the set identifier.
 * 3) if the sort option is enable, this operation is not allowed.
 * 4) allocates and initializes the new node.
 * 5) performs operations from the set options: alloc or not etc.
 * 6) inserts the new node and reorganises the list.
 * 7) increments the number of nodes in the list.
 */

t_error			set_append_ll(i_set			setid,
				      void*			data)
{
  t_set_ll_node		*n;
  o_set*		o;

  assert(data != NULL);

  /*
   * 1)
   */

  if (*((t_id*)data) == ID_UNUSED)
    CORE_ESCAPE("the provided object must begin with a valid identifier");

  /*
   * 2)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 3)
   */

  if (o->options & SET_OPTION_SORT)
    CORE_ESCAPE("unable to append an object to a sorted set");

  /*
   * 4)
   */

  if ((n = malloc(sizeof(t_set_ll_node))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the linked-list node");

  memset(n, 0x0, sizeof(t_set_ll_node));

  /*
   * 5)
   */

  if (o->options & SET_OPTION_ALLOC)
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
   * 6)
   */

  n->previous = o->u.ll.tail;
  n->next = NULL;

  if (n->previous != NULL)
    n->previous->next = n;

  o->u.ll.tail = n;

  if (o->u.ll.head == NULL)
    o->u.ll.head = n;

  /*
   * 7)
   */

  o->size++;

  CORE_LEAVE();
}

/*
 * this function inserts an object before the one referenced by the iterator.
 *
 * steps:
 *
 * 1) checks for a bad identifier.
 * 2) gets the descriptor of the set.
 * 3) does not allow this operation if the sort option is not set.
 * 4) allocates a new node and initializes it.
 * 5) if the alloc option is set, allocates space for the object.
 * 6) inserts the node, so reorganises the linked-list.
 * 7) increments the objects counter.
 */

t_error			set_before_ll(i_set			setid,
				      t_iterator		iterator,
				      void*			data)
{
  t_set_ll_node		*i = iterator.u.ll.node;
  t_set_ll_node		*n;
  o_set*		o;

  assert(data != NULL);

  /*
   * 1)
   */

  if (*((t_id*)data) == ID_UNUSED)
    CORE_ESCAPE("the provided object must begin with a valid identifier");

  /*
   * 2)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 3)
   */

  if (o->options & SET_OPTION_SORT)
    CORE_ESCAPE("unable to insert at a precise location in a sorted set");

  /*
   * 4)
   */

  if ((n = malloc(sizeof(t_set_ll_node))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the linked-list node");

  memset(n, 0x0, sizeof(t_set_ll_node));

  /*
   * 5)
   */

  if (o->options & SET_OPTION_ALLOC)
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
   * 6)
   */

  n->previous = i->previous;
  n->next = i;

  i->previous = n;

  if (n->previous != NULL)
    n->previous->next = n;
  else
    o->u.ll.head = n;

  /*
   * 7)
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
 * 1) checks for a bad identifier.
 * 2) gets the descriptor of the set.
 * 3) does not allow this operation if the sort option is not set.
 * 4) allocates a new node and initializes it.
 * 5) if the alloc option is set, allocates space for the object.
 * 6) inserts the node, so reorganises the linked-list.
 * 7) increments the objects counter.
 */

t_error			set_after_ll(i_set			setid,
				     t_iterator			iterator,
				     void*			data)
{
  t_set_ll_node		*i = iterator.u.ll.node;
  t_set_ll_node		*n;
  o_set*		o;

  assert(data != NULL);

  /*
   * 1)
   */

  if (*((t_id*)data) == ID_UNUSED)
    CORE_ESCAPE("the provided object must begin with a valid identifier");

 /*
   * 2)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 3)
   */

  if (o->options & SET_OPTION_SORT)
    CORE_ESCAPE("unable to insert at a precise location in a sorted set");

  /*
   * 4)
   */

  if ((n = malloc(sizeof(t_set_ll_node))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the linked-list node");

  memset(n, 0x0, sizeof(t_set_ll_node));

  /*
   * 5)
   */

  if (o->options & SET_OPTION_ALLOC)
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
   * 6)
   */

  n->next = i->next;
  n->previous = i;

  i->next = n;

  if (n->next != NULL)
    n->next->previous = n;
  else
    o->u.ll.tail = n;

  /*
   * 7)
   */

  o->size++;

  CORE_LEAVE();
}

/*
 * this function adds an object in the linked-list.
 *
 * steps:
 *
 * 1) avoids bad identifiers.
 * 2) gets the set descriptor corresponding to the set identifier.
 * 3) allocates and initializes the new node for the new object.
 * 4) performs operations from the set options: alloc or not etc.
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

t_error			set_add_ll(i_set			setid,
				   void*			data)
{
  t_set_ll_node*	n;
  o_set*		o;

  assert(data != NULL);

  /*
   * 1)
   */

  if (*((t_id*)data) == ID_UNUSED)
    CORE_ESCAPE("the provided object must begin with a valid identifier");

  /*
   * 2)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 3)
   */

  if ((n = malloc(sizeof(t_set_ll_node))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the linked-list node");

  memset(n, 0x0, sizeof(t_set_ll_node));

  /*
   * 4)
   */

  if (o->options & SET_OPTION_ALLOC)
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

  if (o->options & SET_OPTION_SORT)
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

	  for (tmp = o->u.ll.head; tmp != NULL; tmp = tmp->next)
	    {
	      if (*((t_id*)n->data) == *((t_id*)tmp->data))
		{
		  /*
		   * i)
		   */

		  if ((o->options & SET_OPTION_ALLOC) ||
		      (o->options & SET_OPTION_FREE))
		    free(n->data);

		  free(n);

		  CORE_ESCAPE("identifier collision detected in the set "
			      "%qu on the object identifier %qu",
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

      n->previous = NULL;
      n->next = o->u.ll.head;

      if (n->next != NULL)
	n->next->previous = n;

      o->u.ll.head = n;

      if (o->u.ll.tail == NULL)
	o->u.ll.tail = n;
    }

  /*
   * 6)
   */

  o->size++;

  CORE_LEAVE();
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

t_error			set_remove_ll(i_set			setid,
				      t_id			id)
{
  t_set_ll_node*	tmp;
  o_set*		o;
  t_iterator		i;

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  if (set_locate(setid, id, &i) != ERROR_OK)
    CORE_ESCAPE("unable to locate the object in the set");

  tmp = i.u.ll.node;

  /*
   * 3)
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
   * 4)
   */

  if ((o->options & SET_OPTION_ALLOC) ||
      (o->options & SET_OPTION_FREE))
    free(tmp->data);

  /*
   * 5)
   */

  free(tmp);

  /*
   * 6)
   */

  o->size--;

  CORE_LEAVE();
}

/*
 * this function deletes an element given an iterator.
 *
 * steps:
 *
 * 1) gets the descriptor by its identifier.
 * 2) checks the iterator.
 * 3) updates the links.
 * 4) frees data if necessary.
 * 5) frees node.
 * 6) updates counter.
 */

t_error			set_delete_ll(i_set			setid,
				      t_iterator		iterator)
{
  t_set_ll_node*	n;
  o_set*		o;

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  n = iterator.u.ll.node;
  if (n == NULL)
    CORE_ESCAPE("the object the iterator points to does not exist");

  /*
   * 3)
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
   * 4)
   */

  if (o->options & SET_OPTION_FREE ||
      o->options & SET_OPTION_ALLOC)
    free(n->data);

  /*
   * 5)
   */

  free(n);

  /*
   * 6)
   */

  o->size--;

  CORE_LEAVE();
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

t_error			set_flush_ll(i_set			setid)
{
  t_set_ll_node*	tmp;
  o_set*		o;

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  for (tmp = o->u.ll.tail; tmp != NULL; )
    {
      t_set_ll_node*	t = tmp->previous;

      if ((o->options & SET_OPTION_ALLOC) ||
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
 * this function tries to find an object with its identifier and build
 * a corresponding iterator.
 *
 * steps:
 *
 * 1) checks if the identifier is a correct one.
 * 2) gets the set object corresponding to the set identifier.
 * 3) tries to find the identifier looked for in the set object's elements.
 */

t_error			set_locate_ll(i_set			setid,
				      t_id			id,
				      t_iterator*		iterator)
{
  t_set_ll_node*	tmp;
  o_set*		o;

  assert(iterator != NULL);

  /*
   * 1)
   */

  if (id == ID_UNUSED)
    CORE_ESCAPE("invalid object identifier");

  /*
   * 2)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 3)
   */

  for (tmp = o->u.ll.head; tmp != NULL; tmp = tmp->next)
    {
      if (*((t_id*)tmp->data) == id)
	{
	  iterator->u.ll.node = tmp;

	  CORE_LEAVE();
	}
    }

  CORE_ESCAPE("unable to find the given object identifier in the set");
}

/*
 * this function returns an object given its iterator.
 */

t_error			set_object_ll(i_set			setid,
				      t_iterator		iterator,
				      void**			data)
{
  t_set_ll_node*	n = iterator.u.ll.node;

  *data = n->data;

  CORE_LEAVE();
}

/*
 * this function reserves a set.
 *
 * steps:
 *
 * 1) initializes the set descriptor.
 * 2) avoids bad options.
 * 3) if necessary, reserves an unused identifier for this new set.
 * 4) initializes the set descriptor fields.
 * 5) adds the set descriptor to the set container.
 */

t_error			set_reserve_ll(t_options		options,
				       t_size			datasz,
				       i_set*			setid)
{
  o_set			o;

  assert(datasz >= sizeof (t_id));
  assert(setid != NULL);

  /*
   * 1)
   */

  memset(&o, 0x0, sizeof(o_set));

  /*
   * 2)
   */

  if (options & SET_OPTION_ORGANISE)
    CORE_ESCAPE("unable to set the organise option since linked-list sets "
		"are implicitly organised");

  if ((options & SET_OPTION_ALLOC) && (options & SET_OPTION_FREE))
    CORE_ESCAPE("unable to reserve a set with both alloc and free options");

  /*
   * 3)
   */

  if (options & SET_OPTION_CONTAINER)
    {
      *setid = _set->sets;
    }
  else
    {
      if (id_reserve(&_set->id, setid) != ERROR_OK)
	CORE_ESCAPE("unable to reserve the set identifier");
    }

  /*
   * 4)
   */

  o.id = *setid;
  o.size = 0;
  o.type = SET_TYPE_LL;
  o.options = options;
  o.datasz = datasz;

  o.u.ll.head = NULL;
  o.u.ll.tail = NULL;

  /*
   * 5)
   */

  if (set_new(&o) != ERROR_OK)
    {
      if (!(options & SET_OPTION_CONTAINER))
	id_release(&_set->id, o.id);

      CORE_ESCAPE("unable to register the set descriptor");
    }

  CORE_LEAVE();
}

/*
 * this function releases a set.
 *
 * steps:
 *
 * 1) gets the set given its set identifier.
 * 2) flushs the set to release every objects contained in it.
 * 3) releases the set identifier.
 * 4) then, removes the set from the set container, if possible.
 */

t_error			set_release_ll(i_set			setid)
{
  o_set			*o;

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  if (set_flush(setid) != ERROR_OK)
    CORE_ESCAPE("unable to flush the set");

  /*
   * 3)
   */

  if (id_release(&_set->id, o->id) != ERROR_OK)
    CORE_ESCAPE("unable to release the set identifier");

  /*
   * 4)
   */

  if (!(o->options & SET_OPTION_CONTAINER))
    {
      if (set_destroy(o->id) != ERROR_OK)
	CORE_ESCAPE("unable to destroy the set descriptor");
    }

  CORE_LEAVE();
}

/*
 * this function just returns an error because the ll set does not
 * support this operation.
 */

t_error			set_push_ll(i_set			setid,
				    void*			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error because the ll set does not
 * support this operation.
 */

t_error			set_pop_ll(i_set			setid)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error because the ll set does not
 * support this operation.
 */

t_error			set_pick_ll(i_set			setid,
				    void**			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}
