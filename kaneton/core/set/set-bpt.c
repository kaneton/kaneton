/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/set/set-bpt.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [tue feb  6 22:58:51 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this set implementation provides a balanced+ tree data structure.
 *
 * this set type is build over the header file bpt.h developed by julien
 * quintard. this header file was developed to provide an easy way to build,
 * manage and tune balanced+ trees in C.
 *
 * note that the SORT option must be provided because a non-sorted trees
 * are not conceivable. besides the 'datasz' argument of the set_reserve()
 * function is needed only if the ALLOCATE option is activated.
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
 * the set manager's structure.
 */

extern m_set*		_set;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this macro-function call generates the bpt functions.
 */

bpt_make_functions(set, id, data);

/*
 * this function loads a bpt node from main memory to main memory.
 *
 * in other words this function does nothing but to set the node's attributes.
 */

void			set_load_bpt(t_bpt(set)*		bpt,
				     t_bpt_imm(set)*		node,
				     t_bpt_node(set)		addr)
{
  node->addr = addr;
  node->buf = (void*)addr;
}

/*
 * this function unloads a node.
 *
 * since nodes are stored in main memory, this function has nothing to do.
 */

void			set_unload_bpt(t_bpt(set)*		bpt,
				       t_bpt_imm(set)*		node)
{
}

/*
 * this function compares two bpt addresses.
 */

int			set_addrcmp_bpt(t_bpt(set)*		bpt,
					t_bpt_addr(set)		addr1,
					t_bpt_addr(set)		addr2)
{
  if (addr1 < addr2)
    return (-1);

  if (addr1 > addr2)
    return (1);

  return (0);
}

/*
 * this function compares two bpt keys.
 */

int			set_keycmp_bpt(t_bpt(set)*		bpt,
				       t_bpt_key(set)		key1,
				       t_bpt_key(set)		key2)
{
  if (key1 < key2)
    return (-1);

  if (key1 > key2)
    return (1);

  return (0);
}

/*
 * this function compares two bpt values.
 */

int			set_valcmp_bpt(t_bpt(set)*		bpt,
				       t_bpt_value(set)		value1,
				       t_bpt_value(set)		value2)
{
  if (value1 < value2)
    return (-1);

  if (value1 > value2)
    return (1);

  return (0);
}

/*
 * this function returns true if the object is present in the set.
 */

t_error			set_exist_bpt(i_set			setid,
				      t_id			id)
{
  t_bpt_entry(set)	entry;
  o_set*		o;

  /*
   * 0)
   */

  assert(id != ID_UNUSED);

  /*
   * 1)
   */

  assert(set_descriptor(setid, &o) == ERROR_OK);

  /*
   * 2)
   */

  if (bpt_search(set, &o->u.bpt.bpt, id, &entry) != 0)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function builds an unused object.
 *
 * steps:
 *
 * 1) allocates and initializes the unused object.
 * 2) builds the needed number of elements.
 */

