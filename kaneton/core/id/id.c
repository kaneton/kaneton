/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/id/id.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       julien quintard   [sun jan 30 20:06:28 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the identifier manager provides functions for managing identifier objects
 * which are capable to generate identifiers used to identify the
 * different kaneton objects.
 *
 * note that the kaneton kernel uses 64-bit identifier and that every
 * manager relies on its specific identifier object meaning that the
 * identifier, say 42, can be used for a task, a thread, a segment and so
 * forth. therefore an identifier alone is meaningless as it must always
 * be known which manager it belongs to.
 *
 * the use of manager-specific identifier objects and 64-bit identifiers
 * imply that identifiers do not need to be recycled.
 *
 * finally, note that unlike other managers, the identifier manager does
 * not rely on an hypothetical machine complementary implementation since
 * this manager is at the heart of the kaneton microkernel.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the identifier manager structure.
 */

m_id			_id;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function shows the given object's attributes and state.
 */

t_status		id_show(o_id*				object,
				mt_margin			margin)
{
  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "identifier: id(%qd)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      object->id);

  CORE_LEAVE();
}

/*
 * this function generates an identifier from the given identifier object.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) generate the identifier.
 */

t_status		id_reserve(o_id*			object,
				   t_id*			id)
{
  /*
   * 0)
   */

  if (object == NULL)
    CORE_ESCAPE("the 'object' argument is null");

  if (id == NULL)
    CORE_ESCAPE("the 'id' argument is null");

  /*
   * 1)
   */

  *id = object->id++;

  CORE_LEAVE();
}

/*
 * this function releases an identifier from an identifier object.
 *
 * note that since the identifiers are not recycled, there is nothing to
 * do here.
 *
 * steps:
 *
 * 0) verify the arguments.
 */

t_status		id_release(o_id*			object,
				   t_id				id)
{
  /*
   * 0)
   */

  if (object == NULL)
    CORE_ESCAPE("the 'object' argument is null");

  CORE_LEAVE();
}

/*
 * this function builds an identifier object.
 *
 * steps:
 *
 * 0) verify the arguments
 * 1) initialize the identifier object's structure.
 */

t_status		id_build(o_id*				object)
{
  /*
   * 0)
   */

  if (object == NULL)
    CORE_ESCAPE("the 'object' argument is null");

  /*
   * 1)
   */

  memset(object, 0x0, sizeof (o_id));

  CORE_LEAVE();
}

/*
 * this function destroys an identifier object.
 *
 * 0) verify the arguments.
 * 1) re-initialize the object's structure.
 */

t_status		id_destroy(o_id*			object)
{
  /*
   * 0)
   */

  if (object == NULL)
    CORE_ESCAPE("the 'object' argument is null");

  /*
   * 1)
   */

  memset(object, 0x0, sizeof (o_id));

  CORE_LEAVE();
}

/*
 * this function initialize the identifier manager.
 *
 * steps:
 *
 * 1) display a message.
 * 2) initialize the manager's structure.
 */

t_status		id_initialize(void)
{
  /*
   * 1)
   */

  module_call(console, message,
	      '+', "initializing the identifier manager\n");

  /*
   * 2)
   */

  memset(&_id, 0x0, sizeof (m_id));

  CORE_LEAVE();
}

/*
 * this function cleans the identifier manager.
 *
 * 1) display a message.
 */

t_status		id_clean(void)
{
  /*
   * 1)
   */

  module_call(console, message,
	      '+', "cleaning the identifier manager\n");

  CORE_LEAVE();
}
