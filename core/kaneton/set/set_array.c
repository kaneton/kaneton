/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/kaneton/set/set_array.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [tue dec  6 00:44:59 2005]
 */

/*
 *             /!\ THIS  CODE  IS  IN  TESTING  PHASE /!\
 *
 * my TODO:
 *  - improve set_locate_array on sorted array (something like dichotomy)
 *  - add more tests
 *  - take a look at the comments and check them
 *  ? realloc in remove when ORGANISE ??
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this  subpart  of the  set  manager is  used  to  build array  data
 * structures.
 *
 * there are two way to use the  arrays : with id or with objects. the
 * first method is  the default one (without SET_OPT_ALLOC)  : it only
 * stores id refering to objects in the array. The second method (used
 * with  SET_OPT_ALLOC) makes  copies  of objects  in  the set,  using
 * memcpy.
 *
 * the option ORGANISE is used to keep the array as small as possible,
 * but  some operation  requires to  shift entire  parts of  the array
 * (loss of performances).
 *
 * when an array set is cloned, data are reorganised in. it means than
 * when a  clone operation occurs,  the array is reduced  clearing the
 * empty/unused places.
 *
 * options:    SET_OPT_CONTAINER,    SET_OPT_SORT,   SET_OPT_ORGANISE,
 * SET_OPT_ALLOC
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
 */

t_error			set_type_array(t_setid			setid)
{
  o_set*		o;

  SET_ENTER(set);

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

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
 * 2) loop throught the slots and displays elements.
 */

