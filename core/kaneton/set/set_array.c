/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * set_array.c
 * 
 * path          /home/mycure/kaneton/core/kaneton/stats
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Sun Aug 28 14:03:20 2005   mycure
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
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function dumps set objects contained in a set.
 *
 * steps:
 *
 * 1) checks if the set manager was previously initialised.
 * 2) gets the set descriptor from its identifier.
 * 3) prints each objects' identifier.
 */

int			set_dump_array(t_setid			setid)
{
  o_set*		data;
  o_set*		o;
  t_iterator		i;

  /*
   * 1)
   */

  set_check(set);

  /*
   * 2)
   */

  if (set_descriptor(setid, &o) != 0)
    return (-1);

  /*
   * 3)
   */

  cons_msg('#', "dumping %qu node(s) from the array set %qu:\n",
	   o->size,
	   setid);

  set_foreach(SET_OPT_FORWARD, setid, &i)
    {
      if (set_object(setid, i, (void**)&data) != 0)
	{
	  cons_msg('!', "set: cannot find the object "
		   "corresponding to its identifier\n");

	  return (-1);
	}

      cons_msg('#', "  %qd\n", *data);
    }

  return (0);
}

/*
 * XXX
 */

int			set_expand_array(o_set*			o)
{
  t_sint32		i;

  if (o->size == o->u.array.arraysz)
    {
      if ((o->u.array.array = realloc(o->u.array.array,
				      (o->u.array.arraysz * 2) *
				      o->u.array.datasz)) == NULL)
	{
	  cons_msg('!', "set: not enough memory to expand the array\n");

	  return (-1);
	}

      for (i = o->u.array.arraysz; i < (o->u.array.arraysz * 2); i++)
	*((t_id*)(o->u.array.array + (i * o->u.array.datasz))) = ID_UNUSED;

      o->u.array.arraysz *= 2;
    }

  return (0);
}

/*
 * XXX
 */

int			set_insert_array(o_set*			o,
					 t_sint32		position)
{
  /* XXX */

  return (0);
}

/*
 * this function returns an iterator on the first entry on the array.
 *
 * if there is no node in the list, the function returns -1.
 */

int			set_head_array(t_setid			setid,
				       t_iterator*		iterator)
{
  o_set*		o;
  t_sint32		i;

  set_check(set);

  memset(iterator, 0x0, sizeof(t_iterator));

  if (set_descriptor(setid, &o) != 0)
    return (-1);

  if (o->size == 0)
    return (-1);

  if (o->u.array.opts & SET_OPT_ORGANISE)
    {
      *iterator = o->u.array.array;

      return (0);
    }
  else
    {
      for (i = 0; i < o->u.array.arraysz; i++)
	if (*((t_id*)(o->u.array.array + (i * o->u.array.datasz))) !=
	    ID_UNUSED)
	  {
	    *iterator = o->u.array.array + (i * o->u.array.datasz);

	    return (0);
	  }
    }

  return (-1);
}

/*
 * this function returns an iterator on the last entry of the array.
 *
 * if there is no node in the list, the function returns -1.
 */

int			set_tail_array(t_setid			setid,
				       t_iterator*		iterator)
{
  o_set*		o;
  t_sint32		i;

  set_check(set);

  memset(iterator, 0x0, sizeof(t_iterator));

  if (set_descriptor(setid, &o) != 0)
    return (-1);

  if (o->size == 0)
    return (-1);

  if (o->u.array.opts & SET_OPT_ORGANISE)
    {
      *iterator = o->u.array.array + ((o->size - 1) * o->u.array.datasz);

      return (0);
    }
  else
    {
      for (i = o->u.array.arraysz - 1; i >= 0; i--)
	if (*((t_id*)(o->u.array.array + (i * o->u.array.datasz))) !=
	    ID_UNUSED)
	  {
	    *iterator = o->u.array.array + (i * o->u.array.datasz);

	    return (0);
	  }
    }

  return (-1);
}

/*
 * this function returns an iterator on the previous entry.
 */

int			set_prev_array(t_setid			setid,
				       t_iterator		current,
				       t_iterator*		previous)
{
  o_set*		o;
  t_sint32		i;

  set_check(set);

  memset(previous, 0x0, sizeof(t_iterator));

  if (set_descriptor(setid, &o) != 0)
    return (-1);

  i = (((t_uint8*)current - o->u.array.array) / o->u.array.datasz) - 1;

  if (o->u.array.opts & SET_OPT_ORGANISE)
    {
      if (i >= 0)
	{
	  *previous = o->u.array.array + (i * o->u.array.datasz);

	  return (0);
	}
    }
  else
    {
      for (; i >= 0; i--)
	if (*((t_id*)(o->u.array.array + (i * o->u.array.datasz))) !=
	    ID_UNUSED)
	  {
	    *previous = o->u.array.array + (i * o->u.array.datasz);

	    return (0);
	  }
    }

  return (-1);
}

/*
 * this function returns an iterator on the next entry of the array.
 */

