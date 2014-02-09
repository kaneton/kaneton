/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/set/set-bpt.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       julien quintard   [sun jan 30 20:35:34 2011]
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

extern m_set		_set;

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
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) search for the identifier in the tree and return false if not found.
 */

t_bool			set_exist_bpt(i_set			setid,
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

  assert(set_descriptor(setid, &o) == STATUS_OK);

  /*
   * 2)
   */

  if (bpt_search(set, &o->u.bpt.bpt, id, &entry) != 0)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function builds the initial version of the array of unused blocks.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) initialize the array's size in order to initialize (init operation)
 *    the tree.
 * 2) allocate the array.
 * 3) initialize the array's content.
 * 4) fill the required number of entries with blocks allocated in
 *    main memory.
 */

t_status		set_build_bpt(o_set*			object,
				      BPT_NODESZ_T		nodesz)
{
  /*
   * 0)
   */

  if (object == NULL)
    CORE_ESCAPE("the 'object' argument is null");

  /*
   * 1)
   */

  object->u.bpt.unusedsz = BPT_INIT_SIZE();

  /*
   * 2)
   */

  if ((object->u.bpt.unused.array = malloc(object->u.bpt.unusedsz *
					   sizeof (t_bpt_addr(set)))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the array of unused blocks");

  /*
   * 3)
   */

  memset(object->u.bpt.unused.array, 0x0,
	 object->u.bpt.unusedsz * sizeof (t_bpt_addr(set)));

  object->u.bpt.unused.index = -1;

  /*
   * 4)
   */

  for (;
       (object->u.bpt.unused.index + 1) < object->u.bpt.unusedsz;
       object->u.bpt.unused.index++)
    {
      if ((object->u.bpt.unused.array[(object->u.bpt.unused.index + 1)] =
	   malloc(nodesz)) == NULL)
	CORE_ESCAPE("unable to allocate memory for the unused block");
    }

  CORE_LEAVE();
}

/*
 * this function adjusts the size and content of the array of unused
 * blocks, hence possibly allocating additional blocks or releasing others.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) if the size of the current array is not large enough, resize it.
 * 2) if there is not enough elements in the array, fill it with
 *    additional ones.
 */

t_status		set_adjust_bpt(o_set*			object,
				       t_bpt_uni(set)		alloc,
				       t_bpt_uni(set)		size)
{
  /*
   * 0)
   */

  if (object == NULL)
    CORE_ESCAPE("the 'object' argument is null");

  /*
   * 1)
   */

  if (object->u.bpt.unusedsz < size)
    {
      if ((object->u.bpt.unused.array =
	   realloc(object->u.bpt.unused.array, size *
		   sizeof (t_bpt_addr(set)))) == NULL)
	CORE_ESCAPE("unable to reallocate memory for the array of unused "
		    "blocks");

      object->u.bpt.unusedsz = size;
    }

  /*
   * 2)
   */

  if ((object->u.bpt.unused.index + 1) > alloc)
    {
      for (;
	   (object->u.bpt.unused.index + 1) > alloc;
	   object->u.bpt.unused.index--)
	{
	  free(object->u.bpt.unused.array[object->u.bpt.unused.index]);

	  object->u.bpt.unused.array[object->u.bpt.unused.index] =
	    SET_BPT_UADDR;
	}
    }
  else
    {
      for (;
	   (object->u.bpt.unused.index + 1) < alloc;
	   object->u.bpt.unused.index++)
	{
	  if ((object->u.bpt.unused.array[(object->u.bpt.unused.index + 1)] =
	       malloc(object->u.bpt.bpt.nodesz)) == NULL)
	    CORE_ESCAPE("unable to allocate memory for the unused block");
	}
    }

  CORE_LEAVE();
}

/*
 * this function destroys an array of unused blocks.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) free each block from the array.
 * 2) free the array itself.
 */

t_status		set_destroy_bpt(o_set*			object)
{
  t_bpt_uni(set)	i;

  /*
   * 0)
   */

  if (object == NULL)
    CORE_ESCAPE("the 'object' argument is null");

  /*
   * 1)
   */

  for (i = 0; i <= object->u.bpt.unused.index; i++)
    free(object->u.bpt.unused.array[i]);

  /*
   * 2)
   */

  free(object->u.bpt.unused.array);

  CORE_LEAVE();
}

/*
 * this function shows the set's attributes.
 *
 * steps:
 *
 * 1) gets the descriptor from its set identifier.
 * 2) prints a message for each objects of the set.
 */

t_status		set_show_bpt(i_set			setid,
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
	      "set: id(%qd) type(bpt) size(%qd) datasz(%u) options(%s) "
	      "array(0x%x:%u) arraysz(%d)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      o->id,
	      o->size,
	      o->datasz,
	      options,
	      o->u.bpt.unused.array,
	      o->u.bpt.unused.index,
	      o->u.bpt.unusedsz);

  /*
   * 4)
   */

  set_foreach(SET_OPTION_FORWARD, setid, &i, state)
    {
      t_bpt_imm(set)		node;
      t_bpt_lfentry(set)*	leaf;

      BPT_LOAD(&o->u.bpt.bpt, &node, i.u.bpt.entry.node);

      leaf = BPT_LFENTRY(set, &node, i.u.bpt.entry.ndi);

      module_call(console, message,
		  '#',
		  MODULE_CONSOLE_MARGIN_FORMAT
		  "  object: id(%qd) entry(0x%x:%u) data(0x%x)\n",
		  MODULE_CONSOLE_MARGIN_VALUE(margin),
		  leaf->id,
		  i.u.bpt.entry.node,
		  i.u.bpt.entry.ndi,
		  leaf->data);

      BPT_UNLOAD(&o->u.bpt.bpt, &node);
    }

  CORE_LEAVE();
}

/*
 * this function returns an iterator on the first object.
 *
 * this function relies on th bpt_list() function with the BPT_OPT_HEAD option
 * to walk through the objects in a sequentially manner.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) load the node.
 * 3) look for the first entry and return true if found.
 * 4) unload the node.
 */

t_bool			set_head_bpt(i_set			setid,
				     s_iterator*		iterator)
{
  t_bpt_imm(set)	root;
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

  BPT_LOAD(&o->u.bpt.bpt, &root, o->u.bpt.bpt.root);

  /*
   * 3)
   */

  if (bpt_list(set, &o->u.bpt.bpt, &root,
	       &iterator->u.bpt.entry, BPT_OPT_HEAD) != 0)
    {
      BPT_UNLOAD(&o->u.bpt.bpt, &root);

      CORE_FALSE();
    }

  /*
   * 4)
   */

  BPT_UNLOAD(&o->u.bpt.bpt, &root);

  CORE_TRUE();
}

/*
 * this function looks for the last entry.
 *
 * this function relies on th bpt_list() function with the BPT_OPT_TAIL option
 * to walk through the objects in a sequentially manner.
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) load the node.
 * 3) look for the first entry and return true if found.
 * 4) unload the node.
 */

t_bool			set_tail_bpt(i_set			setid,
				     s_iterator*		iterator)
{
  t_bpt_imm(set)	root;
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

  BPT_LOAD(&o->u.bpt.bpt, &root, o->u.bpt.bpt.root);

  /*
   * 3)
   */

  if (bpt_list(set, &o->u.bpt.bpt, &root,
	       &iterator->u.bpt.entry, BPT_OPT_TAIL) != 0)
    {
      BPT_UNLOAD(&o->u.bpt.bpt, &root);

      CORE_FALSE();
    }

  /*
   * 4)
   */

  BPT_UNLOAD(&o->u.bpt.bpt, &root);

  CORE_TRUE();
}

/*
 * this function returns an iterator on the previous node.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) retrieve the previous entry.
 */

t_bool			set_previous_bpt(i_set			setid,
					 s_iterator		current,
					 s_iterator*		previous)
{
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

  if (bpt_prev_entry(set, &o->u.bpt.bpt, current.u.bpt.entry,
		     &previous->u.bpt.entry, BPT_OPT_TREE) != 0)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function returns an iterator on the next node.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) retrieve the previous entry.
 */

t_bool			set_next_bpt(i_set			setid,
				     s_iterator			current,
				     s_iterator*		next)
{
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

  if (bpt_next_entry(set, &o->u.bpt.bpt, current.u.bpt.entry,
		     &next->u.bpt.entry, BPT_OPT_TREE) != 0)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function just returns an error because the bpt set only
 * works with the sort option.
 */

t_status		set_insert_bpt(i_set			setid,
				       void*			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error because the bpt set only
 * works with the sort option.
 */

t_status		set_append_bpt(i_set			setid,
				       void*			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error because the bpt set only
 * works with the sort option.
 */

t_status		set_before_bpt(i_set			setid,
				       s_iterator		iterator,
				       void*			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error because the bpt set only
 * works with the sort option.
 */

t_status		set_after_bpt(i_set			setid,
				      s_iterator		iterator,
				      void*			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function adds the given object to the tree.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) initialize the leaf entry.
 * 3) complete the leaf entry depending on the options i.e clone the
 *    object if necessary.
 * 4) adjust the size and filling of the array of unused blocks.
 * 5) add the entry to the tree.
 * 6) update the set's size.
 */

t_status		set_add_bpt(i_set			setid,
				    void*			data)
{
  t_bpt_lfentry(set)	lfentry;
  o_set*		o;

  /*
   * 0)
   */

  if (data == NULL)
    CORE_ESCAPE("the 'data' argument is null");

  if (*(t_id*)data == ID_UNUSED)
    CORE_ESCAPE("the object must begin with a valid identifier");

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  memset(&lfentry, 0x0, sizeof (t_bpt_lfentry(set)));

  lfentry.id = *((t_id*)data);

  /*
   * 3)
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
   * 4)
   */

  if (set_adjust_bpt(o,
		     BPT_ADD_ALLOC(&o->u.bpt.bpt),
		     BPT_ADD_SIZE(&o->u.bpt.bpt)) != STATUS_OK)
    CORE_ESCAPE("unable to adjust the array of unused blocks");

  /*
   * 5)
   */

  if (bpt_add(set, &o->u.bpt.bpt, &lfentry, &o->u.bpt.unused) != 0)
    CORE_ESCAPE("unable to add the object to the tree");

  /*
   * 6)
   */

  o->size++;

  CORE_LEAVE();
}

/*
 * this function removes an object from the tree.
 *
 * note that the leaf entry may have to be retrieved in order to release
 * the object's memory.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) depending on the options, retrieve the entry and release the object's
 *    memory.
 * 3) adjust the size and filling of the array of unused blocks.
 * 4) remove the object from the tree.
 * 5) update the set's size.
 */

t_status		set_remove_bpt(i_set			setid,
				       t_id			id)
{
  t_bpt_entry(set)	entry;
  t_bpt_imm(set)	node;
  o_set*		o;

  /*
   * 0)
   */

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
   * 3)
   */

  if (set_adjust_bpt(o,
		     BPT_REMOVE_ALLOC(&o->u.bpt.bpt),
		     BPT_REMOVE_SIZE(&o->u.bpt.bpt)) != STATUS_OK)
    CORE_ESCAPE("unable to adjust the array of unused blocks");

  /*
   * 4)
   */

  if (bpt_remove(set, &o->u.bpt.bpt, id, &o->u.bpt.unused) != 0)
    CORE_ESCAPE("unable to remove the object from the tree");

  /*
   * 5)
   */

  o->size--;

  CORE_LEAVE();
}

