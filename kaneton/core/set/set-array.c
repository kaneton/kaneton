/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/set/set-array.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       julien quintard   [fri apr  8 09:54:31 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this set implementation provides array data structures.
 *
 * there are two way to use the arrays: with pointers or with objects. the
 * first method is the default one (without SET_OPTION_ALLOCATE); it only
 * stores pointers refering to objects. The second method (used with
 * SET_OPTION_ALLOCATE) makes copies of objects in the set, using memcpy().
 *
 * if the FREE option is activated, objects are freed when they are
 * removed from the array or when the array is flushed/released.
 *
 * the option ORGANISE is used to keep the array as small as possible
 * by regularly shifting the elements in order to avoid "holes" in the
 * entries.
 *
 * options:
 *   SET_OPTION_CONTAINER
 *   SET_OPTION_SORT
 *   SET_OPTION_ORGANISE
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
 * the manager's structure.
 */

extern m_set		_set;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function tries to find an object with its identifier and returns
 * true or false.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the descriptor.
 * 2) search sequentially for the element.
 */

t_bool			set_exist_array(i_set			setid,
					t_id			id)
{
  o_set*       		o;
  i_set			i;

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

  for (i = 0; i < o->u.array.arraysz; ++i)
    {
      if (o->u.array.array[i] &&
	  *((t_id*)(o->u.array.array[i])) == id)
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
 * 3) display information on the set.
 * 4) go through the set's objects.
 *   a) retrieve the object's identifier.
 *   b) warn if an unused identifier has been found though this entry
 *      is not marked as available.
 *   c) display the identifier and move on to the next object.
 */

t_status		set_show_array(i_set			setid,
				       mt_margin		margin)
{
  o_set*		o;
  t_setsz		i;
  t_setsz		pos;
  t_id			id;
  char			options[6];

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

  if (o->options & SET_OPTION_ORGANISE)
    options[2] = 'o';
  else
    options[2] = '.';

  if (o->options & SET_OPTION_ALLOCATE)
    options[3] = 'a';
  else
    options[3] = '.';

  if (o->options & SET_OPTION_FREE)
    options[4] = 'f';
  else
    options[4] = '.';

  options[5] = '\0';

  /*
   * 3)
   */

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "set: id(%qd) type(array) size(%qd) datasz(%u) options(%s) "
	      "initsz(%u) array(0x%x) arraysz(%qd)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      o->id,
	      o->size,
	      o->datasz,
	      options,
	      o->u.array.initsz,
	      o->u.array.array,
	      o->u.array.arraysz);

  /*
   * 4)
   */

  for (i = 0, pos = 0; i < o->u.array.arraysz; ++i)
    {
      /*
       * a)
       */

      if (o->u.array.array[i])
	id = *((t_id*)(o->u.array.array[i]));
      else
	continue;

      /*
       * b)
       */

      if (id == ID_UNUSED)
	module_call(console, message,
		    '!', "warning: unused object detected !\n");

      /*
       * c)
       */

      module_call(console, message,
		  '#',
		  MODULE_CONSOLE_MARGIN_FORMAT
		  "  object: id(%qd) slot(%qd)\n",
		  MODULE_CONSOLE_MARGIN_VALUE(margin),
		  id,
		  pos);

      ++pos;
    }

  CORE_LEAVE();
}

/*
 * this function returns an iterator on the first entry of the array.
 *
 * if there is no element in the array, the function returns false.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) if the array is empty, return false.
 * 3) go through the elements, looking for the first used entry.
 * 4) set the iterator.
 */

t_bool			set_head_array(i_set			setid,
				       s_iterator*		iterator)
{
  o_set*		o;
  t_setsz		i;

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

  for (i = 0; i < o->u.array.arraysz; ++i)
    {
      if (o->u.array.array[i])
	break;
    }

  /*
   * 4)
   */

  iterator->u.array.i = i;

  CORE_TRUE();
}

