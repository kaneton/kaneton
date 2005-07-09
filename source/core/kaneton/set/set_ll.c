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
 * last update   Wed Jul  6 17:19:02 2005   mycure
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
  o_set*		o;

#if 1
  printf("[set_ll] adding: 0x%x\n", data);
#endif

  /*
  if ((o = set_get(setid)) != 0)
    return (-1);
  */

}

/*
 * XXX
 */

int			set_rsv_ll(t_opts			opts,
				   t_size			datasz,
				   t_setid*			setid)
{
  o_set*		o;

  if (set_id(setid) != 0)
    return (-1);

  if ((o = malloc(sizeof(o_set))) == NULL)
    return (-1);

  memset(o, 0x0, sizeof(o_set));

  o->setid = *setid;
  o->type = SET_TYPE_LL;

  o->u.ll.opts = opts;
  o->u.ll.datasz = datasz;

  return (0);
}