/*
 * this function deletes the object referenced by the given iterator.
 *
 * steps:
 *
 * 1) retrieve the set descriptor.
 * 2) if necessary, retrieve the object and release its memory.
 * 3) adjusts the size and filling of the array of unused blocks.
 * 4) removes the object from the tree.
 * 5) update the set's size.
 */

t_status		set_delete_bpt(i_set			setid,
				       s_iterator		iterator)
{
  t_bpt_imm(set)	node;
  o_set*		o;

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != STATUS_OK)
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

 if (set_adjust_bpt(o,
		    BPT_REMOVE_ALLOC(&o->u.bpt.bpt),
		    BPT_REMOVE_SIZE(&o->u.bpt.bpt)) != STATUS_OK)
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
 * this function flushes the contents of the tree.
 *
 * this action results in the destruction of the bpt internal data structure.
 * therefore, the function ends by calling the bpt_init() function in order
 * to rebuild the tree data structure.
 *
 * steps:
 *
 * 1) retrieve the set descriptor.
 * 2) save the size of the nodes from the current tree configuration.
 * 3) if necessary, walk through the objects and release their memory.
 * 4) adjust the size and filling of the array of unused objects and
 *    clean the tree data structure.
 * 5) re-adjusts the array of unused blocks for the next operation and
 *    re-initialize the tree.
 * 6) resets the set's size to zero.
 */