t_error			set_show_array(t_setid			setid)
{
  o_set			*o;
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

  if (o->u.array.opts & SET_OPT_ALLOC)
    {
      for (i = 0, pos = 0; i < o->u.array.arraysz; ++i)
	{
	  if (o->u.array.array.pdata[i])
	    id = *((t_id *)(o->u.array.array.pdata[i]));
	  else
	    continue;

	  if (id == ID_UNUSED)
	    cons_msg('!', "warning: unused object detected !\n");

	  cons_msg('#', "  array[%qd] = %qd\n", pos, id);
	  ++pos;
	}
    }
  else
    {
      for (i = 0, pos = 0; i < o->u.array.arraysz; ++i)
	{
	  id = o->u.array.array.id[i];

	  if (id == ID_UNUSED)
	    continue;

	  cons_msg('#', "  array[%qd] = %qd\n", pos, id);
	  ++pos;
	}
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

  if (o->u.array.opts & SET_OPT_ALLOC)
    {
      for (i = 0; i < o->u.array.arraysz; ++i)
	{
	  if (o->u.array.array.pdata[i])
	    break;
	}
    }
  else
    {
      for (i = 0; i < o->u.array.arraysz; ++i)
	{
	  if (o->u.array.array.id[i] != ID_UNUSED)
	    break;
	}
    }

  if (i == o->u.array.arraysz)
    SET_LEAVE(set, ERROR_UNKNOWN);

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

  if (o->u.array.opts & SET_OPT_ALLOC)
    {
      for (i = o->u.array.arraysz - 1; i >= 0; --i)
	{
	  if (o->u.array.array.pdata[i])
	    break;
	}
    }
  else
    {
      for (i = o->u.array.arraysz - 1; i >= 0; --i)
	{
	  if (o->u.array.array.id[i] != ID_UNUSED)
	    break;
	}
    }

  if (i == -1)
    SET_LEAVE(set, ERROR_UNKNOWN);

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

t_error			set_prev_array(t_setid			setid,
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

  if (o->u.array.opts & SET_OPT_ALLOC)
    {
      for (i = current.u.array.i - 1; i >= 0; --i)
	{
	  if (o->u.array.array.pdata[i])
	    break;
	}
    }
  else
    {
      for (i = current.u.array.i - 1; i >= 0; --i)
	{
	  if (o->u.array.array.id[i] != ID_UNUSED)
	    break;
	}
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

  if (o->u.array.opts & SET_OPT_ALLOC)
    {
      for (i = current.u.array.i + 1; i < o->u.array.arraysz; ++i)
	{
	  if (o->u.array.array.pdata[i])
	    break;
	}
    }
  else
    {
      for (i = current.u.array.i + 1; i < o->u.array.arraysz; ++i)
	{
	  if (o->u.array.array.id[i] != ID_UNUSED)
	    break;
	}
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

static t_error		set_expand_array(o_set		*o,
					 void		*data)
{
  t_setsz		i;
  t_setsz		sz;

  i = o->u.array.arraysz;

  /*
   * 1)
   */

  if (o->u.array.opts & SET_OPT_ORGANISE)
    sz = o->u.array.arraysz + 1;
  else
    sz = o->u.array.arraysz * 2;

  /*
   * 2)
   */

  if (o->u.array.opts & SET_OPT_ALLOC)
    {
      if ((o->u.array.array.pdata = realloc(o->u.array.array.pdata,
					    sz * sizeof (t_uint8*))) == NULL)
	return ERROR_UNKNOWN;
    }
  else
    {
      if ((o->u.array.array.id = realloc(o->u.array.array.id,
					 sz * sizeof (t_id))) == NULL)
	return ERROR_UNKNOWN;
    }

  o->u.array.arraysz = sz;

  /*
   * 3)
   */

  if (o->u.array.opts & SET_OPT_ALLOC)
    {
      o->u.array.array.pdata[i] = data;
      for (++i; i < o->u.array.arraysz; ++i)
	o->u.array.array.pdata[i] = NULL;
    }
  else
    {
      memcpy(&o->u.array.array.id[i], data, sizeof (t_id));
      for (++i; i < o->u.array.arraysz; ++i)
	o->u.array.array.id[i] = ID_UNUSED;
    }

  return ERROR_NONE;
}

/*
 * this function inserts an element at the specified place.
 *
 * steps:
 *
 * 1) inserts in its place if this one is empty.
 * 2) inserts just before if possible.
 * 3) shifts right to make some room and inserts.
 */

static t_error		set_insert_at(o_set		*o,
				      t_setsz		pos,
				      void		*data)
{
  t_setsz		i;
  t_setsz		limit;

  /*
   * 1)
   */

  if (o->u.array.arraysz == pos)
    {
      return set_expand_array(o, data);
    }

  if (o->u.array.opts & SET_OPT_ALLOC)
    {
      if (o->u.array.array.pdata[pos] == NULL)
	{
	  o->u.array.array.pdata[pos] = data;
	  return ERROR_NONE;
	}
    }
  else
    {
      if (o->u.array.array.id[pos] == ID_UNUSED)
	{
	  o->u.array.array.id[pos] = *((t_id *)data);
	  return ERROR_NONE;
	}
    }

  /*
   * 2)
   */

  if (pos > 0 && !(o->u.array.opts & SET_OPT_ORGANISE))
    {
      if (o->u.array.opts & SET_OPT_ALLOC)
	{
	  if (o->u.array.array.pdata[pos - 1] == NULL)
	    {
	      o->u.array.array.pdata[pos - 1] = data;
	      return ERROR_NONE;
	    }
	}
      else
	{
	  if (o->u.array.array.id[pos - 1] == ID_UNUSED)
	    {
	      o->u.array.array.id[pos - 1] = *((t_id *)data);
	      return ERROR_NONE;
	    }
	}
    }

  /*
   * 3)
   */

  if (o->u.array.opts & SET_OPT_ALLOC)
    {
      for (limit = pos; limit < o->u.array.arraysz; ++limit)
	{
	  if (o->u.array.array.pdata[limit] == NULL)
	    break;
	}
      if (limit == o->u.array.arraysz)
	{
	  if (set_expand_array(o, o->u.array.array.pdata[limit - 1]) !=
	      ERROR_NONE)
	    return ERROR_UNKNOWN;
	}
      for (; limit > pos; --limit)
	{
	  o->u.array.array.pdata[limit] = o->u.array.array.pdata[limit - 1];
	}
      o->u.array.array.pdata[pos] = data;
    }
  else
    {
      for (limit = pos; limit < o->u.array.arraysz; ++limit)
	{
	  if (o->u.array.array.id[limit] == ID_UNUSED)
	    break;
	}
      if (limit == o->u.array.arraysz)
	{
	  if (set_expand_array(o, &o->u.array.array.id[limit - 1]) !=
	      ERROR_NONE)
	    return ERROR_UNKNOWN;
	}
      for (; limit > pos; --limit)
	{
	  o->u.array.array.id[limit] = o->u.array.array.id[limit - 1];
	}
      o->u.array.array.id[pos] = *((t_id *)data);
    }

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
 */

t_error			set_insert_head_array(t_setid		setid,
					      void*		data)
{
  o_set			*o;
  void			*cpy;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (o->u.array.opts & SET_OPT_SORT)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (o->u.array.opts & SET_OPT_ALLOC)
    {
      if (!(cpy = malloc(o->u.array.datasz)))
	SET_LEAVE(set, ERROR_UNKNOWN);
      memcpy(cpy, data, o->u.array.datasz);
      data = cpy;
    }

  /*
   * 4)
   */

  if (set_insert_at(o, 0, data) != ERROR_NONE)
    {
      if (o->u.array.opts & SET_OPT_ALLOC)
	free(data);
      SET_LEAVE(set, ERROR_UNKNOWN);
    }

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
 */

t_error			set_insert_tail_array(t_setid		setid,
					      void*		data)
{
  o_set			*o;
  t_setsz		i;
  void			*cpy;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (o->u.array.opts & SET_OPT_SORT)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (o->size)
    {
      if (o->u.array.opts & SET_OPT_ALLOC)
	{
	  for (i = o->u.array.arraysz; i > 0; --i)
	    {
	      if (o->u.array.array.pdata[i - 1])
		break;
	    }
	}
      else
	{
	  for (i = o->u.array.arraysz; i > 0; --i)
	    {
	      if (o->u.array.array.id[i - 1] != ID_UNUSED)
		break;
	    }
	}
    }
  else
    i = 0;

  if (o->u.array.opts & SET_OPT_ALLOC)
    {
      if (!(cpy = malloc(o->u.array.datasz)))
	SET_LEAVE(set, ERROR_UNKNOWN);
      memcpy(cpy, data, o->u.array.datasz);
      data = cpy;
    }

  /*
   * 4)
   */

  if (set_insert_at(o, i, data) != ERROR_NONE)
    {
      if (o->u.array.opts & SET_OPT_ALLOC)
	free(data);
      SET_LEAVE(set, ERROR_UNKNOWN);
    }

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
 */

t_error			set_insert_before_array(t_setid		setid,
						t_iterator	iterator,
						void*		data)
{
  o_set			*o;
  t_setsz		i;
  void			*cpy;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (o->u.array.opts & SET_OPT_SORT)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (o->u.array.opts & SET_OPT_ALLOC)
    {
      if (!(cpy = malloc(o->u.array.datasz)))
	SET_LEAVE(set, ERROR_UNKNOWN);
      memcpy(cpy, data, o->u.array.datasz);
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
      if (o->u.array.opts & SET_OPT_ALLOC)
	free(data);
      SET_LEAVE(set, ERROR_UNKNOWN);
    }

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
 */

t_error			set_insert_after_array(t_setid		setid,
					       t_iterator	iterator,
					       void*		data)
{
  o_set			*o;
  t_setsz		i;
  void			*cpy;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (o->u.array.opts & SET_OPT_SORT)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (o->u.array.opts & SET_OPT_ALLOC)
    {
      if (!(cpy = malloc(o->u.array.datasz)))
	SET_LEAVE(set, ERROR_UNKNOWN);
      memcpy(cpy, data, o->u.array.datasz);
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
      if (o->u.array.opts & SET_OPT_ALLOC)
	free(data);
      SET_LEAVE(set, ERROR_UNKNOWN);
    }

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
 *   A) (sorted array)
 *    i) finds the place of the new element.
 *    ii) copies in the right place.
 *    iii) expands the array if necessary.
 *   B) (unsorted array)
 *    i) tries to find a free place.
 *    ii) expands the array if necessary.
 */

t_error			set_add_array(t_setid			setid,
				      void*			data)
{
  o_set			*o;
  t_setsz		i;
  t_id			id;
  t_id			current;
  void			*cpy;
  t_uint8		empty;
  t_setsz		last;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (!data || *((t_id *)data) == ID_UNUSED)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (o->u.array.opts & SET_OPT_ALLOC)
    {
      if (!(cpy = malloc(o->u.array.datasz)))
	SET_LEAVE(set, ERROR_UNKNOWN);
      memcpy(cpy, data, o->u.array.datasz);
      data = cpy;
    }

  if (o->u.array.opts & SET_OPT_SORT)
    {
      /*
       * A)
       */

      /*
       * i)
       */

      empty = 1;
      last = -1;
      id = *((t_id *)data);
      if (o->u.array.opts & SET_OPT_ALLOC)
	{
	  for (i = 0; i < o->u.array.arraysz; ++i)
	    {
	      if (o->u.array.array.pdata[i] != NULL)
		{
		  current = *((t_id *)o->u.array.array.pdata[i]);
		  empty &= (current == ID_UNUSED);
		  if (current == ID_UNUSED)
		    continue;
		  if (current == id)
		    {
		      cons_msg('!', "set: identifier collision detected "
			       "in the set %qu on the object identifier %qu\n",
			       o->setid, id);

		      if (o->u.array.opts & SET_OPT_ALLOC)
			free(data);

		      SET_LEAVE(set, ERROR_UNKNOWN);
		    }
		  if (current > id)
		    break;
		  last = i;
		}
	    }
	}
      else
	{
	  for (i = 0; i < o->u.array.arraysz; ++i)
	    {
	      current = o->u.array.array.id[i];
	      empty &= (current == ID_UNUSED);
	      if (current == ID_UNUSED)
		continue;
	      if (current == id)
		{
		  cons_msg('!', "set: identifier collision detected "
			   "in the set %qu on the object identifier %qu\n",
			   o->setid, id);

		  if (o->u.array.opts & SET_OPT_ALLOC)
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
	      if (o->u.array.opts & SET_OPT_ALLOC)
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
	      if (o->u.array.opts & SET_OPT_ALLOC)
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

      if (o->u.array.opts & SET_OPT_ALLOC)
	{
	  for (i = 0; i < o->u.array.arraysz; ++i)
	    {
	      if (o->u.array.array.pdata[i] == NULL)
		{
		  o->u.array.array.pdata[i] = data;
		  break;
		}
	    }
	}
      else
	{
	  for (i = 0; i < o->u.array.arraysz; ++i)
	    {
	      if (o->u.array.array.id[i] == ID_UNUSED)
		{
		  memcpy(&o->u.array.array.id[i], data, sizeof (t_id));
		  break;
		}
	    }
	}

      /*
       * ii)
       */

      if (i == o->u.array.arraysz)
	{
	  if (set_expand_array(o, data) != ERROR_NONE)
	    {
	      if (o->u.array.opts & SET_OPT_ALLOC)
		free(data);
	      SET_LEAVE(set, ERROR_UNKNOWN);
	    }
	}
    }

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
 */

t_error			set_remove_array(t_setid		setid,
					 t_id			id)
{
  o_set			*o;
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

  if (o->u.array.opts & SET_OPT_ALLOC)
    {
      for (i = 0; i < o->u.array.arraysz; ++i)
	{
	  current = *((t_id *)(o->u.array.array.pdata[i]));

	  if ((o->u.array.opts & SET_OPT_SORT) && current > id)
	    SET_LEAVE(set, ERROR_UNKNOWN);

	  if (current == id)
	    {
	      free(o->u.array.array.pdata[i]);
	      o->u.array.array.pdata[i] = NULL;
	      break;
	    }
	}
    }
  else
    {
      for (i = 0; i < o->u.array.arraysz; ++i)
	{
	  current = o->u.array.array.id[i];

	  if ((o->u.array.opts & SET_OPT_SORT) && current > id)
	    SET_LEAVE(set, ERROR_UNKNOWN);

	  if (current == id)
	    {
	      o->u.array.array.id[i] = ID_UNUSED;
	      break;
	    }
	}
    }

  if (i == o->u.array.arraysz)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (o->u.array.opts & SET_OPT_ORGANISE)
    {
      if (o->u.array.opts & SET_OPT_ALLOC)
	{
	  for (; i < o->u.array.arraysz - 1; ++i)
	    {
	      o->u.array.array.pdata[i] = o->u.array.array.pdata[i + 1];
	    }

	  --o->u.array.arraysz;
	  o->u.array.array.pdata = realloc(o->u.array.array.pdata,
					   o->u.array.arraysz *
					   sizeof (t_uint8*));
	}
      else
	{
	  for (; i < o->u.array.arraysz - 1; ++i)
	    {
	      o->u.array.array.id[i] = o->u.array.array.id[i + 1];
	    }

	  --o->u.array.arraysz;
	  o->u.array.array.id = realloc(o->u.array.array.id,
					o->u.array.arraysz *
					sizeof (t_id));
	}
    }

  --o->size;

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
  o_set			*o;
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

  if (o->u.array.opts & SET_OPT_ALLOC)
    {
      for (i = 0; i < o->u.array.arraysz; ++i)
	{
	  if (o->u.array.array.pdata[i])
	    {
	      free(o->u.array.array.pdata[i]);
	      o->u.array.array.pdata[i] = NULL;
	    }
	}
      o->size = 0;
    }

  /*
   * 3)
   */

  if (o->u.array.opts & SET_OPT_ALLOC)
    {
      if ((o->u.array.array.pdata =
	   realloc(o->u.array.array.pdata,
		   o->u.array.initsz * sizeof (t_uint8*))) == NULL)
	SET_LEAVE(set, ERROR_UNKNOWN);
      for (i = 0; i < o->u.array.initsz; ++i)
	{
	  o->u.array.array.pdata[i] = NULL;
	}
    }
  else
    {
      if ((o->u.array.array.id =
	   realloc(o->u.array.array.id,
		   o->u.array.initsz * sizeof (t_id))) == NULL)
	SET_LEAVE(set, ERROR_UNKNOWN);
      for (i = 0; i < o->u.array.initsz; ++i)
	{
	  o->u.array.array.id[i] = ID_UNUSED;
	}
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
 * A) TODO: dichotomy lookup (on sorted array)
 * B) sequentially searchs for the needed element. (unsorted)
 */

t_error			set_locate_array(t_setid		setid,
					 t_id			id,
					 t_iterator*		iterator)
{
  o_set			*o;
  t_setid		i;
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

  if ((o->u.array.opts & SET_OPT_SORT))
    {
      /*
       * A)
       */

      if (o->u.array.opts & SET_OPT_ALLOC)
	{
	  for (i = 0; i < o->u.array.arraysz; ++i)
	    {
	      if (!o->u.array.array.pdata[i])
		continue;
	      current = *((t_id *)(o->u.array.array.pdata[i]));
	      if (current == id)
		{
		  iterator->u.array.i = i;
		  SET_LEAVE(set, ERROR_NONE);
		}
	      if (current > id)
		break;
	    }
	}
      else
	{
	  for (i = 0; i < o->u.array.arraysz; ++i)
	    {
	      if (o->u.array.array.id[i] == ID_UNUSED)
		continue;
	      if (o->u.array.array.id[i] == id)
		{
		  iterator->u.array.i = i;
		  SET_LEAVE(set, ERROR_NONE);
		}
	      if (o->u.array.array.id[i] > id)
		break;
	    }
	}
    }
  else
    {

      /*
       * B)
       */

      if (o->u.array.opts & SET_OPT_ALLOC)
	{
	  for (i = 0; i < o->u.array.arraysz; ++i)
	    {
	      if (o->u.array.array.pdata[i] &&
		  *((t_id *)(o->u.array.array.pdata[i])) == id)
		{
		  iterator->u.array.i = i;
		  SET_LEAVE(set, ERROR_NONE);
		}
	    }
	}
      else
	{
	  for (i = 0; i < o->u.array.arraysz; ++i)
	    {
	      if (o->u.array.array.id[i] == id)
		{
		  iterator->u.array.i = i;
		  SET_LEAVE(set, ERROR_NONE);
		}
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
 * 4) checks for validity.
 */

t_error			set_object_array(t_setid		setid,
					 t_iterator		iterator,
					 void**			data)
{
  o_set			*o;
  t_id			*id;

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

  if (o->u.array.opts & SET_OPT_ALLOC)
    {
      if (o->u.array.array.pdata[iterator.u.array.i])
	id = ((t_id *)(o->u.array.array.pdata[iterator.u.array.i]));
      else
	id = NULL;
    }
  else
    id = &o->u.array.array.id[iterator.u.array.i];

  /*
   * 4)
   */

  if (id == NULL || *id == ID_UNUSED)
    SET_LEAVE(set, ERROR_UNKNOWN);

  *data = id;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function clones a set.
 *
 * steps:
 *
 * 1) gets the descriptor.
 * 2) reserves a new set.
 * 3) copies data.
 */

t_error			set_clone_array(t_setid			old,
					t_setid*		new)
{
  o_set			*o;
  t_setid		clone;
  t_iterator		it;
  t_state		state;
  void			*pdata;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (set_descriptor(old, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (set_reserve_array(o->u.array.opts, o->size, o->u.array.datasz,
			&clone) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  set_foreach(SET_OPT_FORWARD, old, &it, state)
    {
      set_object(old, it, (void **)&pdata);
      set_add(clone, pdata);
    }

  *new = clone;

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

  if (opts & SET_OPT_FREE)
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
      *setid = set->container;
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

  o.u.array.opts = opts;
  o.u.array.datasz = (opts & SET_OPT_ALLOC) ? datasz : sizeof (t_id);
  o.u.array.arraysz = initsz == 0 ? 1 : initsz;
  o.u.array.initsz = o.u.array.arraysz;

  if (opts & SET_OPT_ALLOC)
    {
      if ((o.u.array.array.pdata =
	   malloc(o.u.array.arraysz * sizeof (t_uint8*))) == NULL)
	{
	  if (!(opts & SET_OPT_CONTAINER))
	    id_release(&set->id, o.setid);

	  SET_LEAVE(set, ERROR_UNKNOWN);
	}

      for (i = 0; i < o.u.array.arraysz; i++)
	o.u.array.array.pdata[i] = NULL;
    }
  else
    {
      if ((o.u.array.array.id = malloc(o.u.array.arraysz * sizeof (t_id))) ==
	  NULL)
	{
	  if (!(opts & SET_OPT_CONTAINER))
	    id_release(&set->id, o.setid);

	  SET_LEAVE(set, ERROR_UNKNOWN);
	}

      for (i = 0; i < o.u.array.arraysz; i++)
	o.u.array.array.id[i] = ID_UNUSED;
    }

  /*
   * 5)
   */

  if (set_new(&o) != ERROR_NONE)
    {
      if (opts & SET_OPT_ALLOC)
	free(o.u.array.array.pdata);
      else
	free(o.u.array.array.id);

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

  if (id_release(&set->id, o->setid) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (o->u.array.opts & SET_OPT_ALLOC)
    free(o->u.array.array.pdata);
  else
    free(o->u.array.array.id);

  /*
   * 5)
   */

  if (!(o->u.array.opts & SET_OPT_CONTAINER))
    if (set_delete(o->setid) != ERROR_NONE)
      SET_LEAVE(set, ERROR_UNKNOWN);

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * adds many elements at a time
 */

static void	many_add(t_setid setid, int nb, ...)
{
  va_list	l;
  int		i;
  t_id		id;

  va_start(l, nb);

  for (i = 0; i < nb; ++i)
    {
      id = va_arg(l, t_id);
      if (set_add(setid, &id) != ERROR_NONE)
	cons_msg('!', "error in set_add()\n");
    }

  va_end(l);
}

/*
 * removes many elements at a time
 */

static void	many_remove(t_setid setid, int nb, ...)
{
  va_list	l;
  int		i;
  t_id		id;

  va_start(l, nb);

  for (i = 0; i < nb; ++i)
    {
      id = va_arg(l, t_id);
      if (set_remove(setid, id) != ERROR_NONE)
	cons_msg('!', "error in set_remove()\n");
    }

  va_end(l);
}

/*
 * tests the SET_TYPE_ARRAY
 *
 *
 */
void		set_test_array(void)
{
  t_uint32	nalloc = alloc_nalloc();
  t_uint32	nfree = alloc_nfree();
  t_setid	id;
  t_setid	cloned;
  t_id		data;
  t_id		objs[4];
  t_id		*pdata;
  t_iterator	it;
  t_state	state;

  cons_msg('#', "testing SET_TYPE_ARRAY\n");

  cons_msg('#', "testing with no options\n");

  if (set_reserve(array, 0, 4, 0, &id) != ERROR_NONE)
    cons_msg('!', "error: set_reserve()\n");

  set_show(id);

  many_add(id, 4, 80LL, 98654LL, 42LL, 122LL);

  many_remove(id, 1, 42LL);

  many_add(id, 2, 45LL, 64LL);

  set_show(id);

  set_release(id);

  cons_msg('#', "testing with SET_OPT_ALLOC\n");


  if (set_reserve(array, SET_OPT_ALLOC, 4, sizeof (t_id), &id) != ERROR_NONE)
    cons_msg('!', "error: set_reserve()\n");

  many_add(id, 4, 80LL, 98654LL, 42LL, 122LL);

  many_remove(id, 1, 42LL);

  many_add(id, 2, 45LL, 64LL);

  set_show(id);

  set_release(id);

  cons_msg('#', "testing with SET_OPT_ORGANISE | SET_OPT_ALLOC\n");


  if (set_reserve(array, SET_OPT_ORGANISE | SET_OPT_ALLOC, 1, sizeof (t_id),
		  &id) != ERROR_NONE)
    cons_msg('!', "error: set_reserve()\n");

  many_add(id, 4, 80LL, 98654LL, 42LL, 122LL);

  many_remove(id, 1, 42LL);

  many_add(id, 2, 45LL, 64LL);

  set_show(id);

  cons_msg('#', "testing iterators\n");

  printf("--> ");
  set_foreach(SET_OPT_FORWARD, id, &it, state)
    {
      set_object(id, it, (void **)&pdata);
      printf("%qd ", *pdata);
    }
  printf("\n");

  printf("<-- ");
  set_foreach(SET_OPT_BACKWARD, id, &it, state)
    {
      set_object(id, it, (void **)&pdata);
      printf("%qd ", *pdata);
    }
  printf("\n");

  cons_msg('#', "testing insert_*\n");

  objs[0] = 4LL;
  if (set_insert_head(id, &objs[0]) != ERROR_NONE)
    cons_msg('!', "error in insert_head\n");

  objs[1] = 123456LL;
  if (set_insert_tail(id, &objs[1]) != ERROR_NONE)
    cons_msg('!', "error in insert_tail\n");

  set_head_array(id, &it);
  set_next_array(id, it, &it);
  objs[2] = 456LL;
  if (set_insert_after(id, it, &objs[2]) != ERROR_NONE)
    cons_msg('!', "error in insert_after\n");

  objs[3] = 454LL;
  if (set_insert_before(id, it, &objs[3]) != ERROR_NONE)
    cons_msg('!', "error in insert_before\n");

  set_show(id);

  cons_msg('#', "testing flush\n");

  set_flush(id);

  set_show(id);

  set_release(id);

  cons_msg('#', "testing with SET_OPT_SORT\n");

  if (set_reserve(array, SET_OPT_SORT, 1, sizeof (t_id), &id) != ERROR_NONE)
    cons_msg('!', "error: set_reserve()\n");

  many_add(id, 4, 80LL, 98654LL, 42LL, 122LL);

  many_remove(id, 1, 42LL);

  many_add(id, 2, 45LL, 64LL);

  set_show(id);

  cons_msg('#', "testing clone function\n");

  set_clone(id, &cloned);

  set_release(id);

  set_show(cloned);

  set_release(cloned);

  if ((nalloc - nfree) != (alloc_nalloc() - alloc_nfree()))
    cons_msg('!', "error: memory leaks detected: %u/%u -> %u/%u\n",
	     nalloc, nfree, alloc_nalloc(), alloc_nfree());
  else
    cons_msg('#', "no memory leak detected\n");
}

/*                                                                 [cut] /k2 */
