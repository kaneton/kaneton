/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * set_array.c
 * 
 * path          /home/mycure/kaneton/core/kaneton/set
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Thu Jul 21 22:25:13 2005   mycure
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

  for (i = 0; i < o->u.array.arraysz; i++)
    if (*((t_id*)(o->u.array.array + (i * o->u.array.datasz))) != ID_UNUSED)
      {
	*iterator = o->u.array.array + (i * o->u.array.datasz);

	return (0);
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

  for (i = o->u.array.arraysz - 1; i >= 0; i++)
    if (*((t_id*)(o->u.array.array + (i * o->u.array.datasz))) != ID_UNUSED)
      {
	*iterator = o->u.array.array + (i * o->u.array.datasz);

	return (0);
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

  for (i = (((t_uint8*)current - o->u.array.array) / o->u.array.datasz) - 1;
       i >= 0; i++)
    if (*((t_id*)(o->u.array.array + (i * o->u.array.datasz))) != ID_UNUSED)
      {
	*previous = o->u.array.array + (i * o->u.array.datasz);

	return (0);
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

  for (i = (((t_uint8*)current - o->u.array.array) / o->u.array.datasz) + 1;
       i < o->u.array.arraysz; i++)
    if (*((t_id*)(o->u.array.array + (i * o->u.array.datasz))) != ID_UNUSED)
      {
	*next = o->u.array.array + (i * o->u.array.datasz);

	return (0);
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

  if (o->u.ll.opts & SET_OPT_SORT)
    return (-1);

  /* XXX */

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

  if (o->u.ll.opts & SET_OPT_SORT)
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

int			set_insert_before_array(t_setid		setid,
						t_iterator	iterator,
						void*		data)
{
  o_set*		o;

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

  if (o->u.ll.opts & SET_OPT_SORT)
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
  o_set*		o;

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

  if (o->u.ll.opts & SET_OPT_SORT)
    return (-1);

  /* XXX */

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
      /* XXX wait for the implementation by pwipwi
      if ((o->u.array.array = realloc(o->u.array.array,
				      (o->u.array.arraysz * 2) *
				      o->u.array.datasz)) == NULL)
	{
	  cons_msg('!', "set: not enough memory to extend the array\n");

	  return (-1);
	}
      */

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
 * 3) cannot release the set container.
 * 4) if needed, releases the set identifier.
 * 5) frees the array allocated at the set reservation.
 * 6) then, removes the set from the set container.
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

  if (setid == set->setid)
    {
      cons_msg('!', "set: cannot release the set container\n");

      return (-1);
    }

  /*
   * 4)
   */

  if (!(o->u.array.opts & SET_OPT_CONTAINER))
    if (id_rel(&set->id, o->id) != 0)
      return (-1);

  /*
   * 5)
   */

  free(o->u.array.array);

  /*
   * 6)
   */

  if (set_delete(o->id) != 0)
    return (-1);

  return (0);
}