/*
 * this function returns an iterator on the last entry of the array.
 *
 * if there is no node in the list, the function returns false.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) if the array is empty, return false.
 * 3) go through the elements, looking for the last used entry.
 * 4) set the iterator.
 */

t_bool			set_tail_array(i_set			setid,
				       s_iterator*		iterator)
{
  o_set*		o;
  t_setsz		i;

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

  for (i = o->u.array.arraysz - 1; i >= 0; --i)
    {
      if (o->u.array.array[i])
	break;
    }

  /*
   * 4)
   */

  iterator->u.array.i = i;

  CORE_TRUE();
}

/*
 * this function returns an iterator on the entry previous to the given
 * iterator.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) go through the elements, looking for the previous used entry.
 * 3) return false if no entry was found.
 * 4) set the iterator.
 */

t_bool			set_previous_array(i_set		setid,
					   s_iterator		current,
					   s_iterator*		previous)
{
  o_set*		o;
  t_setsz		i;

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

  for (i = current.u.array.i - 1; i >= 0; --i)
    {
      if (o->u.array.array[i])
	break;
    }

  /*
   * 3)
   */

  if (i == -1)
    CORE_FALSE();

  /*
   * 4)
   */

  previous->u.array.i = i;

  CORE_TRUE();
}

/*
 * this function returns an iterator on the element following the one
 * pointed by the given iterator.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) go through the elements, looking for the next used entry.
 * 3) return false if no entry was found.
 * 4) set the iterator.
 */

t_bool			set_next_array(i_set			setid,
				       s_iterator		current,
				       s_iterator*		next)
{
  o_set*		o;
  t_setsz		i;

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

  for (i = current.u.array.i + 1; i < o->u.array.arraysz; ++i)
    {
      if (o->u.array.array[i])
	break;
    }

  /*
   * 3)
   */

  if (i >= o->u.array.arraysz)
    CORE_FALSE();

  /*
   * 4)
   */

  next->u.array.i = i;

  CORE_TRUE();
}

/*
 * this function is used when no room is found in the array. it expands
 * the array and copies data, if required.
 *
 * note that the given data is inserted in the first new slot.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) initialize the index with the set's current size.
 * 2) computes the new size according to the options.
 * 3) expands the array and set the new size.
 * 4) insert the data in the new slot i.e at the saved index i.e the end
 *    of previous array.
 * 5) initialize the new entries as available i.e unused.
 */

t_status		set_expand_array(o_set			*object,
					 void			*data)
{
  t_setsz		i;
  t_setsz		sz;

  /*
   * 0)
   */

  if (object == NULL)
    CORE_ESCAPE("the 'object' argument is null");

  /*
   * 1)
   */

  i = object->u.array.arraysz;

  /*
   * 2)
   */

  if (object->options & SET_OPTION_ORGANISE)
    sz = object->u.array.arraysz + 1;
  else
    sz = object->u.array.arraysz * 2;

  /*
   * 3)
   */

  if ((object->u.array.array = realloc(object->u.array.array,
				       sz * sizeof (void*))) == NULL)
    CORE_ESCAPE("unable to reallocate memory for the array");

  object->u.array.arraysz = sz;

  /*
   * 4)
   */

  object->u.array.array[i] = data;

  /*
   * 5)
   */

  for (++i; i < object->u.array.arraysz; ++i)
    object->u.array.array[i] = NULL;

  CORE_LEAVE();
}

/*
 * this function inserts an element at the specified position. this
 * function is used by many others but is not part of the interface.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) if the position is at the end, expand the array and insert the
 *    data.
 * 2) if the entry is available, insert the data and exit.
 * 3) otherwise, start at the position and walk through the entries trying
 *    to find a free slot.
 * 4) if none has been found, expand the array in order to create free slots.
 * 5) one found, walk back and shift the elements to the right so that
 *    the entry at the requested position becomes available.
 * 6) insert the data at the desired position.
 */