t_error			set_build_bpt(o_set*			o,
				      BPT_NODESZ_T		nodesz)
{
  o->u.bpt.unusedsz = BPT_INIT_SIZE();

  /*
   * 1)
   */

  if ((o->u.bpt.unused.array = malloc(o->u.bpt.unusedsz *
				      sizeof(t_bpt_addr(set)))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the array of unused blocks");

  memset(o->u.bpt.unused.array, 0x0,
	 o->u.bpt.unusedsz * sizeof(t_bpt_addr(set)));

  o->u.bpt.unused.index = -1;

  /*
   * 2)
   */

  for (;
       (o->u.bpt.unused.index + 1) < o->u.bpt.unusedsz;
       o->u.bpt.unused.index++)
    {
      if ((o->u.bpt.unused.array[(o->u.bpt.unused.index + 1)] =
	   malloc(nodesz)) == NULL)
	CORE_ESCAPE("unable to allocate memory for the unused block");
    }

  CORE_LEAVE();
}

/*
 * this function adjust the size and contents of the unused object, allocating
 * or freeing some elements.
 *
 * steps:
 *
 * 1) if the size of the current unused object is not large enough, resize
 *    it to be able to contain the desired elements.
 * 2) if there is not enough elements in the array, fills in with
 *    new ones calling the malloc() function.
 */

t_error			set_adjust_bpt(o_set*			o,
				       t_bpt_uni(set)		alloc,
				       t_bpt_uni(set)		size)
{
  /*
   * 1)
   */

  if (o->u.bpt.unusedsz < size)
    {
      if ((o->u.bpt.unused.array =
	   realloc(o->u.bpt.unused.array, size *
		   sizeof(t_bpt_addr(set)))) == NULL)
	CORE_ESCAPE("unable to reallocate memory for the array of unused "
		    "blocks");

      o->u.bpt.unusedsz = size;
    }

  /*
   * 2)
   */

  if ((o->u.bpt.unused.index + 1) > alloc)
    {
      for (; (o->u.bpt.unused.index + 1) > alloc; o->u.bpt.unused.index--)
	{
	  free(o->u.bpt.unused.array[o->u.bpt.unused.index]);
	  o->u.bpt.unused.array[o->u.bpt.unused.index] = SET_BPT_UADDR;
	}
    }
  else
    {
      for (; (o->u.bpt.unused.index + 1) < alloc; o->u.bpt.unused.index++)
	{
	  if ((o->u.bpt.unused.array[(o->u.bpt.unused.index + 1)] =
	       malloc(o->u.bpt.bpt.nodesz)) == NULL)
	    CORE_ESCAPE("unable to allocate memory for the unused block");
	}
    }

  CORE_LEAVE();
}

/*
 * this function destroys the unused object.
 *
 * steps:
 *
 * 1) frees each unused element of the unused object.
 * 2) frees the unused object itself.
 */

t_error			set_destroy_bpt(o_set*			o)
{
  t_bpt_uni(set)	i;

  /*
   * 1)
   */

  for (i = 0; i <= o->u.bpt.unused.index; i++)
    free(o->u.bpt.unused.array[i]);

  /*
   * 2)
   */

  free(o->u.bpt.unused.array);

  CORE_LEAVE();
}

/*
 * this function shows the set objects.
 *
 * steps:
 *
 * 1) gets the descriptor from its set identifier.
 * 2) prints a message for each objects of the set.
 */

t_error			set_show_bpt(i_set			setid,
				     mt_margin			margin)
{
  t_state		state;
  o_set*		o;
  s_iterator		i;

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  module_call(console, message,
	      '#', "  %qd node(s) from the balanced+ tree set %qu:\n",
	      o->size,
	      setid);

  set_foreach(SET_OPTION_FORWARD, setid, &i, state)
    {
      t_bpt_imm(set)		node;
      t_bpt_lfentry(set)*	leaf;

      BPT_LOAD(&o->u.bpt.bpt, &node, i.u.bpt.entry.node);

      leaf = BPT_LFENTRY(set, &node, i.u.bpt.entry.ndi);

      module_call(console, message,
		  '#', "    %qu <%qu 0x%x> [0x%x:%u]\n",
		  leaf->id, *((t_id*)leaf->data), leaf->data,
		  i.u.bpt.entry.node, i.u.bpt.entry.ndi);

      BPT_UNLOAD(&o->u.bpt.bpt, &node);
    }

  CORE_LEAVE();
}

/*
 * this function gets the first object of the set using the bpt_list()
 * function with the BPT_OPT_HEAD option.
 */

t_error			set_head_bpt(i_set			setid,
				     s_iterator*		iterator)
{
  t_bpt_imm(set)	root;
  o_set*		o;

  assert(iterator != NULL);

  assert(set_descriptor(setid, &o) == ERROR_OK);

  BPT_LOAD(&o->u.bpt.bpt, &root, o->u.bpt.bpt.root);

  if (bpt_list(set, &o->u.bpt.bpt, &root,
	       &iterator->u.bpt.entry, BPT_OPT_HEAD) != 0)
    {
      BPT_UNLOAD(&o->u.bpt.bpt, &root);

      CORE_FALSE();
    }

  BPT_UNLOAD(&o->u.bpt.bpt, &root);

  CORE_TRUE();
}

/*
 * this function gets the last object of the set using the bpt_list()
 * function with the BPT_OPT_TAIL option.
 */

t_error			set_tail_bpt(i_set			setid,
				     s_iterator*		iterator)
{
  t_bpt_imm(set)	root;
  o_set*		o;

  assert(iterator != NULL);

  assert(set_descriptor(setid, &o) == ERROR_OK);

  BPT_LOAD(&o->u.bpt.bpt, &root, o->u.bpt.bpt.root);

  if (bpt_list(set, &o->u.bpt.bpt, &root,
	       &iterator->u.bpt.entry, BPT_OPT_TAIL) != 0)
    {
      BPT_UNLOAD(&o->u.bpt.bpt, &root);

      CORE_FALSE();
    }

  BPT_UNLOAD(&o->u.bpt.bpt, &root);

  CORE_TRUE();
}

/*
 * this function returns the previous node of the current iterator.
 */

t_error			set_previous_bpt(i_set			setid,
					 s_iterator		current,
					 s_iterator*		previous)
{
  o_set*		o;

  assert(previous != NULL);

  assert(set_descriptor(setid, &o) == ERROR_OK);

  if (bpt_prev_entry(set, &o->u.bpt.bpt, current.u.bpt.entry,
		     &previous->u.bpt.entry, BPT_OPT_TREE) != 0)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function returns the next node of the current iterator.
 */

t_error			set_next_bpt(i_set			setid,
				     s_iterator			current,
				     s_iterator*		next)
{
  o_set*		o;

  assert(next != NULL);

  assert(set_descriptor(setid, &o) == ERROR_OK);

  if (bpt_next_entry(set, &o->u.bpt.bpt, current.u.bpt.entry,
		     &next->u.bpt.entry, BPT_OPT_TREE) != 0)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function just returns an error because the bpt set only
 * works with the sort option.
 */

t_error			set_insert_bpt(i_set			setid,
				       void*			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error because the bpt set only
 * works with the sort option.
 */

t_error			set_append_bpt(i_set			setid,
				       void*			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error because the bpt set only
 * works with the sort option.
 */

t_error			set_before_bpt(i_set			setid,
				       s_iterator		iterator,
				       void*			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error because the bpt set only
 * works with the sort option.
 */

t_error			set_after_bpt(i_set			setid,
				      s_iterator		iterator,
				      void*			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function builds a leaf entry to add the user object.
 *
 * steps:
 *
 * 1) checks for non-valid identifiers.
 * 2) gets the set descriptor object from its identifier.
 * 3) initializes the leaf entry.
 * 4) handles the options: allocate or not the object.
 * 5) adjusts the bpt unused object to perform the next operation.
 * 6) adds the built leaf entry to the tree.
 * 7) updates the set counter.
 */

t_error			set_add_bpt(i_set			setid,
				    void*			data)
{
  t_bpt_lfentry(set)	lfentry;
  o_set*		o;

  assert(data != NULL);

  /*
   * 1)
   */

  if (*(t_id*)data == ID_UNUSED)
    CORE_ESCAPE("the object must begin with a valid identifier");

  /*
   * 2)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 3)
   */

  memset(&lfentry, 0x0, sizeof(t_bpt_lfentry(set)));

  lfentry.id = *((t_id*)data);

  /*
   * 4)
   */

  if (o->options & SET_OPTION_ALLOCATE)
    {
      if ((lfentry.data = malloc(o->datasz)) == NULL)
	CORE_ESCAPE("unable to allocate memory for the object's copy");

      memcpy(lfentry.data, data, o->datasz);
    }
  else
    {
      lfentry.data = data;
    }

  /*
   * 5)
   */

  if (set_adjust_bpt(o, BPT_ADD_ALLOC(&o->u.bpt.bpt),
		     BPT_ADD_SIZE(&o->u.bpt.bpt)) != ERROR_OK)
    CORE_ESCAPE("unable to adjust the array of unused blocks");

  /*
   * 6)
   */

  if (bpt_add(set, &o->u.bpt.bpt, &lfentry, &o->u.bpt.unused) != 0)
    CORE_ESCAPE("unable to add the object to the tree");

  /*
   * 7)
   */

  o->size++;

  CORE_LEAVE();
}

/*
 * this function removes an object from the tree.
 *
 * the leaf entry has to be retrieved before to release it because
 * the data may be need to be freed.
 *
 * steps:
 *
 * 1) checks for bad identifiers.
 * 2) gets the set descriptor.
 * 3) if the alloc option was set, locates the leaf entry holding
 *    the object and frees the object.
 * 4) adjusts the bpt unused object.
 * 5) removes the leaf entry given the identifier.
 * 6) updates the set counter.
 */

t_error			set_remove_bpt(i_set			setid,
				       t_id			id)
{
  t_bpt_entry(set)	entry;
  t_bpt_imm(set)	node;
  o_set*		o;

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

  if ((o->options & SET_OPTION_ALLOCATE) ||
      (o->options & SET_OPTION_FREE))
    {
      if (bpt_search(set, &o->u.bpt.bpt, id, &entry) != 0)
	CORE_ESCAPE("unable to locate the given object");

      BPT_LOAD(&o->u.bpt.bpt, &node, entry.node);

      free(BPT_GET_LFENTRY(set, &node, entry.ndi, data));

      BPT_UNLOAD(&o->u.bpt.bpt, &node);
    }

  /*
   * 4)
   */

  if (set_adjust_bpt(o, BPT_REMOVE_ALLOC(&o->u.bpt.bpt),
		     BPT_REMOVE_SIZE(&o->u.bpt.bpt)) != ERROR_OK)
    CORE_ESCAPE("unable to adjust the array of unused blocks");

  /*
   * 5)
   */

  if (bpt_remove(set, &o->u.bpt.bpt, id, &o->u.bpt.unused) != 0)
    CORE_ESCAPE("unable to remove the object from the tree");

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
 * 1) gets the set descriptor.
 * 2) frees element if necessary
 * 3) adjusts the bpt unused object.
 * 4) removes the leaf entry given the bpt entry.
 * 5) decrements the number of elements in the set.
 */

