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
 * last update   Wed Sep 28 19:09:42 2005   mycure
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
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the id manager structure.
 */

m_id*			id = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function dumps the id object's state.
 */

t_error			id_dump(o_id*				o)
{
  ID_ENTER(id);

  cons_msg('#', "id object's state: %qu\n", o->id);

  ID_LEAVE(id, ERROR_NONE);
}

/*
 * this function reserves an identifier in an identifier object
 */

t_error			id_rsv(o_id*				o,
			       t_id*				id)
{
  ID_ENTER(id);

  *id = o->id++;

  ID_LEAVE(id, ERROR_NONE);
}

/*
 * this function releases an identifier from an identifier object.
 */

t_error			id_rel(o_id*				o,
			       t_id				id)
{
  ID_ENTER(id);

  ID_LEAVE(id, ERROR_NONE);
}

/*
 * this function initialises an id object.
 */

t_error			id_build(o_id*				o)
{
  ID_ENTER(id);

  memset(o, 0x0, sizeof(o_id));

  ID_LEAVE(id, ERROR_NONE);
}

/*
 * this function cleans an id object.
 */

t_error			id_destroy(o_id*			o)
{
  ID_ENTER(id);

  memset(o, 0x0, sizeof(o_id));

  ID_LEAVE(id, ERROR_NONE);
}

/*
 * this function must initialise the id manager.
 */

t_error			id_init(void)
{
  if ((id = malloc(sizeof(m_id))) == NULL)
    {
      cons_msg('!', "id: cannot allocate memory for the identifier manager "
	       "structure\n");

      return (ERROR_UNKNOWN);
    }

  return (ERROR_NONE);
}

/*
 * this function cleans the id manager.
 */

t_error			id_clean(void)
{
  return (ERROR_NONE);
}