t_status		set_insert_array_at(o_set		*object,
					    t_setsz		pos,
					    void		*data)
{
  t_setsz		limit;

  /*
   * 0)
   */

  if (object == NULL)
    CORE_ESCAPE("the 'object' argument is null");

  /*
   * 1)
   */

  if (object->u.array.arraysz == pos)
    {
      if (set_expand_array(object, data) != STATUS_OK)
	CORE_ESCAPE("unable to expand the array");

      CORE_LEAVE();
    }

  /*
   * 2)
   */

  if (object->u.array.array[pos] == NULL)
    {
      object->u.array.array[pos] = data;

      CORE_LEAVE();
    }

  /*
   * 3)
   */

  for (limit = pos; limit < object->u.array.arraysz; ++limit)
    {
      if (object->u.array.array[limit] == NULL)
	break;
    }

  /*
   * 4)
   */

  if (limit == object->u.array.arraysz)
    {
      if (set_expand_array(object,
			   object->u.array.array[limit - 1]) != STATUS_OK)
	CORE_ESCAPE("unable to expand the array");
    }

  /*
   * 5)
   */

  for (; limit > pos; --limit)
    {
      object->u.array.array[limit] = object->u.array.array[limit - 1];
    }

  /*
   * 6)
   */

  object->u.array.array[pos] = data;

  CORE_LEAVE();
}

/*
 * this function inserts a new entry at the head of the array.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) check if this operation can be performed in this set.
 * 3) if the ALLOCATE option has been activated, clone the object.
 * 4) insert the element at the head.
 * 5) update the size of the set.
 */

t_status		set_insert_array(i_set			setid,
					 void*			data)
{
  o_set*		o;
  void*			cpy;

  /*
   * 0)
   */

  if (data == NULL)
    CORE_ESCAPE("the 'data' argument is null");

  if (*((t_id*)data) == ID_UNUSED)
    CORE_ESCAPE("the object to insert must begin with a valid identifier");

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  if (o->options & SET_OPTION_SORT)
    CORE_ESCAPE("this operation is not allowed for sorted sets");

  /*
   * 3)
   */

  if (o->options & SET_OPTION_ALLOCATE)
    {
      if ((cpy = malloc(o->datasz)) == NULL)
	CORE_ESCAPE("unable to allocate memory for the object's copy");

      memcpy(cpy, data, o->datasz);

      data = cpy;
    }

  /*
   * 4)
   */

  if (set_insert_array_at(o, 0, data) != STATUS_OK)
    {
      if (o->options & SET_OPTION_ALLOCATE)
	free(data);

      CORE_ESCAPE("unable to insert the object in the set");
    }

  /*
   * 5)
   */

  ++o->size;

  CORE_LEAVE();
}

/*
 * this function inserts a new entry at the tail of the array.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) check if this operation can be performed in this set.
 * 3) if the ALLOCATE option has been activated, clone the object.
 * 4) locate the last available slot.
 * 5) insert the data at this specific location.
 * 6) update the size of the set.
 */

t_status		set_append_array(i_set			setid,
					 void*			data)
{
  o_set*		o;
  t_setsz		i;
  void*			cpy;

  /*
   * 0)
   */

  if (data == NULL)
    CORE_ESCAPE("the 'data' argument is null");

  if (*((t_id*)data) == ID_UNUSED)
    CORE_ESCAPE("the object to append must begin with a valid identifier");

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

  if (o->options & SET_OPTION_ALLOCATE)
    {
      if (!(cpy = malloc(o->datasz)))
	CORE_ESCAPE("unable to allocate memory for the object's copy");

      memcpy(cpy, data, o->datasz);

      data = cpy;
    }

  /*
   * 4)
   */

  if (o->size > 0)
    {
      for (i = o->u.array.arraysz; i > 0; --i)
	{
	  if (o->u.array.array[i - 1])
	    break;
	}
    }
  else
    i = 0;

  /*
   * 5)
   */

  if (set_insert_array_at(o, i, data) != STATUS_OK)
    {
      if (o->options & SET_OPTION_ALLOCATE)
	free(data);

      CORE_ESCAPE("unable to insert the object in the set");
    }

  /*
   * 6)
   */

  ++o->size;

  CORE_LEAVE();
}

