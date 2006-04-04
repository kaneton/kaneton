/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/set/set_array.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       julien quintard   [sun apr  2 19:16:47 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this  subpart  of the  set  manager is  used  to  build array  data
 * structures.
 *
 * there  are two  way  to use  the  arrays :  with  pointers or  with
 * objects.    the  first   method   is  the   default  one   (without
 * SET_OPT_ALLOC) : it only  stores pointers refering to objects.  The
 * second method (used with  SET_OPT_ALLOC) makes copies of objects in
 * the set,  using memcpy. with  SET_OPT_FREE, objects are  freed when
 * they   are  removed   from  the   array  or   when  the   array  is
 * flushed/released.
 *
 * the option ORGANISE is used to keep the array as small as possible,
 * but  some operation  requires to  shift entire  parts of  the array
 * (loss of performances).
 *
 * options:    SET_OPT_CONTAINER,    SET_OPT_SORT,   SET_OPT_ORGANISE,
 * SET_OPT_ALLOC, SET_OPT_FREE
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * the students must develop the entire array data structure.
 *
 * this data structure is very useful to contain very little objects like
 * identifiers.
 *
 * the address space objects, the task objects etc.. use it widely.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- extern ---------------------------------------------------------
 */

extern m_set*		set;

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

/*
 * this function tells if the set object is an array set.
 *
 * steps:
 *
 * 1) gets the descriptor associated with the id.
 * 2) checks the type field.
 */

t_error			set_type_array(t_setid			setid)
{
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

  if (o->type != SET_TYPE_ARRAY)
    SET_LEAVE(set, ERROR_UNKNOWN);

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function shows set objects contained in a set.
 *
 * steps:
 *
 * 1) retrieves the set associated with the setid.
 * 2) loops through the slots and displays elements.
 */

t_error			set_show_array(t_setid			setid)
{
  o_set*		o;
  t_setsz		i;
  t_setsz		pos;
  t_id			id;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  cons_msg('#', "  %qd node(s) from the array set %qu:\n",
	   o->size,
	   setid);

  for (i = 0, pos = 0; i < o->u.array.arraysz; ++i)
    {
      if (o->u.array.array[i])
	id = *((t_id*)(o->u.array.array[i]));
      else
	continue;

      if (id == ID_UNUSED)
	cons_msg('!', "warning: unused object detected !\n");

      cons_msg('#', "  array[%qd] = %qd\n", pos, id);
      ++pos;
    }

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function returns an iterator on the first entry on the array.
 * if there is no element in the array, the function returns ERROR_UNKNOWN.
 *
 * steps:
 *
 * 1) gets descriptor.
 * 2) looks for the first element of the list.
 * 3) updates the iterator.
 */

t_error			set_head_array(t_setid			setid,
				       t_iterator*		iterator)
{
  o_set*		o;
  t_setsz		i;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (o->size == 0)
    SET_LEAVE(set, ERROR_UNKNOWN);

  for (i = 0; i < o->u.array.arraysz; ++i)
    {
      if (o->u.array.array[i])
	break;
    }

  /*
   * 3)
   */

  memset(iterator, 0x0, sizeof(t_iterator));

  iterator->u.array.i = i;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function returns an iterator on the last entry of the array.
 * if there is no node in the list, the function returns ERROR_UNKNOWN.
 *
 * steps:
 *
 * 1) gets descriptor.
 * 2) looks for the last element of the list.
 * 3) updates the iterator.
 */

t_error			set_tail_array(t_setid			setid,
				       t_iterator*		iterator)
{
  o_set*		o;
  t_setsz		i;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (o->size == 0)
    SET_LEAVE(set, ERROR_UNKNOWN);

  for (i = o->u.array.arraysz - 1; i >= 0; --i)
    {
      if (o->u.array.array[i])
	break;
    }

  /*
   * 3)
   */

  memset(iterator, 0x0, sizeof(t_iterator));

  iterator->u.array.i = i;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function returns an iterator on the previous entry.
 *
 * steps:
 *
 * 1) gets the descriptor.
 * 2) moves to the previous non-empty slot.
 * 3) updates the iterator.
 */

t_error			set_previous_array(t_setid		setid,
					   t_iterator		current,
					   t_iterator*		previous)
{
  o_set*		o;
  t_setsz		i;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  for (i = current.u.array.i - 1; i >= 0; --i)
    {
      if (o->u.array.array[i])
	break;
    }

  if (i == -1)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  previous->u.array.i = i;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function returns an iterator on the next entry of the array.
 *
 * steps:
 *
 * 1) gets the descriptor.
 * 2) moves to the next non-empty slot.
 * 3) updates the iterator.
 */

