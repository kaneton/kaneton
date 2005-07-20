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
 * last update   Wed Jul 20 21:26:52 2005   mycure
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
  /*
  o_set*		o;

  set_check(set);

  memset(iterator, 0x0, sizeof(t_iterator));

  if (set_descriptor(setid, &o) != 0)
    return (-1);

  if (o->size == 0)
    return (-1);

  *iterator = o->u.ll.head;
  */
  return (0);
}

/*
 * this function returns an iterator on the last entry of the array.
 *
 * if there is no node in the list, the function returns -1.
 */

int			set_tail_array(t_setid			setid,
				       t_iterator*		iterator)
{
  /*
  o_set*		o;

  set_check(set);

  memset(iterator, 0x0, sizeof(t_iterator));

  if (set_descriptor(setid, &o) != 0)
    return (-1);

  if (o->size == 0)
    return (-1);

  *iterator = o->u.ll.tail;
  */
  return (0);
}

/*
 * this function returns an iterator on the previous entry.
 */

int			set_prev_array(t_setid			setid,
				       t_iterator		current,
				       t_iterator*		previous)
{
  /*
  t_set_array_node*	c = current;
  o_set*		o;

  set_check(set);

  memset(previous, 0x0, sizeof(t_iterator));

  if (set_descriptor(setid, &o) != 0)
    return (-1);

  if (c->prv == NULL)
    return (-1);

  *previous = c->prv;
  */
  return (0);
}

/*
 * this function returns an iterator on the next entry of the array.
 */

int			set_next_array(t_setid			setid,
				       t_iterator		current,
				       t_iterator*		next)
{
  /*
  t_set_array_node*	c = current;
  o_set*		o;

  set_check(set);

  memset(next, 0x0, sizeof(t_iterator));

  if (set_descriptor(setid, &o) != 0)
    return (-1);

  if (c->nxt == NULL)
    return (-1);

  *next = c->nxt;
  */
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
  /*
   * 1)
   */

  set_check(set);

  /* XXX */

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

int			set_get_array(t_setid			setid,
				      t_iterator		iterator,
				      void**		data)
{
  t_set_array_node*	n = iterator;

  set_check(set);

  /* XXX *data = n->data; */

  return (0);
}

/*
 * this function reserves a set.
 *
 * steps:
 *
 * 1) checks whether the set manager was previously initialized.
 * 2) initializes the set descriptor.
 * 3) if necessary, reserves an unused identifier for this new set.
 * 4) XXX
 * 5) adds the set descriptor to the set container.
 */

int			set_rsv_array(t_size			datasz,
				      t_setid*			setid)
{
  o_set			o;

  /*
   * 1)
   */

  set_check(set);

  /*
   * 2)
   */

  memset(&o, 0x0, sizeof(o_set));

  /*
   * 3)
   */

  if (opts & SET_OPT_CONTAINER)
    {
      *setid = set->contid;
    }
  else
    {
      if (id_rsv(&set->id, setid) != 0)
	return (-1);
    }

  /*
   * 4)
   */

  /* XXX */

  /*
   * 5)
   */

  if (set_new(&o) != 0)
    {
      id_rel(&set->id, *setid);

      return (-1);
    }

  return (0);
}

/*
 * this function releases a set.
 *
 * steps:
 *
 * 1) XXX
 */

int			set_rel_array(t_setid			setid)
{
  /* XXX */

  return (0);
}