/*
 * this function inserts an object before the one referenced by the iterator.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) check if this operation can be performed in this set.
 * 3) if the ALLOCATE option has been activated, clone the object.
 * 4) insert the object at the location pointed to by the iterator, hence
 *    shifting the following entries to the right.
 * 5) update the size of the set.
 */

t_status		set_before_array(i_set			setid,
					 s_iterator		iterator,
					 void*			data)
{
  o_set*		o;
  t_setsz		i;
  void*			cpy;

  /*
   * 0)
   */

  if (data == NULL)
    CORE_ESCAPE("the 'data' argument is null");

  if (*((t_id*)data) == ID_UNUSED)
    CORE_ESCAPE("the object to insert must begin with a valid identifier");

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  if (o->options & SET_OPTION_SORT)
    CORE_ESCAPE("unable to insert at a specific location in a sorted set");

  /*
   * 3)
   */

  if (o->options & SET_OPTION_ALLOCATE)
    {
      if (!(cpy = malloc(o->datasz)))
	CORE_ESCAPE("unable to allocate memory for the object's copy");

      memcpy(cpy, data, o->datasz);

      data = cpy;
    }

  /*
   * 4)
   */

  i = iterator.u.array.i;

  if (i < 0)
    i = 0;

  if (set_insert_array_at(o, i, data) != STATUS_OK)
    {
      if (o->options & SET_OPTION_ALLOCATE)
	free(data);

      CORE_ESCAPE("unable to insert the object in the set");
    }

  /*
   * 5)
   */

  ++o->size;

  CORE_LEAVE();
}

/*
 * this function inserts a new object after the one referenced by the iterator.
 *
 * steps:
 *
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) check if this operation can be performed in this set.
 * 3) if the ALLOCATE option has been activated, clone the object.
 * 4) insert the object at the location following the one pointed
 *    to by the iterator.
 * 5) update the size of the set.
 */

t_status		set_after_array(i_set			setid,
					s_iterator		iterator,
					void*			data)
{
  o_set*       		o;
  t_setsz		i;
  void*			cpy;

  /*
   * 0)
   */

  if (data == NULL)
    CORE_ESCAPE("the 'data' argument is null");

  if (*((t_id*)data) == ID_UNUSED)
    CORE_ESCAPE("the object to insert must begin with a valid identifier");

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  if (o->options & SET_OPTION_SORT)
    CORE_ESCAPE("unable to insert at a precise position in a sorted set");

  /*
   * 3)
   */

  if (o->options & SET_OPTION_ALLOCATE)
    {
      if (!(cpy = malloc(o->datasz)))
	CORE_ESCAPE("unable to allocate memory for the object's copy");

      memcpy(cpy, data, o->datasz);

      data = cpy;
    }

  /*
   * 4)
   */

  i = iterator.u.array.i + 1;

  if (i > o->u.array.arraysz)
    i = o->u.array.arraysz;

  if (set_insert_array_at(o, i, data) != STATUS_OK)
    {
      if (o->options & SET_OPTION_ALLOCATE)
	free(data);

      CORE_ESCAPE("unable to insert the object in the set");
    }

  /*
   * 5)
   */

  ++o->size;

  CORE_LEAVE();
}

/*
 * this function adds an object to the array.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) clone the object if required.
 * 3) inserts the element depending on the options.
 *   A) if the SORT option has been activated...
 *    i) find the place of the new element.
 *    ii) copy in the right place.
 *    iii) expand the array if necessary.
 *   B) otherwise...
 *    i) try to find a free place.
 *    ii) expand the array if necessary.
 * 4) update the set's size.
 */