t_error			set_next_array(t_setid			setid,
				       t_iterator		current,
				       t_iterator*		next)
{
  o_set*		o;
  t_setsz		i;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  for (i = current.u.array.i + 1; i < o->u.array.arraysz; ++i)
    {
      if (o->u.array.array[i])
	break;
    }

  if (i >= o->u.array.arraysz)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  next->u.array.i = i;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this  function is  used when  no room  is found  in the  array. it
 * expands the array and copy needed data.
 *
 * steps:
 *
 * 1) computes the new size for the array
 * 2) expands the array
 * 3) initialises created slots
 */

static t_error		set_expand_array(o_set			*o,
					 void			*data)
{
  t_setsz		i;
  t_setsz		sz;

  i = o->u.array.arraysz;

  /*
   * 1)
   */

  if (o->opts & SET_OPT_ORGANISE)
    sz = o->u.array.arraysz + 1;
  else
    sz = o->u.array.arraysz * 2;

  /*
   * 2)
   */

  if ((o->u.array.array = realloc(o->u.array.array,
				  sz * sizeof(void*))) == NULL)
    return ERROR_UNKNOWN;

  o->u.array.arraysz = sz;

  /*
   * 3)
   */

  o->u.array.array[i] = data;
  for (++i; i < o->u.array.arraysz; ++i)
    o->u.array.array[i] = NULL;

  return ERROR_NONE;
}

/*
 * this function inserts an element at the specified place.
 *
 * steps:
 *
 * 1) expands the array if necessary.
 * 2) inserts in its place if this one is empty.
 * 3) inserts just before if possible.
 * 4) shifts right to make some room and inserts.
 */

static t_error		set_insert_at(o_set			*o,
				      t_setsz			pos,
				      void			*data)
{
  t_setsz		limit;

  /*
   * 1)
   */

  if (o->u.array.arraysz == pos)
    {
      return set_expand_array(o, data);
    }

  /*
   * 2)
   */

  if (o->u.array.array[pos] == NULL)
    {
      o->u.array.array[pos] = data;
      return ERROR_NONE;
    }

  /*
   * 3)
   */

  if (pos > 0 && !(o->opts & SET_OPT_ORGANISE))
    {
      if (o->u.array.array[pos - 1] == NULL)
	{
	  o->u.array.array[pos - 1] = data;
	  return ERROR_NONE;
	}
    }

  /*
   * 4)
   */

  for (limit = pos; limit < o->u.array.arraysz; ++limit)
    {
      if (o->u.array.array[limit] == NULL)
	break;
    }
  if (limit == o->u.array.arraysz)
    {
      if (set_expand_array(o, o->u.array.array[limit - 1]) !=
	  ERROR_NONE)
	return ERROR_UNKNOWN;
    }
  for (; limit > pos; --limit)
    {
      o->u.array.array[limit] = o->u.array.array[limit - 1];
    }
  o->u.array.array[pos] = data;

  return ERROR_NONE;
}

/*
 * this function inserts a new entry at the head of the array.
 *
 * steps:
 * 1) gets the descriptor.
 * 2) checks options.
 * 3) copies the element if necessary.
 * 4) inserts the element in its place.
 * 5) updates count.
 */

t_error			set_insert_array(t_setid		setid,
					 void*			data)
{
  o_set*		o;
  void*			cpy;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (o->opts & SET_OPT_SORT)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (o->opts & SET_OPT_ALLOC)
    {
      if (!(cpy = malloc(o->datasz)))
	SET_LEAVE(set, ERROR_UNKNOWN);
      memcpy(cpy, data, o->datasz);
      data = cpy;
    }

  /*
   * 4)
   */

  if (set_insert_at(o, 0, data) != ERROR_NONE)
    {
      if (o->opts & SET_OPT_ALLOC)
	free(data);
      SET_LEAVE(set, ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  ++o->size;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function inserts a new entry at the tail of the array.
 *
 * steps:
 * 1) gets the descriptor.
 * 2) checks options.
 * 3) copies the element if necessary.
 * 4) inserts the element in its place.
 * 5) updates count.
 */