t_error			set_delete_bpt(i_set			setid,
				       s_iterator		iterator)
{
  t_bpt_imm(set)	node;
  o_set*		o;

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  if (o->options & SET_OPTION_ALLOCATE ||
      o->options & SET_OPTION_FREE)
    {
      BPT_LOAD(&o->u.bpt.bpt, &node, iterator.u.bpt.entry.node);

      free(BPT_GET_LFENTRY(set, &node, iterator.u.bpt.entry.ndi, data));

      BPT_UNLOAD(&o->u.bpt.bpt, &node);
    }

  /*
   * 3)
   */

 if (set_adjust_bpt(o, BPT_REMOVE_ALLOC(&o->u.bpt.bpt),
		    BPT_REMOVE_SIZE(&o->u.bpt.bpt)) != ERROR_OK)
   CORE_ESCAPE("unable to adjust the array of unused blocks");

 /*
  * 4)
  */

 if (bpt_collide_remove(set, &o->u.bpt.bpt, iterator.u.bpt.entry,
			&o->u.bpt.unused) != 0)
   CORE_ESCAPE("unable to remove the object from the tree");

  /*
   * 5)
   */

  o->size--;

  CORE_LEAVE();
}

/*
 * this function flushs the contents of the bpt set.
 *
 * this action results in the destruction of the bpt internal data structure,
 * so we have to call bpt_init() to rebuild the tree data structure.
 *
 * steps:
 *
 * 1) gets the set descriptor from the given identifier.
 * 2) for each elements of the tree, frees the data and resets the
 *    element.
 * 3) adjusts the unused object to be usable with the bpt functions.
 *    then, calls the bpt_clean() function to destroy the tree.
 * 4) adjusts the unused object and calls the bpt_init() function to
 *    rebuild the tree data structure.
 * 5) resets the size of the data structure.
 */

