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
 * last update   Tue Jul 19 16:21:49 2005   mycure
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
 * the set manager maintains a data structure containing the set descriptors:
 * the container. moreover some data structure types required the malloc()
 * suite functions, the simplest example being the array data structure
 * which requires the realloc() function to be able to extend the data
 * structure without effort.
 *
 * due to this requirement, the set manager cannot manage the data structure
 * for the segments of the segment manager, the kernel address space segments
 * and the kernel address space regions of the address space manager because
 * the malloc() function is not provided yet. to bypass this restriction
 * the kaneton kernel uses an initial malloc() version which is based on
 * a little amount of memory provided by the bootloader, via the fields
 * alloc and allocsz of the init structure.
 *
 * using this initial malloc() function, the set manager is able to build
 * sets for every kernel managers: segment manager, address space manager,
 * region manager etc..
 *
 * the implementation use macros to provide different data structures
 * creation with different number of arguments and different
 * type of arguments.
 *
 * note that the set manager uses itself to store the set descriptors.
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
 *
 * nomenclature:
 *
 *  container
 *
 *    the container is the set which contains all the set descriptors.
 *
 *  descriptor
 *
 *    a set descriptor is the set meta-data. indeed, a set descriptor
 *    contains the set data structure containing the objects, the number
 *    of objects managed, the identifier of the set etc..
 *
 *  node
 *
 *    a node is a data structure element. indeed, each iterator references a
 *    node to be able to locate the previous and next nodes in this
 *    set.
 *
 *  object
 *
 *    an object, in the set manager terms, is the data contained in
 *    a set node, the data the "user" wants to store using the set manager.
 *
 * in other words, the container is a set which contains the set descriptors.
 * each set descriptor contains a data structure composed of nodes
 * which contain the objects provided by the set manager user.
 *
 * note that in the kaneton terms, the set manager is composed of objects.
 * indeed, the container, each descriptor are in fact set objects (o_set)
 * so kaneton objects.
 *
 * moreover, the objects the user provides to the set manager to be stored
 * also are kaneton objects.
 *
 * remember that, in the kaneton terms, an object structure always begins
 * with an identifier and is manipulated by a capability on the distributed
 * system.
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
 * this function dumps set objects contained in the container of
 * the set manager.
 *
 * in other words, this function displays the set descriptors.
 *
 * the function just displays the identifier of each element.
 *
 * steps:
 *
 * 1) checks if the set manager was previously initialized.
 * 2) gets the set descriptor from its identifier.
 * 3) prints each objects' identifier.
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

  if (set_descriptor(setid, &o) != 0)
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
	  cons_msg('!', "the set manager cannot find the set object "
		   "corresponding to a set identifier\n");

	  return (-1);
	}

      cons_msg('#', "%qu\n", data->id);
    }

  return (0);
}
#endif

/*
 * this function adds a set descriptor to the set container.
 *
 * steps:
 *
 * 1) checks whether the set manager was previously initialized.
 * 2) if the descriptor to add is the set which will contain the set objects,
 *    the container, just put it as the set container.
 * 3) otherwise, add this object in the set container.
 */

int			set_new(o_set*				o)
{
  /*
   * 1)
   */

  set_check(set);

  /*
   * 2)
   */

  if (o->id == set->contid)
    {
      memcpy(set->cont, o, sizeof(o_set));

      return (0);
    }

  /*
   * 3)
   */

  if (set_add(set->cont->id, o) != 0)
    {
      cons_msg('!', "the set manager is unable to add this set descriptor "
	       "to the set container\n");

      return (-1);
    }

  return (0);
}

/*
 * this function removes a set descriptor from the set container.
 *
 * steps:
 *
 * 1) checks whether the set manager was previously initialized.
 * 2) removing the container object is not allowed.
 * 3) removes the set descriptor from the set container.
 */

int			set_delete(t_setid			setid)
{
  /*
   * 1)
   */

  set_check(set);

  /*
   * 2)
   */

  if (setid == set->contid)
    {
      cons_msg('!', "the set manager cannot remove the set container\n");

      return (-1);
    }

  /*
   * 3)
   */

  if (set_remove(set->cont->id, setid) != 0)
    {
      cons_msg('!', "the set manager is unable to remove this descriptor "
	       "from the set container\n");

      return (-1);
    }

  return (0);
}

/*
 * this function returns the set descriptor corresponding to a set identifier.
 *
 * steps:
 *
 * 1) checks whether the set manager was previously initialized.
 * 2) if the looked for descriptor is the set container, just return it.
 * 3) otherwise, try to find the set descriptor in the set container
 *    and return it if possible.
 */

int			set_descriptor(t_setid			setid,
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

  if (setid == set->contid)
    {
      *o = set->cont;

      return (0);
    }

  /*
   * 3)
   */

  if (set_find(set->cont->id, setid, &i) != 0)
    return (-1);

  *o = i;

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

  if (set_descriptor(setid, &o) != 0)
    {
      cons_msg('!', "the set manager is not able to find the set object "
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
	  cons_msg('!', "the set manager cannot find the set object "
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
 * 2) builds the identifier object used to generate set identifiers.
 * 3) reserves an identifier for the container.
 * 4) allocates the set container structure.
 * 5) reserves the set container which will contain the set descriptors
 *    reserved later.
 * 6) if necessary, dumps the set container.
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

  if (id_build(&set->id) != 0)
    return (-1);

  /*
   * 3)
   */

  if (id_rsv(&set->id, &set->contid) != 0)
    return (-1);

  /*
   * 4)
   */

  if ((set->cont = malloc(sizeof(o_set))) == NULL)
    return (-1);

  /*
   * 5)
   */

  if (set_rsv(ll, SET_OPT_ALLOC | SET_OPT_CONTAINER,
	      sizeof(o_set), &needless) != 0)
    return (-1);

  /*
   * 6)
   */

#if (KANETON_DEBUG & KANETON_DEBUG_SET)
  set_dump(set->cont->id);
#endif

  return (0);
}