t_error			set_append_array(t_setid		setid,
					 void*			data)
{
  o_set*		o;
  t_setsz		i;
  void*			cpy;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (o->opts & SET_OPT_SORT)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (o->opts & SET_OPT_ALLOC)
    {
      if (!(cpy = malloc(o->datasz)))
	SET_LEAVE(set, ERROR_UNKNOWN);
      memcpy(cpy, data, o->datasz);
      data = cpy;
    }

  /*
   * 4)
   */

  if (o->size)
    {
      for (i = o->u.array.arraysz; i > 0; --i)
	{
	  if (o->u.array.array[i - 1])
	    break;
	}
    }
  else
    i = 0;

  if (set_insert_at(o, i, data) != ERROR_NONE)
    {
      if (o->opts & SET_OPT_ALLOC)
	free(data);
      SET_LEAVE(set, ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  ++o->size;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function inserts an object before the one referenced by the iterator.
 *
 * steps:
 * 1) gets the descriptor.
 * 2) checks options.
 * 3) copies the element if necessary.
 * 4) inserts the element in its place.
 * 5) updates item count.
 */

t_error			set_before_array(t_setid		setid,
					 t_iterator		iterator,
					 void*			data)
{
  o_set*		o;
  t_setsz		i;
  void*			cpy;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (o->opts & SET_OPT_SORT)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (o->opts & SET_OPT_ALLOC)
    {
      if (!(cpy = malloc(o->datasz)))
	SET_LEAVE(set, ERROR_UNKNOWN);
      memcpy(cpy, data, o->datasz);
      data = cpy;
    }

  /*
   * 4)
   */

  i = iterator.u.array.i;

  if (i < 0)
    i = 0;

  if (set_insert_at(o, i, data) != ERROR_NONE)
    {
      if (o->opts & SET_OPT_ALLOC)
	free(data);
      SET_LEAVE(set, ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  ++o->size;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function inserts a new object after a existing one referenced
 * by the iterator.
 *
 * steps:
 * 1) gets the descriptor.
 * 2) checks options.
 * 3) copies the element if necessary.
 * 4) inserts the element in its place.
 * 5) updates count.
 */

t_error			set_after_array(t_setid			setid,
					t_iterator		iterator,
					void*			data)
{
  o_set*       		o;
  t_setsz		i;
  void*			cpy;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (o->opts & SET_OPT_SORT)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (o->opts & SET_OPT_ALLOC)
    {
      if (!(cpy = malloc(o->datasz)))
	SET_LEAVE(set, ERROR_UNKNOWN);
      memcpy(cpy, data, o->datasz);
      data = cpy;
    }

  /*
   * 4)
   */

  i = iterator.u.array.i + 1;

  if (i > o->u.array.arraysz)
    i = o->u.array.arraysz;

  if (set_insert_at(o, i, data) != ERROR_NONE)
    {
      if (o->opts & SET_OPT_ALLOC)
	free(data);
      SET_LEAVE(set, ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  ++o->size;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function adds an object in the array.
 *
 * steps:
 *
 * 1) checks the data.
 * 2) get the object associated with the id.
 * 3) copies the object if required.
 * 4) inserts the element.
 *   A) (sorted array)
 *    i) finds the place of the new element.
 *    ii) copies in the right place.
 *    iii) expands the array if necessary.
 *   B) (unsorted array)
 *    i) tries to find a free place.
 *    ii) expands the array if necessary.
 * 5) updates size.
 */

t_error			set_add_array(t_setid			setid,
				      void*			data)
{
  o_set*		o;
  t_setsz		i;
  t_id			id;
  t_id			current;
  void*			cpy;
  t_uint8		empty;
  t_setsz		last;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (!data || *((t_id*)data) == ID_UNUSED)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (o->opts & SET_OPT_ALLOC)
    {
      if (!(cpy = malloc(o->datasz)))
	SET_LEAVE(set, ERROR_UNKNOWN);
      memcpy(cpy, data, o->datasz);
      data = cpy;
    }

  /*
   * 4)
   */

  if (o->opts & SET_OPT_SORT)
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
		  cons_msg('!', "set: identifier collision detected "
			   "in the set %qu on the object identifier %qu\n",
			   o->setid, id);

		  if (o->opts & SET_OPT_ALLOC)
		    free(data);

		  SET_LEAVE(set, ERROR_UNKNOWN);
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
	  if (set_insert_at(o, last, data) != ERROR_NONE)
	    {
	      if (o->opts & SET_OPT_ALLOC)
		free(data);
	      SET_LEAVE(set, ERROR_UNKNOWN);
	    }
	}

      /*
       * iii)
       */

      if (last >= o->u.array.arraysz)
	{
	  if (set_expand_array(o, data) != ERROR_NONE)
	    {
	      if (o->opts & SET_OPT_ALLOC)
		free(data);
	      SET_LEAVE(set, ERROR_UNKNOWN);
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
	  if (set_expand_array(o, data) != ERROR_NONE)
	    {
	      if (o->opts & SET_OPT_ALLOC)
		free(data);
	      SET_LEAVE(set, ERROR_UNKNOWN);
	    }
	}
    }

  /*
   * 5)
   */

  ++o->size;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function removes a object from the array, setting the identifier
 * field of the entry as 'unused'.
 *
 * steps:
 *
 * 1) checks the given id.
 * 2) gets object associated to setid.
 * 3) looks for the element, then delete it.
 * 4) re-organises the set if needed.
 * 5) decrements counter.
 */

