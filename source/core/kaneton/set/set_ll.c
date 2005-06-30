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
 * last update   Mon Jun 20 23:24:01 2005   mycure
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

int			set_add_ll(t_setid			setid,
				   void*			data)
{
  printf("[ll] adding: 0x%x\n", data);
}

/*
 * XXX
 */

int			set_make_ll(o_set*			o,
				    t_setid			setid,
				    t_set_alloc_fn		alloc,
				    t_set_free_fn		free)
{
  memset(o, 0x0, sizeof(o_set));

  o->setid = setid;
  o->type = SET_TYPE_LL;
  o->alloc = alloc;
  o->free = free;

  return (0);
}

/*
 * XXX
 */

int			set_rsv_ll(t_setid*			setid)
{
  set_id(setid);

  /* set_make(ll, *setid, ... */

  return (0);
}