t_error			set_flush_bpt(i_set			setid)
{
  t_bpt_nodesz(set)	nodesz;
  t_state		state;
  s_iterator		i;
  o_set*		o;

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  nodesz = o->u.bpt.bpt.nodesz;

  /*
   * 2)
   */

  if ((o->options & SET_OPTION_ALLOCATE) ||
      (o->options & SET_OPTION_FREE))
    {
      set_foreach(SET_OPTION_FORWARD, setid, &i, state)
	{
	  t_bpt_imm(set)	node;
	  t_bpt_lfentry(set)*	leaf;

	  BPT_LOAD(&o->u.bpt.bpt, &node, i.u.bpt.entry.node);

	  leaf = BPT_LFENTRY(set, &node, i.u.bpt.entry.ndi);

	  free(leaf->data);

	  leaf->data = SET_BPT_UADDR;

	  BPT_UNLOAD(&o->u.bpt.bpt, &node);
	}
    }

  /*
   * 3)
   */

  if (set_adjust_bpt(o, BPT_CLEAN_ALLOC(&o->u.bpt.bpt),
		     BPT_CLEAN_SIZE(&o->u.bpt.bpt)) != ERROR_OK)
    CORE_ESCAPE("unable to adjust the array of unused blocks");

  if (bpt_clean(set, &o->u.bpt.bpt, &o->u.bpt.unused) != 0)
    CORE_ESCAPE("unable to clean the tree of all the elements");

  /*
   * 4)
   */

  if (set_adjust_bpt(o, BPT_INIT_ALLOC(),
		     BPT_INIT_SIZE()) != ERROR_OK)
    CORE_ESCAPE("unable to adjust the array of unused blocks");

  if (bpt_init(set, &o->u.bpt.bpt, nodesz,
	       SET_BPT_UADDR, SET_BPT_UKEY, SET_BPT_UVALUE,
	       BPT_FLAG_ZERO, set_load_bpt, set_unload_bpt,
	       set_addrcmp_bpt, set_keycmp_bpt, set_valcmp_bpt,
	       NULL, NULL, &o->u.bpt.unused) != 0)
    CORE_ESCAPE("unable to re-initialize the tree");

  /*
   * 5)
   */

  o->size = 0;

  CORE_LEAVE();
}

