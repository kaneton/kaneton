/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * set_bpt.c
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Fri Sep  9 04:34:23 2005   mycure
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX
 *
 * XXX SET_OPT_ALLOC
 *
 * XXX datasz juste utilise pour OPT_ALLOC
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

bpt_make_functions(set, id, data);

/*
 * this function loads a node from main memory to main memory.
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
 * using node storing in main memory, this function finally does nothing.
 */

void			set_unload_bpt(t_bpt(set)*		bpt,
				       t_bpt_imm(set)*		node)
{
  /*
   * nothing to do here because we are in main memory
   */
}

/*
 * this function compares two addresses.
 */

int			set_addrcmp_bpt(t_bpt(set)*		bpt,
					t_bpt_addr(set)		addr1,
					t_bpt_addr(set)		addr2)
{
  return (addr1 - addr2);
}

/*
 * this function compares two keys.
 */

int			set_keycmp_bpt(t_bpt(set)*		bpt,
				       t_bpt_key(set)		key1,
				       t_bpt_key(set)		key2)
{
  return (key1 - key2);
}

/*
 * this function compares two values.
 */

int			set_valcmp_bpt(t_bpt(set)*		bpt,
				       t_bpt_value(set)		value1,
				       t_bpt_value(set)		value2)
{
  return (value1 - value2);
}

/*
 * this function dumps the state of the unused object, in other
 * words the unused nodes contained in the unused object.
 */

t_error			set_dump_unused_bpt(o_set*		o)
{
  t_bpt_uni(set)	i;

  SET_ENTER(set);

  cons_msg('#', "dumping the unused nodes: %u / %u\n",
	   o->u.bpt.unused.index + 1,
	   o->u.bpt.unusedsz);

  for (i = 0; i <= o->u.bpt.unused.index; i++)
    cons_msg('#', "  [%d] 0x%x\n", i, o->u.bpt.unused.array[i]);

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function builds an unused object.
 *
 * steps:
 *
 * 1) allocates and initialises the unused object.
 * 3) builds the needed number of elements.
 */

