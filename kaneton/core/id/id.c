/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/id/id.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [tue feb  6 22:27:40 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the kaneton kernel uses 64-bit identifier.
 *
 * from this fact, we do not care about identifier recycling.
 *
 * the best example of this fact is located in the id_release()
 * function. indeed the function does nothing, meaning that the
 * identifier released will not be recycled.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
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
 * this function shows the id object's state.
 */

t_error			id_show(o_id*				o)
{
  ID_ENTER(id);

  cons_msg('#', "    id object's state: %qu\n", o->id);

  ID_LEAVE(id, ERROR_NONE);
}

/*
 * this function clones an identifier meaning that a new identifier
 * is reserved with same properties.
 */

t_error			id_clone(o_id*				o,
				 t_id				old,
				 t_id*				new)
{
  t_error		r;

  ID_ENTER(id);

  r = id_reserve(o, new);

  ID_LEAVE(id, r);
}

/*
 * this function reserves an identifier in an identifier object
 */

t_error			id_reserve(o_id*			o,
				   t_id*			i)
{
  ID_ENTER(id);

  *i = o->id++;

  ID_LEAVE(id, ERROR_NONE);
}

/*
 * this function releases an identifier from an identifier object.
 */

t_error			id_release(o_id*			o,
				   t_id				i)
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
  free(id);

  return (ERROR_NONE);
}