t_status		set_flush_bpt(i_set			setid)
{
  t_bpt_nodesz(set)	nodesz;
  t_state		state;
  s_iterator		i;
  o_set*		o;

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  nodesz = o->u.bpt.bpt.nodesz;

  /*
   * 3)
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
   * 4)
   */

  if (set_adjust_bpt(o,
		     BPT_CLEAN_ALLOC(&o->u.bpt.bpt),
		     BPT_CLEAN_SIZE(&o->u.bpt.bpt)) != STATUS_OK)
    CORE_ESCAPE("unable to adjust the array of unused blocks");

  if (bpt_clean(set, &o->u.bpt.bpt, &o->u.bpt.unused) != 0)
    CORE_ESCAPE("unable to clean the tree of all the elements");

  /*
   * 5)
   */

  if (set_adjust_bpt(o,
		     BPT_INIT_ALLOC(),
		     BPT_INIT_SIZE()) != STATUS_OK)
    CORE_ESCAPE("unable to adjust the array of unused blocks");

  if (bpt_init(set, &o->u.bpt.bpt, nodesz,
	       SET_BPT_UADDR, SET_BPT_UKEY, SET_BPT_UVALUE,
	       BPT_FLAG_ZERO, set_load_bpt, set_unload_bpt,
	       set_addrcmp_bpt, set_keycmp_bpt, set_valcmp_bpt,
	       NULL, NULL, &o->u.bpt.unused) != 0)
    CORE_ESCAPE("unable to re-initialize the tree");

  /*
   * 6)
   */

  o->size = 0;

  CORE_LEAVE();
}

