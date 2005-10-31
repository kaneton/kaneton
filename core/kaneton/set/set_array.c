/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * set_array.c
 * 
 * path          /home/mycure/kaneton/core/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Sun Oct 30 14:13:29 2005   mycure
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX
 *
 * XXX notez qu il n est pas possible de construire un tableau ou l on
 *     passe des objets deja alloues et donc que le tableau ne contiennent
 *     que des pointeurs vers des object alloues. pour faire cela,
 *     l utilisateur doit le faire lui meme en construisant un tableau
 *     de taille sizeof(void*) (pointeur).
 *
 * XXX en effet le but de cette structure de donnes et de contenir en elle
 *     meme les donnees, generalement des petites donnees comme des
 *     identfificateurs.
 *
 * XXX ca foire le truc: il ne faut pas faire une erreur si jamais il n'y
 *     a pas de place pour inserer apres par exemple. il faut se demerder
 *     pour caser le truc, par exemple en allouant de la place.
 *
 * XXX option ORGANISE pour toujours faire en sorte que le tableau soit
 *     le plus petit possible.
 *
 * options: SET_OPT_CONTAINER, SET_OPT_SORT, SET_OPT_ORGANISE
 * XXX SET_OPT_ALLOC ?
 *
 * XXX set_insert_array(set, position)
 *
 * t_sint32 -> t_setsz
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
 * this function shows set objects contained in a set.
 *
 * steps:
 *
 * 1) gets the set descriptor from its identifier.
 * 2) prints each objects' identifier.
 */

t_error			set_show_array(t_setid			setid)
{
  t_state		state;
  o_set*		data;
  o_set*		o;
  t_iterator		i;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != 0)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  cons_msg('#', "showing %qd node(s) from the array set %qu:\n",
	   o->size,
	   setid);

  set_foreach(SET_OPT_FORWARD, setid, &i, state)
    {
      if (set_object(setid, i, (void**)&data) != ERROR_NONE)
	{
	  cons_msg('!', "set: cannot find the object "
		   "corresponding to its identifier\n");

	  SET_LEAVE(set, ERROR_UNKNOWN);
	}

      cons_msg('#', "  %qd\n", *data);
    }

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * XXX
 */