t_error			set_remove_array(t_setid		setid,
					 t_id			id)
{
  o_set*       		o;
  t_setsz		i;
  t_id			current;

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

  if (o->size == 0)
    SET_LEAVE(set, ERROR_UNKNOWN);

  for (i = 0; i < o->u.array.arraysz; ++i)
    {
      if (o->u.array.array[i])
	{
	  current = *((t_id*)(o->u.array.array[i]));

	  if ((o->opts & SET_OPT_SORT) && current > id)
	    SET_LEAVE(set, ERROR_UNKNOWN);

	  if (current == id)
	    {
	      if ((o->opts & SET_OPT_FREE) ||
		  (o->opts & SET_OPT_ALLOC))
		free(o->u.array.array[i]);
	      o->u.array.array[i] = NULL;
	      break;
	    }
	}
    }

  if (i == o->u.array.arraysz)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (o->opts & SET_OPT_ORGANISE)
    {
      for (; i < o->u.array.arraysz - 1; ++i)
	{
	  o->u.array.array[i] = o->u.array.array[i + 1];
	}

      --o->u.array.arraysz;
      o->u.array.array = realloc(o->u.array.array,
				 o->u.array.arraysz *
				 sizeof(void*));
    }

  /*
   * 5)
   */

  --o->size;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function deletes an element given an iterator.
 *
 * steps:
 *
 * 1) gets the array.
 * 2) checks bounds.
 * 3) checks if the place isn't already empty.
 * 4) frees the element if necessary, then removes it.
 * 5) organises the set if necessary.
 * 6) updates counter.
 */

t_error			set_delete_array(t_setid		setid,
					 t_iterator		iterator)
{
  o_set*		o;
  t_setsz		i;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  i = iterator.u.array.i;
  if (i >= o->u.array.arraysz)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (o->u.array.array[i] == NULL)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (o->opts & SET_OPT_ALLOC ||
      o->opts & SET_OPT_FREE)
    free(o->u.array.array[i]);
  o->u.array.array[i] = NULL;

  /*
   * 5)
   */

  if (o->opts & SET_OPT_ORGANISE)
    {
      for (; i < o->u.array.arraysz - 1; ++i)
	{
	  o->u.array.array[i] = o->u.array.array[i + 1];
	}

      --o->u.array.arraysz;
      o->u.array.array = realloc(o->u.array.array,
				 o->u.array.arraysz *
				 sizeof(void*));
    }

  /*
   * 6)
   */

  o->size--;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function flushes the set, freeing each element.
 *
 * steps:
 *
 * 1) gets descriptor.
 * 2) frees elements if needed.
 * 3) resets the array.
 */

t_error			set_flush_array(t_setid			setid)
{
  o_set*       		o;
  t_setsz		i;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if ((o->opts & SET_OPT_FREE) ||
      (o->opts & SET_OPT_ALLOC))
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

  if ((o->u.array.array =
       realloc(o->u.array.array,
	       o->u.array.initsz * sizeof(void*))) == NULL)
    SET_LEAVE(set, ERROR_UNKNOWN);
  for (i = 0; i < o->u.array.initsz; ++i)
    {
      o->u.array.array[i] = NULL;
    }

  o->u.array.arraysz = o->u.array.initsz;
  o->size = 0;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function tries to find an object with its identifier and build
 * a corresponding identifier.
 *
 * steps:
 *
 * 1) checks the given id.
 * 2) gets the descriptor.
 * 3) search for the element.
 *   A) dichotomy lookup. (on sorted array)
 *   B) sequentially searchs for the needed element. (unsorted)
 */