/*
 * this function locates the leaf entry corresponding to the given
 * identifier, then returns an iterator for it.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) search the entry with the given identifier.
 * 3) set the iterator.
 */

t_status		set_locate_bpt(i_set			setid,
				       t_id			id,
				       s_iterator*		iterator)
{
  t_bpt_entry(set)	entry;
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

  if (bpt_search(set, &o->u.bpt.bpt, id, &entry) != 0)
    CORE_ESCAPE("unable to locate the element in the tree");

  /*
   * 3)
   */

  memcpy(&iterator->u.bpt.entry, &entry, sizeof (t_bpt_entry(set)));

  CORE_LEAVE();
}

/*
 * this function returns the object from the given iterator.
 *
 * steps:
 *
 * 1) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) load the node.
 * 3) retrieve the data from the node's entry.
 * 4) unload the node.
 */

t_status		set_object_bpt(i_set			setid,
				       s_iterator		iterator,
				       void**			data)
{
  t_bpt_imm(set)	node;
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

  BPT_LOAD(&o->u.bpt.bpt, &node, iterator.u.bpt.entry.node);

  /*
   * 3)
   */

  *data = BPT_GET_LFENTRY(set, &node, iterator.u.bpt.entry.ndi, data);

  /*
   * 4)
   */

  BPT_UNLOAD(&o->u.bpt.bpt, &node);

  CORE_LEAVE();
}

/*
 * this function reserves a set according to the given options.
 *
 * note that while the 'datasz' argument specifies the size of the objects
 * to be stored, the 'nodesz' indicates the size of the internal and leaf
 * nodes.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) assign an identifier, depending on the options: either this is
 *    the set container or not. if it is, take the set container identifier
 *    that is in the set manager.
 * 2) initialize and fill the set descriptor.
 * 3) build the initial array of unused blocks with enough filling to
 *    satisfy the bpt_init() operation.
 * 4) set up the tree data structure.
 * 5) register the set descriptor.
 */