int			set_next_array(t_setid			setid,
				       t_iterator		current,
				       t_iterator*		next)
{
  o_set*		o;
  t_sint32		i;

  set_check(set);

  memset(next, 0x0, sizeof(t_iterator));

  if (set_descriptor(setid, &o) != 0)
    return (-1);

  i = (((t_uint8*)current - o->u.array.array) / o->u.array.datasz) + 1;

  if (o->u.array.opts & SET_OPT_ORGANISE)
    {
      if (i < o->size)
	{
	  *next = o->u.array.array + (i * o->u.array.datasz);

	  return (0);
	}
    }
  else
    {
      for (; i < o->u.array.arraysz; i++)
	if (*((t_id*)(o->u.array.array + (i * o->u.array.datasz))) !=
	    ID_UNUSED)
	  {
	    *next = o->u.array.array + (i * o->u.array.datasz);

	    return (0);
	  }
    }

  return (-1);
}

/*
 * this function inserts a new entry at the head of the array.
 *
 * steps:
 *
 * 1) XXX
 */

int			set_insert_head_array(t_setid		setid,
					      void*		data)
{
  o_set*		o;
  t_sint32		i;

  /*
   * 1)
   */

  set_check(set);

  /*
   * 2)
   */

  if (*((t_id*)data) == ID_UNUSED)
    return (-1);

  /*
   * 3)
   */

  if (set_descriptor(setid, &o) != 0)
    return (-1);

  /*
   * 4)
   */

  if (o->u.array.opts & SET_OPT_SORT)
    return (-1);

  /*
   * 5)
   */

  if (set_expand_array(o) != 0)
    return (-1);

  /*
   * 6)
   */

  if (set_insert_array(o, 0) != 0)
    return (-1);

  /*
   * 8)
   */

  o->size++;

  return (0);
}

/*
 * this function inserts a new entry at the tail of the array.
 *
 * steps:
 *
 * 1) XXX
 */

int			set_insert_tail_array(t_setid		setid,
					      void*		data)
{
  o_set*		o;
  t_sint32		i;

  /*
   * 1)
   */

  set_check(set);

  /*
   * 2)
   */

  if (*((t_id*)data) == ID_UNUSED)
    return (-1);

  /*
   * 3)
   */

  if (set_descriptor(setid, &o) != 0)
    return (-1);

  /*
   * 4)
   */

  if (o->u.array.opts & SET_OPT_SORT)
    return (-1);

  /*
   * 5)
   */

  if (set_expand_array(o) != 0)
    return (-1);

  /*
   * 6)
   */

  if (o->u.array.opts & SET_OPT_ORGANISE)
    i = o->size;
  else
    i = o->u.array.arraysz - 1;

  /*
   * 7)
   */

  if (set_insert_array(o, i) != 0)
    return (-1);

  /*
   * 8)
   */

  o->size++;

  return (0);
}

/*
 * XXX
 */

int			set_insert_before_array(t_setid		setid,
						t_iterator	iterator,
						void*		data)
{
  o_set*		o;
  t_sint32		i;

  /*
   * 1)
   */

  set_check(set);

  /*
   * 2)
   */

  if (*((t_id*)data) == ID_UNUSED)
    return (-1);

  /*
   * 3)
   */

  if (set_descriptor(setid, &o) != 0)
    return (-1);

  /*
   * 4)
   */

  if (o->u.array.opts & SET_OPT_SORT)
    return (-1);

  /* XXX */

  /*
   * 8)
   */

  o->size++;

  return (0);
}

/*
 * XXX
 */

