/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * set.c
 * 
 * path          /home/mycure/kaneton/core/kaneton/set
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Wed Jul 13 00:47:27 2005   mycure
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the set manager is used to manage the data structures in order to simplify
 * the other kernel managers. indeed, every kernel manager including the
 * task manager, the thread manager, the segment manager etc.. uses the
 * set manager to store the data rather than create themselves data structures
 * by their own.
 *
 * notice that the set manager is based on the malloc() suite functions. indeed
 * the set manager maintains a data structure containing the set descriptors.
 * moreover some data structure type required the malloc() suite functions, the
 * simplest example being the array data structure which require the realloc()
 * function to be able to extend the data structure without effort.
 *
 * due to this requirement, the set manager cannot manage the data structure
 * for the segments of the segment manager, the kernel address space segments
 * and the kernel address space regions of the address space manager because
 * the malloc() function is not provided yet. to bypass this restriction
 * the kaneton kernel uses a initial malloc() version which is based on
 * a little amount of memory provided by the bootloader, via the fields
 * alloc and allocsz of the init structure.
 *
 * then once the kernel starts, the malloc() suite functions are initialized
 * using this special memory used by the segment manager, the region
 * manager and the address space manager to initialize themselves. thereafter,
 * the malloc() suite functions are able to work properly.
 *
 * the implementation use macros to provide different data structure
 * creation with different number of arguments and type of arguments.
 *
 * note that the set manager uses itself to store the set objects.
 *
 * to add a data structure to the set manager you have to complete
 * the following steps:
 *
 *  1) create the core/kaneton/set/set_X.c file and modify the makefile
 *     to add this new file.
 *  2) create the core/include/kaneton/set_X.h file and modify the
 *     set.h to include this new file.
 *  3) add the new data structure to the union contained in the
 *     set object: o_set, in the set.h header file.
 *  4) add the new data structure type in the switch case contained
 *     in the set_trap() macro, in the set.h header file.
 *
 * X represents the new data structure short name.
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
 * the set manager variable.
 */

m_set*			set = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function dumps set objects contained in the set manager.
 *
 * the function just displays the identifier of each element.
 *
 * steps:
 *
 * 1) checks if the set manager was previously initialized.
 * 2) gets the set object from its identifier.
 * 3) prints each element's identifier.
 */

#if (KANETON_DEBUG & KANETON_DEBUG_SET)
int			set_dump(t_setid			setid)
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

  if (set_object(setid, &o) != 0)
    return (-1);

  /*
   * 3)
   */

  cons_msg('#', "dumping %qu node(s) from the set %qu:\n",
	   o->size,
	   setid);

  set_foreach(SET_OPT_FORWARD, setid, &i)
    {
      if (set_get(setid, i, &data) != 0)
	{
	  kaneton_error("the set manager cannot find the set object "
			"corresponding to a set identifier\n");

	  return (-1);
	}

      cons_msg('#', "%qu\n", data->id);
    }

  return (0);
}
#endif

/*
 * this function adds a set object to the set objects list.
 *
 * steps:
 *
 * 1) checks whether the set manager was previously initialized.
 * 2) if the object to add is the set which will contain the set objects
 *    just put it as the set objects container.
 * 3) otherwise, add this object in the set object container.
 */

int			set_object_add(o_set*			o)
{
  /*
   * 1)
   */

  set_check(set);

  /*
   * 2)
   */

  if (o->id == SET_ID_CONTAINER)
    {
      set->container = o;

      return (0);
    }

  /*
   * 3)
   */

  if (set_add(set->container->id, o) != 0)
    {
      kaneton_error("the set manager is unable to add this object "
		    "to the set objects container\n");

      return (-1);
    }

  return (0);
}

/*
 * this function removes a set object from the set objects container.
 *
 * steps:
 *
 * 1) checks whether the set manager was previously initialized.
 * 2) removing the container object is not allowed.
 * 3) removes the set object from the set objects container.
 */

int			set_object_remove(t_setid		setid)
{
  /*
   * 1)
   */

  set_check(set);

  /*
   * 2)
   */

  if (setid == SET_ID_CONTAINER)
    {
      kaneton_error("the set manager cannot remove the set object container"
		    "object\n");

      return (-1);
    }

  /*
   * 3)
   */

  if (set_remove(set->container->id, setid) != 0)
    {
      kaneton_error("the set manager is unable to remove this object "
		    "from the set objects container\n");

      return (-1);
    }

  return (0);
}

/*
 * this function returns the set object corresponding to a set identifier.
 *
 * steps:
 *
 * 1) checks whether the set manager was previously initialized.
 * 2) if the looked for object is the set objects container, just returns
 *    it.
 * 3) otherwise, try to find the set object in the set objects container
 *    and return it if possible.
 */

int			set_object(t_setid			setid,
				   o_set**			o)
{
  t_iterator		i;

  /*
   * 1)
   */

  set_check(set);

  /*
   * 2)
   */

  if (setid == SET_ID_CONTAINER)
    {
      *o = set->container;

      return (0);
    }

  /*
   * 3)
   */

  if (set_find(set->container->id, setid, &i) != 0)
    return (-1);

  *o = i;

  return (0);
}

/*
 * this function returns an unique unused set identifier.
 */

int			set_id_rsv(t_setid*			setid)
{
  set_check(set);

  *setid = set->id++;

  return (0);
}

/*
 * this function releases a previously reserved identifier.
 *
 * because we use 64-bit integer, we do not care about recycling
 * identifiers, so just do nothing in this case.
 */

int			set_id_rel(t_setid			setid)
{
  set_check(set);

  return (0);
}

/*
 * this generic function tries to find an object with its identifier and build
 * a corresponding identifier.
 *
 * steps:
 *
 * 1) checks whether the set manager was previously initialized.
 * 2) gets the set object corresponding to the set identifier.
 * 3) tries to find the identifier looked for in the set object's elements.
 */

int			set_find(t_setid			setid,
				 t_id				id,
				 t_iterator*			iterator)
{
  t_id*			data;
  o_set*		o;
  t_iterator		i;

  /*
   * 1)
   */

  set_check(set);

  /*
   * 2)
   */

  if (set_object(setid, &o) != 0)
    {
      kaneton_error("the set manager is not able to find the set object "
		    "corresponding to the set identifier %qu\n", setid);

      return (-1);
    }

  /*
   * 3)
   */

  set_foreach(SET_OPT_FORWARD, setid, &i)
    {
      if (set_get(setid, i, &data) != 0)
	{
	  kaneton_error("the set manager cannot find the set object "
			"corresponding to a set identifier\n");

	  return (-1);
	}

      if (*data == id)
	{
	  *iterator = data;

	  return (0);
	}
    }

  return (-1);
}

/*
 * this function initializes the set manager.
 *
 * steps:
 *
 * 1) allocates and initializes the set manager structure.
 * 2) reserves the set objects container which will contain the
 *    set objects reserved later.
 * 3) if necessary, dumps the set objects container.
 */

int			set_init(void)
{
  t_setid		needless;

  /*
   * 1)
   */

  if ((set = malloc(sizeof(m_set))) == NULL)
    return (-1);

  memset(set, 0x0, sizeof(m_set));

  /*
   * 2)
   */

  set_rsv(ll, SET_OPT_NONE, sizeof(o_set), &needless);

  /*
   * 3)
   */

#if (KANETON_DEBUG & KANETON_DEBUG_SET)
  set_dump(set->container->id);
#endif

  return (0);
}

/*
 * XXX
 *
 * ajouter SORT
 */