t_status		set_reserve_bpt(t_options		options,
					t_size			datasz,
					t_bpt_nodesz(set)	nodesz,
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

  if (!(options & SET_OPTION_SORT))
    CORE_ESCAPE("the sort option cannot be provided since bpt-based are "
		"always sorted");

  if (options & SET_OPTION_ORGANISE)
    CORE_ESCAPE("the organise option is invalid for bpt-based sets");

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
	CORE_ESCAPE("unable to reserve an identifier");
    }

  /*
   * 2)
   */

  memset(&o, 0x0, sizeof (o_set));

  o.id = *id;
  o.size = 0;
  o.type = SET_TYPE_BPT;
  o.options = options;
  o.datasz = datasz;

  /*
   * 3)
   */

  if (set_build_bpt(&o, nodesz) != STATUS_OK)
    {
      set_destroy_bpt(&o);

      if (!(options & SET_OPTION_CONTAINER))
	id_release(&_set.id, o.id);

      CORE_ESCAPE("unable to build the array of unused blocks");
    }

  /*
   * 4)
   */

  if (bpt_init(set, &o.u.bpt.bpt, nodesz,
	       SET_BPT_UADDR, SET_BPT_UKEY, SET_BPT_UVALUE,
	       BPT_FLAG_ZERO, set_load_bpt, set_unload_bpt,
	       set_addrcmp_bpt, set_keycmp_bpt, set_valcmp_bpt,
	       NULL, NULL, &o.u.bpt.unused) != 0)
    {
      if (!(options & SET_OPTION_CONTAINER))
	id_release(&_set.id, o.id);

      CORE_ESCAPE("unable to initialize the tree");
    }

  /*
   * 5)
   */

  if (set_new(&o) != STATUS_OK)
    {
      set_adjust_bpt(&o,
		     BPT_CLEAN_ALLOC(&o.u.bpt.bpt),
		     BPT_CLEAN_SIZE(&o.u.bpt.bpt));

      bpt_clean(set, &o.u.bpt.bpt, &o.u.bpt.unused);

      set_destroy_bpt(&o);

      if (!(options & SET_OPTION_CONTAINER))
	id_release(&_set.id, o.id);

      CORE_ESCAPE("unable to register the new set descriptor");
    }

  CORE_LEAVE();
}

/*
 * this function releases the set.
 *
 * steps:
 *
 * 1) retrieve the set descriptor.
 * 2) flush the set's content.
 * 3) adjust the size and filling of the array of unused blocks and
 *    clean the tree data structure.
 * 4) destroy the array of unused blocks.
 * 5) release the set identifier.
 * 6) remove the set descriptor from the set container.
 */

t_status		set_release_bpt(i_set			setid)
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

  if (set_adjust_bpt(o,
		     BPT_CLEAN_ALLOC(&o->u.bpt.bpt),
		     BPT_CLEAN_SIZE(&o->u.bpt.bpt)) != STATUS_OK)
    CORE_ESCAPE("unable to adjust the array of unused blocks");

  if (bpt_clean(set, &o->u.bpt.bpt, &o->u.bpt.unused) != 0)
    CORE_ESCAPE("unable to clean the tree");

  /*
   * 4)
   */

  if (set_destroy_bpt(o) != STATUS_OK)
    CORE_ESCAPE("unable to destroy the set");

  /*
   * 5)
   */

  if (id_release(&_set.id, o->id) != STATUS_OK)
    CORE_ESCAPE("unable to release the set identifier");

  /*
   * 6)
   */

  if (set_destroy(o->id) != STATUS_OK)
    CORE_ESCAPE("unable to destroy the set descriptor");

  CORE_LEAVE();
}

/*
 * this function just returns an error because the bpt set only
 * works with the sort option.
 */

t_status		set_push_bpt(i_set			setid,
				     void*			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error because the bpt set only
 * works with the sort option.
 */

t_status		set_pop_bpt(i_set			setid)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error because the bpt set only
 * works with the sort option.
 */

t_status		set_pick_bpt(i_set			setid,
				     void**			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}