int			set_insert_after_array(t_setid		setid,
					       t_iterator	iterator,
					       void*		data)
{
  t_uint8*		n;
  o_set*		o;
  t_sint32		i;
  t_sint32		j;

  /*
   * 1)
   */

  set_check(set);
 
  /*
   * 2)
   */

  if (*((t_id*)data) == ID_UNUSED)
    return (-1);

 /*
   * 3)
   */

  if (set_descriptor(setid, &o) != 0)
    return (-1);

  /*
   * 4)
   */

  if (o->u.array.opts & SET_OPT_SORT)
    return (-1);

  /*
   * 5)
   */

  if (set_expand_array(o) != 0)
    return (-1);

  /*
   * 6)
   */

  n = (t_uint8*)iterator + o->u.array.datasz;

  /*
   * 7)
   */

  if (*((t_id*)n) == ID_UNUSED)
    {
      memcpy(n, data, o->u.array.datasz);
    }
  else
    {
      if (o->u.array.opts & SET_OPT_ORGANISE)
	{
	  i = o->size;

	  /* XXX decalage droite >> */
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

  /*
   * 8)
   */

  o->size++;

  return (0);
}

/*
 * this function adds an object in the array.
 *
 * steps:
 *
 * 1) XXX
 * 6) increments the number of nodes in the list.
 */

int			set_add_array(t_setid			setid,
				      void*			data)
{
  o_set*		o;
  t_sint32		i;
  t_sint32		j;

  /*
   * 1)
   */

  set_check(set);

  /*
   * 2)
   */

  if (*((t_id*)data) == ID_UNUSED)
    return (-1);

  /*
   * 3)
   */

  if (set_descriptor(setid, &o) != 0)
    return (-1);

  /*
   * 4)
   */

  if (o->size == o->u.array.arraysz)
    {
      if ((o->u.array.array = realloc(o->u.array.array,
				      (o->u.array.arraysz * 2) *
				      o->u.array.datasz)) == NULL)
	{
	  cons_msg('!', "set: not enough memory to expand the array\n");

	  return (-1);
	}

      for (i = o->u.array.arraysz; i < (o->u.array.arraysz * 2); i++)
	*((t_id*)(o->u.array.array + (i * o->u.array.datasz))) = ID_UNUSED;

      o->u.array.arraysz *= 2;
    }

  /*
   * 5)
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

	      return (-1);
	    }
	}
    }

  /*
   * 6)
   */

  o->size++;

  return (0);
}

/*
 * this function removes a object from the array, setting the identifier
 * field of the entry as 'unused'.
 *
 * steps:
 *
 * 1) XXX
 */

int			set_remove_array(t_setid		setid,
					 t_id			id)
{
  /* XXX */

  return (0);
}

/*
 * XXX
 */

int			set_flush_array(t_setid			setid)
{
  /* XXX */

  return (0);
}

/*
 * this function tries to find an object with its identifier and build
 * a corresponding identifier.
 *
 * steps:
 *
 * 1) XXX
 */

int			set_locate_array(t_setid		setid,
					 t_id			id,
					 t_iterator*		iterator)
{
  /* XXX */

  return (-1);
}

/*
 * this function returns an object given its iterator.
 */

int			set_object_array(t_setid		setid,
					 t_iterator		iterator,
					 void**			data)
{
  set_check(set);

  *data = iterator;

  return (0);
}

/*
 * this function reserves a set.
 *
 * steps:
 *
 * 1) checks whether the set manager was previously initialised.
 * 2) the array sets only works with the option alloc.
 * 3) initialises the set descriptor.
 * 4) if necessary, reserves an unused identifier for this new set.
 * 5) initialises the set fields allocating the array.
 * 6) adds the set descriptor in the set container.
 */

int			set_rsv_array(t_opts			opts,
				      t_setsz			initsz,
				      t_size			datasz,
				      t_setid*			setid)
{
  o_set			o;
  t_setsz		i;

  /*
   * 1)
   */

  set_check(set);

  /*
   * 2)
   */

  if (!(opts & SET_OPT_ALLOC))
    return (-1);

  /*
   * 3)
   */

  memset(&o, 0x0, sizeof(o_set));

  /*
   * 4)
   */

  if (opts & SET_OPT_CONTAINER)
    {
      *setid = set->setid;
    }
  else
    {
      if (id_rsv(&set->id, setid) != 0)
	return (-1);
    }

  /*
   * 5)
   */

  o.id = *setid;
  o.size = 0;
  o.type = SET_TYPE_ARRAY;

  o.u.array.opts = opts;
  o.u.array.datasz = datasz;
  o.u.array.arraysz = initsz == 0 ? 1 : initsz;

  if ((o.u.array.array = malloc(o.u.array.arraysz * o.u.array.datasz)) == NULL)
    {
      if (!(opts & SET_OPT_CONTAINER))
	id_rel(&set->id, o.id);

      return (-1);
    }

  memset(o.u.array.array, 0x0, o.u.array.arraysz * o.u.array.datasz);

  for (i = 0; i < o.u.array.arraysz; i++)
    *((t_id*)(o.u.array.array + (i * o.u.array.datasz))) = ID_UNUSED;

  /*
   * 6)
   */

  if (set_new(&o) != 0)
    {
      free(o.u.array.array);

      if (!(opts & SET_OPT_CONTAINER))
	id_rel(&set->id, o.id);

      return (-1);
    }

  return (0);
}

/*
 * this function releases a set.
 *
 * steps:
 *
 * 1) checks whether the set manager was previously initialised.
 * 2) gets the set given its set identifier.
 * 3) flushs the set.
 * 4) cannot release the set container.
 * 5) if needed, releases the set identifier.
 * 6) frees the array allocated at the set reservation.
 * 7) then, removes the set from the set container.
 */

int			set_rel_array(t_setid			setid)
{
  o_set			*o;

  /*
   * 1)
   */

  set_check(set);

  /*
   * 2)
   */

  if (set_descriptor(setid, &o) != 0)
    return (-1);

  /*
   * 3)
   */

  if (set_flush(setid) != 0)
    return (-1);

  /*
   * 4)
   */

  if (setid == set->setid)
    {
      cons_msg('!', "set: cannot release the set container\n");

      return (-1);
    }

  /*
   * 5)
   */

  if (!(o->u.array.opts & SET_OPT_CONTAINER))
    if (id_rel(&set->id, o->id) != 0)
      return (-1);

  /*
   * 6)
   */

  free(o->u.array.array);

  /*
   * 7)
   */

  if (set_delete(o->id) != 0)
    return (-1);

  return (0);
}