t_error			set_expand_array(o_set*			o)
{
  t_sint32		i;

  SET_ENTER(set);

  if (o->size == o->u.array.arraysz)
    {
      if ((o->u.array.array = realloc(o->u.array.array,
				      (o->u.array.arraysz * 2) *
				      o->u.array.datasz)) == NULL)
	{
	  cons_msg('!', "set: not enough memory to expand the array\n");

	  SET_LEAVE(set, ERROR_UNKNOWN);
	}

      for (i = o->u.array.arraysz; i < (o->u.array.arraysz * 2); i++)
	*((t_id*)(o->u.array.array + (i * o->u.array.datasz))) = ID_UNUSED;

      o->u.array.arraysz *= 2;
    }

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * XXX
 */

t_error			set_insert_array(o_set*			o,
					 t_sint32		position)
{
  SET_ENTER(set);

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function returns an iterator on the first entry on the array.
 *
 * if there is no node in the list, the function returns ERROR_UNKNOWN.
 */

t_error			set_head_array(t_setid			setid,
				       t_iterator*		iterator)
{
  /* XXX
  o_set*		o;
  t_sint32		i;

  SET_ENTER(set);

  memset(iterator, 0x0, sizeof(t_iterator));

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  if (o->size == 0)
    SET_LEAVE(set, ERROR_UNKNOWN);

  if (o->u.array.opts & SET_OPT_ORGANISE)
    {
      *iterator = o->u.array.array;

      SET_LEAVE(set, ERROR_NONE);
    }
  else
    {
      for (i = 0; i < o->u.array.arraysz; i++)
	if (*((t_id*)(o->u.array.array + (i * o->u.array.datasz))) !=
	    ID_UNUSED)
	  {
	    *iterator = o->u.array.array + (i * o->u.array.datasz);

	    SET_LEAVE(set, ERROR_NONE);
	  }
    }

  SET_LEAVE(set, ERROR_UNKNOWN);
  */
}

/*
 * this function returns an iterator on the last entry of the array.
 *
 * if there is no node in the list, the function returns ERROR_UNKNOWN.
 */

t_error			set_tail_array(t_setid			setid,
				       t_iterator*		iterator)
{
  /* XXX
  o_set*		o;
  t_sint32		i;

  SET_ENTER(set);

  memset(iterator, 0x0, sizeof(t_iterator));

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  if (o->size == 0)
    SET_LEAVE(set, ERROR_UNKNOWN);

  if (o->u.array.opts & SET_OPT_ORGANISE)
    {
      *iterator = o->u.array.array + ((o->size - 1) * o->u.array.datasz);

      SET_LEAVE(set, ERROR_NONE);
    }
  else
    {
      for (i = o->u.array.arraysz - 1; i >= 0; i--)
	if (*((t_id*)(o->u.array.array + (i * o->u.array.datasz))) !=
	    ID_UNUSED)
	  {
	    *iterator = o->u.array.array + (i * o->u.array.datasz);

	    SET_LEAVE(set, ERROR_NONE);
	  }
    }

  SET_LEAVE(set, ERROR_UNKNOWN);
  */
}

/*
 * this function returns an iterator on the previous entry.
 */

t_error			set_prev_array(t_setid			setid,
				       t_iterator		current,
				       t_iterator*		previous)
{
  /* XXX
  o_set*		o;
  t_sint32		i;

  SET_ENTER(set);

  memset(previous, 0x0, sizeof(t_iterator));

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  i = (((t_uint8*)current - o->u.array.array) / o->u.array.datasz) - 1;

  if (o->u.array.opts & SET_OPT_ORGANISE)
    {
      if (i >= 0)
	{
	  *previous = o->u.array.array + (i * o->u.array.datasz);

	  SET_LEAVE(set, ERROR_NONE);
	}
    }
  else
    {
      for (; i >= 0; i--)
	if (*((t_id*)(o->u.array.array + (i * o->u.array.datasz))) !=
	    ID_UNUSED)
	  {
	    *previous = o->u.array.array + (i * o->u.array.datasz);

	    SET_LEAVE(set, ERROR_NONE);
	  }
    }

  SET_LEAVE(set, ERROR_UNKNOWN);
  */
}

/*
 * this function returns an iterator on the next entry of the array.
 */

t_error			set_next_array(t_setid			setid,
				       t_iterator		current,
				       t_iterator*		next)
{
  /* XXX
  o_set*		o;
  t_sint32		i;

  SET_ENTER(set);

  memset(next, 0x0, sizeof(t_iterator));

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  i = (((t_uint8*)current - o->u.array.array) / o->u.array.datasz) + 1;

  if (o->u.array.opts & SET_OPT_ORGANISE)
    {
      if (i < o->size)
	{
	  *next = o->u.array.array + (i * o->u.array.datasz);

	  SET_LEAVE(set, ERROR_NONE);
	}
    }
  else
    {
      for (; i < o->u.array.arraysz; i++)
	if (*((t_id*)(o->u.array.array + (i * o->u.array.datasz))) !=
	    ID_UNUSED)
	  {
	    *next = o->u.array.array + (i * o->u.array.datasz);

	    SET_LEAVE(set, ERROR_NONE);
	  }
    }

  SET_LEAVE(set, ERROR_UNKNOWN);
  */
}

/*
 * this function inserts a new entry at the head of the array.
 *
 * steps:
 *
 * 1) XXX
 */

t_error			set_insert_head_array(t_setid		setid,
					      void*		data)
{
  o_set*		o;
  t_sint32		i;

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

  if (o->u.array.opts & SET_OPT_SORT)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (set_expand_array(o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 5)
   */

  if (set_insert_array(o, 0) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 6)
   */

  o->size++;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function inserts a new entry at the tail of the array.
 *
 * steps:
 *
 * 1) XXX
 */

t_error			set_insert_tail_array(t_setid		setid,
					      void*		data)
{
  o_set*		o;
  t_sint32		i;

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

  if (o->u.array.opts & SET_OPT_SORT)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (set_expand_array(o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 5)
   */

  if (o->u.array.opts & SET_OPT_ORGANISE)
    i = o->size;
  else
    i = o->u.array.arraysz - 1;

  /*
   * 6)
   */

  if (set_insert_array(o, i) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 7)
   */

  o->size++;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * XXX
 */

t_error			set_insert_before_array(t_setid		setid,
						t_iterator	iterator,
						void*		data)
{
  /* XXX
  o_set*		o;
  t_sint32		i;

  SET_ENTER(set);

  if (*((t_id*)data) == ID_UNUSED)
    SET_LEAVE(set, ERROR_UNKNOWN);

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  if (o->u.array.opts & SET_OPT_SORT)
    SET_LEAVE(set, ERROR_UNKNOWN);

  o->size++;

  SET_LEAVE(set, ERROR_NONE);
  */
}

/*
 * XXX
 */

t_error			set_insert_after_array(t_setid		setid,
					       t_iterator	iterator,
					       void*		data)
{
  /* XXX
  t_uint8*		n;
  o_set*		o;
  t_sint32		i;
  t_sint32		j;

  SET_ENTER(set);

  if (*((t_id*)data) == ID_UNUSED)
    SET_LEAVE(set, ERROR_UNKNOWN);

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  if (o->u.array.opts & SET_OPT_SORT)
    SET_LEAVE(set, ERROR_UNKNOWN);

  if (set_expand_array(o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  n = (t_uint8*)iterator + o->u.array.datasz;

  if (*((t_id*)n) == ID_UNUSED)
    {
      memcpy(n, data, o->u.array.datasz);
    }
  else
    {
      if (o->u.array.opts & SET_OPT_ORGANISE)
	{
	  i = o->size;

	  // XXX decalage droite >>
	}
      else
	{
	  for (i = ((n - o->u.array.array) / o->u.array.datasz) + 1;
	       i < o->u.array.arraysz; i++)
	    {
	      if (*((t_id*)(o->u.array.array + (i * o->u.array.datasz))) ==
		  ID_UNUSED)
		{
		  for (j = o->u.array.arraysz - 1; j >= i; j--)
		    {
		      memcpy(o->u.array.array + ((j + 1) * o->u.array.datasz),
			     o->u.array.array + (j * o->u.array.datasz),
			     o->u.array.datasz);
		    }

		  break;
		}
	    }

	  if (i == o->u.array.arraysz)
	    {
	      for (i = ((n - o->u.array.array) / o->u.array.datasz) - 1;
		   i < o->u.array.arraysz; i++)
		{
		  if (*((t_id*)(o->u.array.array + (i * o->u.array.datasz))) ==
		      ID_UNUSED)
		    {
		      for (j = o->u.array.arraysz - 1; j >= i; j--)
			{
			  memcpy(o->u.array.array +
				 ((j + 1) * o->u.array.datasz),
				 o->u.array.array + (j * o->u.array.datasz),
				 o->u.array.datasz);
			}
		    }
		}
	    }
	}
    }

  o->size++;

  SET_LEAVE(set, ERROR_NONE);
  */
}

/*
 * this function adds an object in the array.
 *
 * steps:
 *
 * 1) XXX
 * 6) increments the number of nodes in the list.
 */

t_error			set_add_array(t_setid			setid,
				      void*			data)
{
  o_set*		o;
  t_sint32		i;
  t_sint32		j;

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

  if (o->size == o->u.array.arraysz)
    {
      if ((o->u.array.array = realloc(o->u.array.array,
				      (o->u.array.arraysz * 2) *
				      o->u.array.datasz)) == NULL)
	{
	  cons_msg('!', "set: not enough memory to expand the array\n");

	  SET_LEAVE(set, ERROR_UNKNOWN);
	}

      for (i = o->u.array.arraysz; i < (o->u.array.arraysz * 2); i++)
	*((t_id*)(o->u.array.array + (i * o->u.array.datasz))) = ID_UNUSED;

      o->u.array.arraysz *= 2;
    }

  /*
   * 4)
   */

  if (o->u.array.opts & SET_OPT_SORT)
    {
      /*
       * A)
       */

      for (i = 0; i < o->size; i++)
	{
	  if (*((t_id*)(o->u.array.array + (i * o->u.array.datasz))) >
	      *((t_id*)data))
	    {
	      for (j = o->size - 1; j >= i; j--)
		{
		  memcpy(o->u.array.array + ((j + 1) * o->u.array.datasz),
			 o->u.array.array + (j * o->u.array.datasz),
			 o->u.array.datasz);
		}

	      memcpy(o->u.array.array + (i * o->u.array.datasz),
		     data, o->u.array.datasz);

	      break;
	    }
	}

      if (i == o->size)
	{
	  memcpy(o->u.array.array + (o->size * o->u.array.datasz),
		 data, o->u.array.datasz);
	}
    }
  else
    {
      /*
       * B)
       */

      for (i = 0; i < o->u.array.arraysz; i++)
	{
	  if (*((t_id*)(o->u.array.array + (i * o->u.array.datasz))) ==
	      ID_UNUSED)
	    {
	      memcpy(o->u.array.array + (i * o->u.array.datasz),
		     data, o->u.array.datasz);

	      break;
	    }

	  if (i == o->u.array.arraysz)
	    {
	      cons_msg('!', "set: unreachable code reached\n");

	      SET_LEAVE(set, ERROR_UNKNOWN);
	    }
	}
    }

  /*
   * 5)
   */

  o->size++;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function removes a object from the array, setting the identifier
 * field of the entry as 'unused'.
 *
 * steps:
 *
 * 1) XXX
 */

t_error			set_remove_array(t_setid		setid,
					 t_id			id)
{
  SET_ENTER(set);

  /* XXX */

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * XXX
 */

t_error			set_flush_array(t_setid			setid)
{
  SET_ENTER(set);

  /* XXX */

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function tries to find an object with its identifier and build
 * a corresponding identifier.
 *
 * steps:
 *
 * 1) XXX
 */

t_error			set_locate_array(t_setid		setid,
					 t_id			id,
					 t_iterator*		iterator)
{
  SET_ENTER(set);

  /* XXX */

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function returns an object given its iterator.
 */

t_error			set_object_array(t_setid		setid,
					 t_iterator		iterator,
					 void**			data)
{
  SET_ENTER(set);

  /* XXX
  *data = iterator;
  */

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function clones a set.
 *
 * steps:
 *
 * 1) XXX
 */

t_error			set_clone_array(t_setid			old,
					t_setid*		new)
{
  /* XXX */
}

/*
 * this function reserves a set.
 *
 * steps:
 *
 * 1) the array sets only works with the option alloc.
 * 2) initialises the set descriptor.
 * 3) if necessary, reserves an unused identifier for this new set.
 * 4) initialises the set fields allocating the array.
 * 5) adds the set descriptor in the set container.
 */

t_error			set_rsv_array(t_opts			opts,
				      t_setsz			initsz,
				      t_size			datasz,
				      t_setid*			setid)
{
  o_set			o;
  t_setsz		i;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (!(opts & SET_OPT_ALLOC))
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
      if (id_rsv(&set->id, setid) != ERROR_NONE)
	SET_LEAVE(set, ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  o.setid = *setid;
  o.size = 0;
  o.type = SET_TYPE_ARRAY;

  o.u.array.opts = opts;
  o.u.array.datasz = datasz;
  o.u.array.arraysz = initsz == 0 ? 1 : initsz;

  if ((o.u.array.array = malloc(o.u.array.arraysz * o.u.array.datasz)) == NULL)
    {
      if (!(opts & SET_OPT_CONTAINER))
	id_rel(&set->id, o.setid);

      SET_LEAVE(set, ERROR_UNKNOWN);
    }

  memset(o.u.array.array, 0x0, o.u.array.arraysz * o.u.array.datasz);

  for (i = 0; i < o.u.array.arraysz; i++)
    *((t_id*)(o.u.array.array + (i * o.u.array.datasz))) = ID_UNUSED;

  /*
   * 5)
   */

  if (set_new(&o) != ERROR_NONE)
    {
      free(o.u.array.array);

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
 * 2) flushs the set.
 * 3) releases the set identifier.
 * 4) frees the array allocated at the set reservation.
 * 5) then, removes the set from the set container.
 */

t_error			set_rel_array(t_setid			setid)
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

  if (id_rel(&set->id, o->setid) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 4)
   */

  free(o->u.array.array);

  /*
   * 5)
   */

  if (!(o->u.array.opts & SET_OPT_CONTAINER))
    if (set_delete(o->setid) != ERROR_NONE)
      SET_LEAVE(set, ERROR_UNKNOWN);

  SET_LEAVE(set, ERROR_NONE);
}

/*                                                                 [cut] /k2 */