/*
 * this function locates the leaf entry corresponding to the given
 * identifier, then builds an iterator for it.
 */

t_error			set_locate_bpt(i_set			setid,
				       t_id			id,
				       s_iterator*		iterator)
{
  t_bpt_entry(set)	entry;
  o_set*		o;

  assert(iterator != NULL);

  if (id == ID_UNUSED)
    CORE_ESCAPE("invalid object identifier");

  if (set_descriptor(setid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  if (bpt_search(set, &o->u.bpt.bpt, id, &entry) != 0)
    CORE_ESCAPE("unable to locate the element in the tree");

  memcpy(iterator, &entry, sizeof(t_bpt_entry(set)));

  CORE_LEAVE();
}

/*
 * this function returns the object from its iterator.
 */

t_error			set_object_bpt(i_set			setid,
				       s_iterator		iterator,
				       void**			data)
{
  t_bpt_imm(set)	node;
  o_set*		o;

  assert(data != NULL);

  if (set_descriptor(setid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  BPT_LOAD(&o->u.bpt.bpt, &node, iterator.u.bpt.entry.node);

  *data = BPT_GET_LFENTRY(set, &node, iterator.u.bpt.entry.ndi, data);

  BPT_UNLOAD(&o->u.bpt.bpt, &node);

  CORE_LEAVE();
}

/*
 * this function reserves a set.
 *
 * steps:
 *
 * 1) initializes the set object.
 * 2) checks for the options.
 * 3) reserves a identifier for the new set.
 * 4) initializes the set object.
 * 5) builds the unused object for the bpt calls.
 * 6) calls the bpt_init() function which initializes the tree data structure.
 * 7) adds the new set object to the set container.
 */

t_error			set_reserve_bpt(t_options		options,
					t_size			datasz,
					t_bpt_nodesz(set)	nodesz,
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

  if (!(options & SET_OPTION_SORT))
    CORE_ESCAPE("the sort option cannot be provided since bpt-based are "
		"always sorted");

  if (options & SET_OPTION_ORGANISE)
    CORE_ESCAPE("the organise option is invalid for bpt-based sets");

  if ((options & SET_OPTION_ALLOCATE) && (options & SET_OPTION_FREE))
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
	CORE_ESCAPE("unable to reserve an identifier");
    }

  /*
   * 4)
   */

  o.id = *setid;
  o.size = 0;
  o.type = SET_TYPE_BPT;
  o.options = options;
  o.datasz = datasz;

  /*
   * 5)
   */

  if (set_build_bpt(&o, nodesz) != ERROR_OK)
    {
      set_destroy_bpt(&o);

      if (!(options & SET_OPTION_CONTAINER))
	id_release(&_set->id, o.id);

      CORE_ESCAPE("unable to build the array of unused blocks");
    }

  /*
   * 6)
   */

  if (bpt_init(set, &o.u.bpt.bpt, nodesz,
	       SET_BPT_UADDR, SET_BPT_UKEY, SET_BPT_UVALUE,
	       BPT_FLAG_ZERO, set_load_bpt, set_unload_bpt,
	       set_addrcmp_bpt, set_keycmp_bpt, set_valcmp_bpt,
	       NULL, NULL, &o.u.bpt.unused) != 0)
    {
      if (!(options & SET_OPTION_CONTAINER))
	id_release(&_set->id, o.id);

      CORE_ESCAPE("unable to initialize the tree");
    }

  /*
   * 7)
   */

  if (set_new(&o) != ERROR_OK)
    {
      set_adjust_bpt(&o, BPT_CLEAN_ALLOC(&o.u.bpt.bpt),
		     BPT_CLEAN_SIZE(&o.u.bpt.bpt));

      bpt_clean(set, &o.u.bpt.bpt, &o.u.bpt.unused);

      set_destroy_bpt(&o);

      if (!(options & SET_OPTION_CONTAINER))
	id_release(&_set->id, o.id);

      CORE_ESCAPE("unable to register the new set descriptor");
    }

  CORE_LEAVE();
}

/*
 * this function releases the bpt set.
 *
 * steps:
 *
 * 1) gets the set descriptor from the given identifier.
 * 2) flushs the set's contents.
 * 3) builds a unused object to be able to call the bpt_clean() function.
 *    in fact we call the bpt_clean() function even after calling the
 *    set_flush() function because after the flush the tree still contains
 *    an allocated node so this one has to be released with the bpt_clean()
 *    call.
 * 4) releases the set identifier.
 * 5) removes the set object from the set container.
 */

t_error			set_release_bpt(i_set			setid)
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

  if (set_adjust_bpt(o, BPT_CLEAN_ALLOC(&o->u.bpt.bpt),
		     BPT_CLEAN_SIZE(&o->u.bpt.bpt)) != ERROR_OK)
    CORE_ESCAPE("unable to adjust the array of unused blocks");

  if (bpt_clean(set, &o->u.bpt.bpt, &o->u.bpt.unused) != 0)
    CORE_ESCAPE("unable to clean the tree");

  if (set_destroy_bpt(o) != ERROR_OK)
    CORE_ESCAPE("unable to destroy the set");

  /*
   * 4)
   */

  if (id_release(&_set->id, o->id) != ERROR_OK)
    CORE_ESCAPE("unable to release the set identifier");

  /*
   * 5)
   */

  if (set_destroy(o->id) != ERROR_OK)
    CORE_ESCAPE("unable to destroy the set descriptor");

  CORE_LEAVE();
}

/*
 * this function just returns an error because the bpt set only
 * works with the sort option.
 */

t_error			set_push_bpt(i_set			setid,
				     void*			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error because the bpt set only
 * works with the sort option.
 */

t_error			set_pop_bpt(i_set			setid)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error because the bpt set only
 * works with the sort option.
 */

t_error			set_pick_bpt(i_set			setid,
				     void**			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}