t_error			set_build_bpt(o_set*			o,
				      BPT_NODESZ_T		nodesz)
{
  t_bpt_uni(set)	i;

  SET_ENTER(set);

  o->u.bpt.unusedsz = BPT_INIT_SIZE();

  /*
   * 1)
   */

  if ((o->u.bpt.unused.array = malloc(o->u.bpt.unusedsz *
				      sizeof(t_bpt_addr(set)))) == NULL)
    SET_LEAVE(set, ERROR_UNKNOWN);

  o->u.bpt.unused.index = -1;

  /*
   * 2)
   */

  if (set_adjust_bpt(o, BPT_INIT_ALLOC(), BPT_INIT_SIZE()) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * XXX
 *
 * XXX le faire mieux, on peut tres bien envisager dans cette fonction
 *     de liberer de l espace plutot que d agrandir size
 */

t_error			set_adjust_bpt(o_set*			o,
				       t_bpt_uni(set)		alloc,
				       t_bpt_uni(set)		size)
{
  t_bpt_uni(set)	i;

  SET_ENTER(set);

  if (o->u.bpt.unusedsz < size)
    {
      if ((o->u.bpt.unused.array =
	   realloc(o->u.bpt.unused.array, size)) == NULL)
	SET_LEAVE(set, ERROR_UNKNOWN);

      o->u.bpt.unusedsz = size;
    }

  for (; (o->u.bpt.unused.index + 1) < alloc; o->u.bpt.unused.index++)
    {
      if ((o->u.bpt.unused.array[(o->u.bpt.unused.index + 1)] =
	   (t_bpt_addr(set))malloc(o->u.bpt.bpt.nodesz)) == NULL)
	SET_LEAVE(set, ERROR_UNKNOWN);
    }

  if ((o->u.bpt.unusedsz - (o->u.bpt.unused.index + 1)) < (size - alloc))
    {
      o->u.bpt.unusedsz += (size - alloc) -
	(o->u.bpt.unusedsz - (o->u.bpt.unused.index + 1));

      if ((o->u.bpt.unused.array =
	   realloc(o->u.bpt.unused.array, o->u.bpt.unusedsz)) == NULL)
	SET_LEAVE(set, ERROR_UNKNOWN);
    }

  SET_LEAVE(set, ERROR_NONE);
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

  SET_ENTER(set);

  /*
   * 1)
   */

  for (i = 0; i <= o->u.bpt.unused.index; i++)
    free(o->u.bpt.unused.array[i]);

  /*
   * 2)
   */

  free(o->u.bpt.unused.array);

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function dumps the set objects.
 *
 * steps:
 *
 * 1) gets the descriptor from its set identifier.
 * 2) prints a message for each objects of the set.
 */

t_error			set_dump_bpt(t_setid			setid)
{
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

  cons_msg('#', "dumping %qd node(s) from the balanced+ tree set %qu:\n",
	   o->size,
	   setid);

  set_foreach(SET_OPT_FORWARD, setid, &i)
    {
      t_bpt_imm(set)		node;
      t_bpt_lfentry(set)*	leaf;

      BPT_LOAD(&o->u.bpt.bpt, &node, i.u.bpt.entry.node);

      leaf = BPT_LFENTRY(set, &node, i.u.bpt.entry.ndi);

      cons_msg('#', "  %qu <%qu 0x%x>\n",
	       leaf->id, *((t_id*)leaf->data), leaf->data);
    }

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function gets the first object of the set using the bpt_list()
 * function with the BPT_OPT_HEAD option.
 */

t_error			set_head_bpt(t_setid			setid,
				     t_iterator*		iterator)
{
  t_bpt_imm(set)	root;
  o_set*		o;

  SET_ENTER(set);

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  BPT_LOAD(&o->u.bpt.bpt, &root, o->u.bpt.bpt.root);

  if (bpt_list(set, &o->u.bpt.bpt, &root,
	       &iterator->u.bpt.entry, BPT_OPT_HEAD) != 0)
    {
      BPT_UNLOAD(&o->u.bpt.bpt, &root);

      SET_LEAVE(set, ERROR_UNKNOWN);
    }

  BPT_UNLOAD(&o->u.bpt.bpt, &root);

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function gets the last object of the set using the bpt_list()
 * function with the BPT_OPT_TAIL option.
 */

t_error			set_tail_bpt(t_setid			setid,
				     t_iterator*		iterator)
{
  t_bpt_imm(set)	root;
  o_set*		o;

  SET_ENTER(set);

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  BPT_LOAD(&o->u.bpt.bpt, &root, o->u.bpt.bpt.root);

  if (bpt_list(set, &o->u.bpt.bpt, &root,
	       &iterator->u.bpt.entry, BPT_OPT_TAIL) != 0)
    {
      BPT_UNLOAD(&o->u.bpt.bpt, &root);

      SET_LEAVE(set, ERROR_UNKNOWN);
    }

  BPT_UNLOAD(&o->u.bpt.bpt, &root);

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function returns the previous node of the current iterator.
 */

t_error			set_prev_bpt(t_setid			setid,
				     t_iterator			current,
				     t_iterator*		previous)
{
  o_set*		o;

  SET_ENTER(set);

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  if (bpt_prev_entry(set, &o->u.bpt.bpt, current.u.bpt.entry,
		     &previous->u.bpt.entry, BPT_OPT_TREE) != 0)
    SET_LEAVE(set, ERROR_UNKNOWN);

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function returns the next node of the current iterator.
 */

t_error			set_next_bpt(t_setid			setid,
				     t_iterator			current,
				     t_iterator*		next)
{
  o_set*		o;

  SET_ENTER(set);

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  if (bpt_next_entry(set, &o->u.bpt.bpt, current.u.bpt.entry,
		     &next->u.bpt.entry, BPT_OPT_TREE) != 0)
    SET_LEAVE(set, ERROR_UNKNOWN);

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function just returns an error because the bpt set only
 * works with the sort option.
 */

t_error			set_insert_head_bpt(t_setid		setid,
					    void*		data)
{
  SET_ENTER(set);

  SET_LEAVE(set, ERROR_UNKNOWN);
}

/*
 * this function just returns an error because the bpt set only
 * works with the sort option.
 */

t_error			set_insert_tail_bpt(t_setid		setid,
					    void*		data)
{
  SET_ENTER(set);

  SET_LEAVE(set, ERROR_UNKNOWN);
}

/*
 * this function just returns an error because the bpt set only
 * works with the sort option.
 */

t_error			set_insert_before_bpt(t_setid		setid,
					      t_iterator	iterator,
					      void*		data)
{
  SET_ENTER(set);

  SET_LEAVE(set, ERROR_UNKNOWN);
}

/*
 * this function just returns an error because the bpt set only
 * works with the sort option.
 */

t_error			set_insert_after_bpt(t_setid		setid,
					     t_iterator		iterator,
					     void*		data)
{
  SET_ENTER(set);

  SET_LEAVE(set, ERROR_UNKNOWN);
}

/*
 * this function builds a leaf entry to add the user object.
 *
 * steps:
 *
 * 1) checks for non-valid identifiers.
 * 2) gets the set descriptor object from its identifier.
 * 3) initialises the leaf entry.
 * 4) handles the options: allocate or not the object.
 * 5) adjusts the bpt unused object to perform the next operation.
 * 6) adds the built leaf entry to the tree.
 * 7) updates the set counter.
 */

t_error			set_add_bpt(t_setid			setid,
				    void*			data)
{
  t_bpt_lfentry(set)	lfentry;
  o_set*		o;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (*(t_id*)data == ID_UNUSED)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  memset(&lfentry, 0x0, sizeof(t_bpt_lfentry(set)));

  lfentry.id = *((t_id*)data);

  /*
   * 4)
   */

  if (o->u.bpt.opts & SET_OPT_ALLOC)
    {
      if ((lfentry.data = malloc(o->u.bpt.datasz)) == NULL)
	SET_LEAVE(set, ERROR_UNKNOWN);

      memcpy(lfentry.data, data, o->u.bpt.datasz);
    }
  else
    {
      lfentry.data = data;
    }

  /*
   * 5)
   */

  if (set_adjust_bpt(o, BPT_ADD_ALLOC(&o->u.bpt.bpt),
		     BPT_ADD_SIZE(&o->u.bpt.bpt)) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 6)
   */

  if (bpt_add(set, &o->u.bpt.bpt, &lfentry, &o->u.bpt.unused) != 0)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 7)
   */

  o->size++;

  SET_LEAVE(set, ERROR_NONE);
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

t_error			set_remove_bpt(t_setid			setid,
				       t_id			id)
{
  t_bpt_entry(set)	entry;
  t_bpt_imm(set)	node;
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

  if (o->u.bpt.opts & SET_OPT_ALLOC)
    {
      if (bpt_search(set, &o->u.bpt.bpt, id, &entry) != 0)
	SET_LEAVE(set, ERROR_UNKNOWN);

      BPT_LOAD(&o->u.bpt.bpt, &node, entry.node);

      free(BPT_GET_LFENTRY(set, &node, entry.ndi, data));

      BPT_UNLOAD(&o->u.bpt.bpt, &node);
    }

  /*
   * 4)
   */

  if (set_adjust_bpt(o, BPT_REMOVE_ALLOC(&o->u.bpt.bpt),
		     BPT_REMOVE_SIZE(&o->u.bpt.bpt)) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 5)
   */

  if (bpt_remove(set, &o->u.bpt.bpt, id, &o->u.bpt.unused) != 0)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 6)
   */

  o->size--;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * XXX
 */

t_error			set_flush_bpt(t_setid			setid)
{
  t_bpt_nodesz(set)	nodesz;
  t_iterator		i;
  o_set*		o;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  nodesz = o->u.bpt.bpt.nodesz;

  /*
   * 2)
   */

  set_foreach(SET_OPT_FORWARD, setid, &i)
    {
      t_bpt_imm(set)		node;
      t_bpt_lfentry(set)*	leaf;

      BPT_LOAD(&o->u.bpt.bpt, &node, i.u.bpt.entry.node);

      leaf = BPT_LFENTRY(set, &node, i.u.bpt.entry.ndi);

      free(leaf->data);
      leaf->data = SET_BPT_UADDR;
    }

  /*
   * 3)
   */

  if (set_adjust_bpt(o, BPT_CLEAN_ALLOC(&o->u.bpt.bpt),
		     BPT_CLEAN_SIZE(&o->u.bpt.bpt)) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  if (bpt_clean(set, &o->u.bpt.bpt, &o->u.bpt.unused) != 0)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * XXX
   */

  if (set_adjust_bpt(o, BPT_INIT_ALLOC(),
		     BPT_INIT_SIZE()) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  if (bpt_init(set, &o->u.bpt.bpt, nodesz,
	       SET_BPT_UADDR, SET_BPT_UKEY, SET_BPT_UVALUE,
	       BPT_FLAG_ZERO, set_load_bpt, set_unload_bpt,
	       set_addrcmp_bpt, set_keycmp_bpt, set_valcmp_bpt,
	       NULL, NULL, &o->u.bpt.unused) != 0)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 4)
   */

  o->size = 0;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function locates the leaf entry corresponding to the given
 * identifier, then builds an iterator for it.
 */

t_error			set_locate_bpt(t_setid			setid,
				       t_id			id,
				       t_iterator*		iterator)
{
  t_bpt_entry(set)	entry;
  o_set*		o;

  SET_ENTER(set);

  if (id == ID_UNUSED)
    SET_LEAVE(set, ERROR_UNKNOWN);

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  if (bpt_search(set, &o->u.bpt.bpt, id, &entry) != 0)
    SET_LEAVE(set, ERROR_NONE);

  memcpy(iterator, &entry, sizeof(t_bpt_entry(set)));

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function returns the object from its iterator.
 */

t_error			set_object_bpt(t_setid			setid,
				       t_iterator		iterator,
				       void**			data)
{
  t_bpt_imm(set)	node;
  o_set*		o;

  SET_ENTER(set);

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  BPT_LOAD(&o->u.bpt.bpt, &node, iterator.u.bpt.entry.node);

  *data = BPT_GET_LFENTRY(set, &node, iterator.u.bpt.entry.ndi, data);

  BPT_UNLOAD(&o->u.bpt.bpt, &node);

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * XXX
 */

t_error			set_rsv_bpt(t_opts			opts,
				    t_size			datasz,
				    t_bpt_nodesz(set)		nodesz,
				    t_setid*			setid)
{
  o_set			o;

  SET_ENTER(set);

  /*
   * 1)
   */

  memset(&o, 0x0, sizeof(o_set));

  /*
   * XXX
   */

  if (!(opts & SET_OPT_SORT))
    SET_LEAVE(set, ERROR_UNKNOWN);

  if (opts & SET_OPT_ORGANISE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
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
   * 3)
   */

  o.setid = *setid;
  o.size = 0;
  o.type = SET_TYPE_BPT;

  o.u.bpt.opts = opts;
  o.u.bpt.datasz = datasz;

  /*
   * 4)
   */

  if (set_build_bpt(&o, nodesz) != ERROR_NONE)
    {
      set_destroy_bpt(&o);

      if (!(opts & SET_OPT_CONTAINER))
	id_rel(&set->id, o.setid);

      SET_LEAVE(set, ERROR_UNKNOWN);
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
      if (!(opts & SET_OPT_CONTAINER))
	id_rel(&set->id, o.setid);

      SET_LEAVE(set, ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  if (set_new(&o) != ERROR_NONE)
    {
      set_adjust_bpt(&o, BPT_CLEAN_ALLOC(&o.u.bpt.bpt),
		     BPT_CLEAN_SIZE(&o.u.bpt.bpt));

      bpt_clean(set, &o.u.bpt.bpt, &o.u.bpt.unused);

      set_destroy_bpt(&o);

      if (!(opts & SET_OPT_CONTAINER))
	id_rel(&set->id, o.setid);

      SET_LEAVE(set, ERROR_UNKNOWN);
    }

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * XXX
 */

t_error			set_rel_bpt(t_setid			setid)
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
   * XXX
   */

  if (set_adjust_bpt(o, BPT_CLEAN_ALLOC(&o->u.bpt.bpt),
		     BPT_CLEAN_SIZE(&o->u.bpt.bpt)) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  if (bpt_clean(set, &o->u.bpt.bpt, &o->u.bpt.unused) != 0)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (!(o->u.bpt.opts & SET_OPT_CONTAINER))
    if (id_rel(&set->id, o->setid) != ERROR_NONE)
      SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 5)
   */

  if (set_delete(o->setid) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  SET_LEAVE(set, ERROR_NONE);
}