t_error			set_locate_array(t_setid		setid,
					 t_id			id,
					 t_iterator*		iterator)
{
  o_set*       		o;
  t_setid		i;
  t_setsz		left;
  t_setsz		right;
  t_setsz		dicho;

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
/* XXX la dicho marche mal */
  if (0 && (o->opts & SET_OPT_SORT))
    {

      /*
       * A)
       */

      left = 0;
      right = o->u.array.arraysz - 1;

      while (left <= right)
	{
	  dicho = left + (right - left) / 2;
	  for (i = dicho; i < right; ++i)
	    if (o->u.array.array[i])
	      break;
	  if (*((t_id*)(o->u.array.array[i])) == id)
	    break;
	  if (i == right || *((t_id*)(o->u.array.array[i])) > id)
	    {
	      right = dicho - 1;
	    }
	  else
	    {
	      left = dicho + 1;
	    }
	}

      if (left <= right)
	{
	  iterator->u.array.i = i;
	  SET_LEAVE(set, ERROR_NONE);
	}
    }
  else
    {

      /*
       * B)
       */

      for (i = 0; i < o->u.array.arraysz; ++i)
	{
	  if (o->u.array.array[i] &&
	      *((t_id*)(o->u.array.array[i])) == id)
	    {
	      iterator->u.array.i = i;
	      SET_LEAVE(set, ERROR_NONE);
	    }
	}
    }

  SET_LEAVE(set, ERROR_UNKNOWN);
}

/*
 * this function returns an object given its iterator.
 *
 * steps:
 *
 * 1) gets the set object.
 * 2) checks bounds.
 * 3) gets data for given position.
 */

t_error			set_object_array(t_setid		setid,
					 t_iterator		iterator,
					 void**			data)
{
  o_set*       		o;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (iterator.u.array.i >= o->u.array.arraysz)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (o->u.array.array[iterator.u.array.i])
    *data = o->u.array.array[iterator.u.array.i];
  else
    SET_LEAVE(set, ERROR_UNKNOWN);

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function reserves a set.
 *
 * steps:
 *
 * 1) checks options for the array.
 * 2) initialises the set descriptor.
 * 3) if necessary, reserves an unused identifier for this new set.
 * 4) initialises the set fields allocating the array.
 * 5) adds the set descriptor in the set container.
 */

t_error			set_reserve_array(t_opts		opts,
					  t_setsz		initsz,
					  t_size		datasz,
					  t_setid*		setid)
{
  o_set			o;
  t_setsz		i;

  SET_ENTER(set);

  /*
   * 1)
   */

  if ((opts & SET_OPT_ALLOC) && (opts & SET_OPT_FREE))
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  memset(&o, 0x0, sizeof(o_set));

  /*
   * 3)
   */

  if (opts & SET_OPT_CONTAINER)
    {
      *setid = set->sets;
    }
  else
    {
      if (id_reserve(&set->id, setid) != ERROR_NONE)
	SET_LEAVE(set, ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  o.setid = *setid;
  o.size = 0;
  o.type = SET_TYPE_ARRAY;
  o.opts = opts;
  o.datasz = datasz;

  o.u.array.arraysz = initsz == 0 ? 1 : initsz;
  o.u.array.initsz = o.u.array.arraysz;

  if ((o.u.array.array =
       malloc(o.u.array.arraysz * sizeof(void*))) == NULL)
    {
      if (!(opts & SET_OPT_CONTAINER))
	id_release(&set->id, o.setid);

      SET_LEAVE(set, ERROR_UNKNOWN);
    }

  for (i = 0; i < o.u.array.arraysz; i++)
    o.u.array.array[i] = NULL;

  /*
   * 5)
   */

  if (set_new(&o) != ERROR_NONE)
    {
      free(o.u.array.array);

      if (!(opts & SET_OPT_CONTAINER))
	id_release(&set->id, o.setid);

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
 * 2) flushs the set.
 * 3) releases the set identifier.
 * 4) frees the array allocated at the set reservation.
 * 5) then, removes the set from the set container.
 */

t_error			set_release_array(t_setid		setid)
{
  o_set*       		o;

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

  if (id_release(&set->id, o->setid) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 4)
   */

  free(o->u.array.array);

  /*
   * 5)
   */

  if (!(o->opts & SET_OPT_CONTAINER))
    if (set_destroy(o->setid) != ERROR_NONE)
      SET_LEAVE(set, ERROR_UNKNOWN);

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function just returns an error because the array set does not
 * support this operation.
 */

t_error			set_push_array(t_setid			setid,
				       void*			data)
{
  SET_ENTER(set);

  SET_LEAVE(set, ERROR_UNKNOWN);
}

/*
 * this function just returns an error because the array set does not
 * support this operation.
 */

t_error			set_pop_array(t_setid			setid)
{
  SET_ENTER(set);

  SET_LEAVE(set, ERROR_UNKNOWN);
}

/*
 * this function just returns an error because the array set does not
 * support this operation.
 */

t_error			set_pick_array(t_setid			setid,
				       void**			data)
{
  SET_ENTER(set);

  SET_LEAVE(set, ERROR_UNKNOWN);
}

/*                                                                 [cut] /k2 */