t_status		set_add_array(i_set			setid,
				      void*			data)
{
  o_set*		o;
  t_setsz		i;
  t_id			id;
  t_id			current;
  void*			cpy;
  t_uint8		empty;
  t_setsz		last;

  /*
   * 0)
   */

  if (data == NULL)
    CORE_ESCAPE("the 'data' argument is null");

  if (*((t_id*)data) == ID_UNUSED)
    CORE_ESCAPE("the object to add must begin with a valid identifier");

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  if (o->options & SET_OPTION_ALLOCATE)
    {
      if (!(cpy = malloc(o->datasz)))
	CORE_ESCAPE("unable to allocate memory for the object's copy");

      memcpy(cpy, data, o->datasz);

      data = cpy;
    }

  /*
   * 3)
   */

  if (o->options & SET_OPTION_SORT)
    {
      /*
       * A)
       */

      /*
       * i)
       */

      empty = 1;
      last = -1;

      id = *((t_id*)data);

      for (i = 0; i < o->u.array.arraysz; ++i)
	{
	  if (o->u.array.array[i] != NULL)
	    {
	      current = *((t_id*)o->u.array.array[i]);

	      empty = 0;

	      if (current == id)
		{
		  if (o->options & SET_OPTION_ALLOCATE)
		    free(data);

		  CORE_ESCAPE("identifier collision detected in the set "
			      "%qd on the object identifier %qd",
			      o->id, id);
		}

	      if (current > id)
		break;

	      last = i;
	    }
	}

      /*
       * ii)
       */

      ++last;

      if (empty)
	last = i = o->u.array.arraysz / 2;

      if (last < o->u.array.arraysz)
	{
	  if (set_insert_array_at(o, last, data) != STATUS_OK)
	    {
	      if (o->options & SET_OPTION_ALLOCATE)
		free(data);

	      CORE_ESCAPE("unable to insert the object in the set");
	    }
	}

      /*
       * iii)
       */

      if (last >= o->u.array.arraysz)
	{
	  if (set_expand_array(o, data) != STATUS_OK)
	    {
	      if (o->options & SET_OPTION_ALLOCATE)
		free(data);

	      CORE_ESCAPE("unable to expand the array");
	    }
	}
    }
  else
    {
      /*
       * B)
       */

      /*
       * i)
       */

      for (i = 0; i < o->u.array.arraysz; ++i)
	{
	  if (o->u.array.array[i] == NULL)
	    {
	      o->u.array.array[i] = data;

	      break;
	    }
	}

      /*
       * ii)
       */

      if (i == o->u.array.arraysz)
	{
	  if (set_expand_array(o, data) != STATUS_OK)
	    {
	      if (o->options & SET_OPTION_ALLOCATE)
		free(data);

	      CORE_ESCAPE("unable to expand the array");
	    }
	}
    }

  /*
   * 4)
   */

  ++o->size;

  CORE_LEAVE();
}

/*
 * this function removes an object from the array, setting the identifier
 * field of the entry as 'unused'.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) if there is no object, return an error.
 * 3) try to locate the object according to its identifier.
 * 4) if not found, return an error.
 * 5) if the ORGANIZE option has been activated, shift the elements to
 *    the left and re-allocate the array with the necessary memory.
 * 6) update the set's size.
 */

