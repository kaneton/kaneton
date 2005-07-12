/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * set_ll.c
 * 
 * path          /home/mycure/kaneton/core/kaneton/set
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Tue Jul 12 21:22:01 2005   mycure
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
 * XXX
 */

int			set_head_ll(t_setid			setid,
				    t_iterator*			iterator)
{
  o_set*		o;

  set_check(set);

  memset(iterator, 0x0, sizeof(t_iterator));

  if (set_object(setid, &o) != 0)
    return (-1);

  if (o->size == 0)
    return (-1);

  *iterator = o->u.ll.head;

  return (0);
}

/*
 * XXX
 */

int			set_tail_ll(t_setid			setid,
				    t_iterator*			iterator)
{
  o_set*		o;

  set_check(set);

  memset(iterator, 0x0, sizeof(t_iterator));

  if (set_object(setid, &o) != 0)
    return (-1);

  if (o->size == 0)
    return (-1);

  *iterator = o->u.ll.tail;

  return (0);
}

/*
 * XXX
 */

int			set_prev_ll(t_setid			setid,
				    t_iterator			current,
				    t_iterator*			previous)
{
  t_set_ll_node*	c = current;
  o_set*		o;

  set_check(set);

  memset(previous, 0x0, sizeof(t_iterator));

  if (set_object(setid, &o) != 0)
    return (-1);

  if (c->prv == NULL)
    return (-1);

  *previous = c->prv;

  return (0);
}

/*
 * XXX
 */

int			set_next_ll(t_setid			setid,
				    t_iterator			current,
				    t_iterator*			next)
{
  t_set_ll_node*	c = current;
  o_set*		o;

  set_check(set);

  memset(next, 0x0, sizeof(t_iterator));

  if (set_object(setid, &o) != 0)
    return (-1);

  if (c->nxt == NULL)
    return (-1);

  *next = c->nxt;

  return (0);
}

/*
 * XXX
 */

int			set_add_ll(t_setid			setid,
				   void*			data)
{
  t_set_ll_node*	n;
  o_set*		o;

  set_check(set);

  if (set_object(setid, &o) != 0)
    return (-1);

  if ((n = malloc(sizeof(t_set_ll_node))) == NULL)
    return (-1);

  memset(n, 0x0, sizeof(t_set_ll_node));

  if (o->u.ll.opts & SET_OPT_ALLOC)
    {
      if ((n->data = malloc(o->u.ll.datasz)) == NULL)
	{
	  free(n);

	  return (-1);
	}

      memcpy(n->data, data, o->u.ll.datasz);
    }
  else
    n->data = data;

  if (o->u.ll.head != NULL)
    {
      t_set_ll_node	*tmp;

      for (tmp = o->u.ll.head; tmp != NULL; tmp = tmp->nxt)
	{
	  if (*((t_id*)n->data) == *((t_id*)tmp->data))
	    {
	      /* XXX id collision */

	      printf("id collision detected: %qu vs %qu\n",
		     *((t_id*)n->data),
		     *((t_id*)tmp->data));

	      if (o->u.ll.opts & SET_OPT_ALLOC)
		free(n->data);

	      free(n);

	      return (-1);
	    }

	  if (*((t_id*)n->data) < *((t_id*)tmp->data))
	    {
	      n->prv = tmp->prv;
	      n->nxt = tmp;

	      if (n->prv != NULL)
		n->prv->nxt = n;
	      else
		o->u.ll.head = n;

	      if (n->nxt != NULL)
		n->nxt->prv = n;
	      else
		o->u.ll.tail = n;

	      break;
	    }
	}

      if (tmp == NULL)
	{
	  n->prv = o->u.ll.tail;
	  n->nxt = NULL;

	  if (n->prv != NULL)
	    n->prv->nxt = n;
	  else
	    o->u.ll.head = n;

	  o->u.ll.tail = n;
	}
    }
  else
    {
      n->prv = NULL;
      n->nxt = NULL;

      o->u.ll.head = n;
      o->u.ll.tail = n;
    }

  o->size++;

  return (0);
}

/*
 * XXX
 */

int			set_remove_ll(t_setid			setid,
				      t_id			id)
{
  /* XXX */

  return (0);
}

/*
 * XXX
 */

int			set_rsv_ll(t_opts			opts,
				   t_size			datasz,
				   t_setid*			setid)
{
  o_set*		o;

  set_check(set);

  if (set_id_rsv(setid) != 0)
    return (-1);

  if ((o = malloc(sizeof(o_set))) == NULL)
    {
      set_id_rel(*setid);

      return (-1);
    }

  memset(o, 0x0, sizeof(o_set));

  o->id = *setid;
  o->size = 0;
  o->type = SET_TYPE_LL;

  if (set_object_add(o) != 0)
    {
      free(o);
      set_id_rel(*setid);

      return (-1);
    }

  o->u.ll.opts = opts;
  o->u.ll.datasz = datasz;
  o->u.ll.head = NULL;
  o->u.ll.tail = NULL;

  return (0);
}

/*
 * this function returns an object given its iterator.
 */

int			set_get_ll(t_setid			setid,
				   t_iterator			iterator,
				   void**			data)
{
  /* XXX faire des checks supplementaires */

  t_set_ll_node*	n = iterator;

  set_check(set);

  *data = n->data;

  return (0);
}
