/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * id.c
 * 
 * path          /home/mycure/kaneton/core/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Fri Jan 29 05:06:25 1999   mycure
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the kaneton kernel uses 64-bit identifier.
 *
 * from this fact, we do not care about identifier recycling.
 *
 * the best example of this fact is located in the id_rel() function. indeed
 * the function does nothing, meaning that the identifier released will not
 * be recycled.
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
 * this function reserves an identifier in an identifier object
 */

int			id_rsv(o_id*				o,
			       t_id*				id)
{
  *id = o->id++;

  return (0);
}

/*
 * this function releases an identifier from an identifier object.
 */

int			id_rel(o_id*				o,
			       t_id				id)
{
  return (0);
}

/*
 * this function initialises an id object.
 */

int			id_build(o_id*				o)
{
  memset(o, 0x0, sizeof(o_id));

  return (0);
}

/*
 * this function cleans an id object.
 */

int			id_destroy(o_id*			o)
{
  memset(o, 0x0, sizeof(o_id));

  return (0);
}

/*
 * this function must initialise the id manager.
 *
 * nothing to do here but return.
 */

int			id_init(void)
{
  return (0);
}

/*
 * this function cleans the id manager.
 *
 * nothing to do here but return.
 */

int			id_clean(void)
{
  return (0);
}