t_status		set_remove_array(i_set			setid,
					 t_id			id)
{
  o_set*       		o;
  t_setsz		i;
  t_id			current;

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

  if (o->size == 0)
    CORE_ESCAPE("the set is empty");

  /*
   * 3)
   */

  for (i = 0; i < o->u.array.arraysz; ++i)
    {
      if (o->u.array.array[i])
	{
	  current = *((t_id*)(o->u.array.array[i]));

	  if ((o->options & SET_OPTION_SORT) && current > id)
	    CORE_ESCAPE("unable to locate the given object");

	  if (current == id)
	    {
	      if ((o->options & SET_OPTION_FREE) ||
		  (o->options & SET_OPTION_ALLOCATE))
		free(o->u.array.array[i]);

	      o->u.array.array[i] = NULL;

	      break;
	    }
	}
    }

  /*
   * 4)
   */

  if (i == o->u.array.arraysz)
    CORE_ESCAPE("unable to locate the given object");

  /*
   * 5)
   */

  if (o->options & SET_OPTION_ORGANISE)
    {
      for (; i < o->u.array.arraysz - 1; ++i)
	{
	  o->u.array.array[i] = o->u.array.array[i + 1];
	}

      --o->u.array.arraysz;

      if ((o->u.array.array = realloc(o->u.array.array,
				      o->u.array.arraysz *
				      sizeof (void*))) == NULL)
	CORE_ESCAPE("unable to reallocate memory for the array");
    }

  /*
   * 6)
   */

  --o->size;

  CORE_LEAVE();
}

/*
 * this function deletes an element given an iterator.
 *
 * steps:
 *
 * 1) retrieve the set descriptor.
 * 2) save the iterator's index.
 * 3) if the iterator is out of bound, return an error.
 * 4) if the slot is unused, return an error.
 * 5) if required, free the object and re-initialize its slot
 *    as being available.
 * 6) if required, re-organise the array.
 * 7) update the set's size.
 */

t_status		set_delete_array(i_set			setid,
					 s_iterator		iterator)
{
  o_set*		o;
  t_setsz		i;

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  i = iterator.u.array.i;

  /*
   * 3)
   */

  if (i >= o->u.array.arraysz)
    CORE_ESCAPE("the given iterator is out of bound");

  /*
   * 4)
   */

  if (o->u.array.array[i] == NULL)
    CORE_ESCAPE("the object the iterator points to does not exist");

  /*
   * 5)
   */

  if (o->options & SET_OPTION_ALLOCATE ||
      o->options & SET_OPTION_FREE)
    free(o->u.array.array[i]);

  o->u.array.array[i] = NULL;

  /*
   * 6)
   */

  if (o->options & SET_OPTION_ORGANISE)
    {
      for (; i < o->u.array.arraysz - 1; ++i)
	{
	  o->u.array.array[i] = o->u.array.array[i + 1];
	}

      --o->u.array.arraysz;

      if ((o->u.array.array = realloc(o->u.array.array,
				      o->u.array.arraysz *
				      sizeof (void*))) == NULL)
	CORE_ESCAPE("unable to reallocate memory for the array");
    }

  /*
   * 7)
   */

  o->size--;

  CORE_LEAVE();
}

/*
 * this function flushes the set, releasing every element.
 *
 * steps:
 *
 * 1) retrieve the set descriptor.
 * 2) if required, free the memory of every object.
 * 3) reset the array to its original size and mark every slot as
 *    being unused.
 * 4) set the size of the set to zero.
 */

t_status		set_flush_array(i_set			setid)
{
  o_set*       		o;
  t_setsz		i;

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  if ((o->options & SET_OPTION_FREE) ||
      (o->options & SET_OPTION_ALLOCATE))
    {
      for (i = 0; i < o->u.array.arraysz; ++i)
	{
	  if (o->u.array.array[i])
	    {
	      free(o->u.array.array[i]);

	      o->u.array.array[i] = NULL;
	    }
	}

      o->size = 0;
    }

  /*
   * 3)
   */

  if ((o->u.array.array = realloc(o->u.array.array,
				  o->u.array.initsz * sizeof (void*))) == NULL)
    CORE_ESCAPE("unable to reallocate memory for the array");

  for (i = 0; i < o->u.array.initsz; ++i)
    {
      o->u.array.array[i] = NULL;
    }

  o->u.array.arraysz = o->u.array.initsz;

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
 * 2) go through the elements and locate the slot.
 * 3) if not found, return an error.
 */

t_status		set_locate_array(i_set			setid,
					 t_id			id,
					 s_iterator*		iterator)
{
  o_set*       		o;
  i_set			i;

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

  for (i = 0; i < o->u.array.arraysz; ++i)
    {
      if (o->u.array.array[i] &&
	  *((t_id*)(o->u.array.array[i])) == id)
	{
	  iterator->u.array.i = i;

	  CORE_LEAVE();
	}
    }

  /*
   * 3)
   */

  CORE_ESCAPE("unable to locate the given identifier in the set");
}

/*
 * this function returns an object given its iterator.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) if the iterator is out of bound, return an error.
 * 3) if the slot the iterator points to is unused, return an error.
 * 4) return the object.
 */

t_status		set_object_array(i_set			setid,
					 s_iterator		iterator,
					 void**			data)
{
  o_set*       		o;

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

  if (iterator.u.array.i >= o->u.array.arraysz)
    CORE_ESCAPE("the given iterator is out of bound");

  /*
   * 3)
   */

  if (o->u.array.array[iterator.u.array.i] == NULL)
    CORE_ESCAPE("the object the iterator points to does not exist");

  /*
   * 4)
   */

  *data = o->u.array.array[iterator.u.array.i];

  CORE_LEAVE();
}

/*
 * this function reserves a set according to several options, the initial
 * number of elements in the array and the size of every element.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) assign an identifier, depending on the options: either this is
 *    the set container or not. if it is, take the set container identifier
 *    that is in the set manager.
 * 2) initialize and fill the set descriptor.
 * 3) allocate and initialize the array data structure.
 * 4) register the set descriptor.
 */

t_status		set_reserve_array(t_options		options,
					  t_setsz		initsz,
					  t_size		datasz,
					  i_set*		id)
{
  o_set			o;
  t_setsz		i;

  /*
   * 0)
   */

  if (datasz < sizeof (t_id))
    CORE_ESCAPE("unable to reserve a set for objects smaller than "
		"an identifier");

  if (id == NULL)
    CORE_ESCAPE("the 'id' argument is null");

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
  o.type = SET_TYPE_ARRAY;
  o.options = options;
  o.datasz = datasz;

  o.u.array.arraysz = (initsz == 0 ? 1 : initsz);
  o.u.array.initsz = o.u.array.arraysz;

  /*
   * 3)
   */

  if ((o.u.array.array = malloc(o.u.array.arraysz * sizeof (void*))) == NULL)
    {
      if (!(options & SET_OPTION_CONTAINER))
	id_release(&_set.id, o.id);

      CORE_ESCAPE("unable to allocate memory for the array");
    }

  for (i = 0; i < o.u.array.arraysz; i++)
    o.u.array.array[i] = NULL;

  /*
   * 4)
   */

  if (set_new(&o) != STATUS_OK)
    {
      free(o.u.array.array);

      if (!(options & SET_OPTION_CONTAINER))
	id_release(&_set.id, o.id);

      CORE_ESCAPE("unable to register the new set descriptor");
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
 * 4) free the array data structure's memory.
 * 5) remove the set descriptor from the set container.
 */

t_status		set_release_array(i_set			setid)
{
  o_set*       		o;

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
    CORE_ESCAPE("unable to release the set's identifier");

  /*
   * 4)
   */

  free(o->u.array.array);

  /*
   * 5)
   */

  if (set_destroy(o->id) != STATUS_OK)
    CORE_ESCAPE("unable to destroy the set descriptor");

  CORE_LEAVE();
}

/*
 * this function just returns an error because the array set does not
 * support this operation.
 */

t_status		set_push_array(i_set			setid,
				       void*			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error because the array set does not
 * support this operation.
 */

t_status		set_pop_array(i_set			setid)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error because the array set does not
 * support this operation.
 */

t_status		set_pick_array(i_set			setid,
				       void**			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}
