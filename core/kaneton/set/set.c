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
 * last update   Fri Jul 22 14:29:55 2005   mycure
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
 * note that the set manager uses itself to store the set descriptors.
 *
 * moreover, the set manager will provide a very very simple way to compare
 * different algorithms in different cases just by modifying the
 * call to set_rsv() in the desired manager.
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
 *
 * the implementation uses macros to provide different data structures
 * manipulation with different number of arguments and different
 * type of arguments.
 *
 * note the specific use of the ## in the set header located
 * in core/include/kaneton/.
 *
 * using the ## with variadic macros allow the use of a variable arguments
 * parameter to be empty, deleting the previous comma if needed.
 *
 * note that this advanced feature is only implemented by the gnu
 * c preprocessor.
 *
 * each data structure allows some options to parameterise the data structure
 * wanted. the common ones are: allocate, sort etc..
 *
 * the allocate option may be disabled for certain data structures because
 * of its meaningless for this data structure.
 *
 * note that the sort option may becomes very inefficient for some data
 * structures. moreover the identifier collision detection may only
 * takes place if the sort option is enabled.
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
 * 1) checks if the set manager was previously initialised.
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
      if (set_object(setid, i, (void**)&data) != 0)
	{
	  cons_msg('!', "set: cannot find the object "
		   "corresponding to its identifier\n");

	  return (-1);
	}

      cons_msg('#', "  %qd [XXX] 0x%x\n",
	       data->id == ID_UNUSED ? -1 : data->id, i);
    }

  return (0);
}
#endif

/*
 * this function fills the variable size with the set size.
 */

int			set_size(t_setid			setid,
				 t_setsz*			size)
{
  o_set*		o;

  if (set_descriptor(setid, &o) != 0)
    return (-1);

  *size = o->size;

  return (0);
}

/*
 * this function adds a set descriptor to the set container.
 *
 * steps:
 *
 * 1) checks whether the set manager was previously initialised.
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

  if (o->id == set->setid)
    {
      if ((set->container = malloc(sizeof(o_set))) == NULL)
	return (-1);

      memcpy(set->container, o, sizeof(o_set));

      return (0);
    }

  /*
   * 3)
   */

  if (set_add(set->container->id, o) != 0)
    {
      cons_msg('!', "set: unable to add this set descriptor "
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
 * 1) checks whether the set manager was previously initialised.
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

  if (setid == set->setid)
    {
      cons_msg('!', "set: cannot remove the set container\n");

      return (-1);
    }

  /*
   * 3)
   */

  if (set_remove(set->container->id, setid) != 0)
    {
      cons_msg('!', "set: unable to remove this descriptor "
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
 * 1) checks whether the set manager was previously initialised.
 * 2) if the looked for descriptor is the set container, just return it.
 * 3) otherwise, tries to get the set descriptor in the set container.
 */

int			set_descriptor(t_setid			setid,
				       o_set**			o)
{
  /*
   * 1)
   */

  set_check(set);

  /*
   * 2)
   */

  if (setid == set->setid)
    {
      *o = set->container;

      return (0);
    }

  /*
   * 3)
   */

  if (set_get(set->setid, setid, (void**)o) != 0)
    return (-1);

  return (0);
}

/*
 * this generic function tries to find an object with its identifier and build
 * a corresponding identifier.
 *
 * steps:
 *
 * 1) checks whether the set manager was previously initialised.
 * 2) gets the set object corresponding to the set identifier.
 * 3) tries to find the identifier looked for in the set object's elements.
 */

int			set_locate(t_setid			setid,
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
      cons_msg('!', "set: not able to locate the set object "
	       "corresponding to the set identifier %qu\n", setid);

      return (-1);
    }

  /*
   * 3)
   */

  set_foreach(SET_OPT_FORWARD, setid, &i)
    {
      if (set_object(setid, i, (void**)&data) != 0)
	{
	  cons_msg('!', "set: cannot find the object "
		   "corresponding to its identifier\n");

	  return (-1);
	}

      if (*data == id)
	{
	  memcpy(iterator, &i, sizeof(t_iterator));

	  return (0);
	}
    }

  return (-1);
}

/*
 * this function returns an object given its identifier without being
 * force to deal with iterators.
 *
 * steps:
 *
 * 1) first checks whether the set manager is initialised.
 * 2) finds the object given its identifier and gets an iterator on it.
 * 3) from the iterator, gets the real object.
 */

int			set_get(t_setid				setid,
				t_id				id,
				void**				o)
{
  t_iterator		iterator;

  /*
   * 1)
   */

  set_check(set);

  /*
   * 2)
   */

  if (set_locate(setid, id, &iterator) != 0)
    return (-1);

  /*
   * 3)
   */

  if (set_object(setid, iterator, o) != 0)
    return (-1);

  return (0);
}

/*
 * this function initialises the set manager.
 *
 * be careful, the container have to be build using the option SET_OPT_ALLOC.
 * if this option is not set, the entire set manager will not be able
 * to work properly.
 *
 * steps:
 *
 * 1) allocates and initialises the set manager structure.
 * 2) builds the identifier object used to generate set identifiers.
 * 3) reserves an identifier for the container.
 * 4) reserves the set container which will contain the set descriptors
 *    reserved later.
 * 5) if necessary, dumps the set container.
 */

int			set_init(void)
{
  t_setid		needless;

  /*
   * 1)
   */

  if ((set = malloc(sizeof(m_set))) == NULL)
    {
      cons_msg('!', "set: cannot allocate memory for the set manager "
	       "structure\n");

      return (-1);
    }

  memset(set, 0x0, sizeof(m_set));

  /*
   * 2)
   */

  if (id_build(&set->id) != 0)
    {
      cons_msg('!', "set: unable to initialise the identifier object\n");

      return (-1);
    }

  /*
   * 3)
   */

  if (id_rsv(&set->id, &set->setid) != 0)
    {
      cons_msg('!', "set: unable to reserve an identifier\n");

      return (-1);
    }

  /*
   * 4)
   */

  if (set_rsv(ll, SET_OPT_CONTAINER | SET_OPT_ALLOC | SET_OPT_SORT,
	      sizeof(o_set), &needless) != 0)
    {
      cons_msg('!', "set: unable to reserve the set container\n");

      return (-1);
    }

  /*
   * 5)
   */

#if (KANETON_DEBUG & KANETON_DEBUG_SET)
  set_dump(set->container->id);
#endif

#if 0 /* SET_TYPE_ARRAY */
 {
   t_iterator	iterator;
   t_id		data;
   t_id		*o;
   t_setid	id;

   printf("SET_TYPE_ARRAY\n");

   if (set_rsv(array, SET_OPT_ALLOC | SET_OPT_SORT, 10,
	       sizeof(t_uint64), &id) != 0)
     printf("error: set_rsv()\n");

   data = 98LL;
   if (set_add(id, &data) != 0)
     printf("error: set_add()\n");

   data = 843536LL;
   if (set_add(id, &data) != 0)
     printf("error: set_add()\n");

   data = 100LL;
   if (set_add(id, &data) != 0)
     printf("error: set_add()\n");

   /* XXX continue the tests */

   set_dump(id);

   while (1);
 }
#endif

#if 0 /* SET_TYPE_LL */
 {
   t_iterator	iterator;
   t_id		data;
   t_id		*o;
   t_setid	id;

   printf("SET_TYPE_LL\n");

   if (set_rsv(ll, SET_OPT_ALLOC, sizeof(t_uint64), &id) != 0)
     printf("error: set_rsv()\n");

   data = 98LL;
   if (set_add(id, &data) != 0)
     printf("error: set_add()\n");

   data = 843536LL;
   if (set_add(id, &data) != 0)
     printf("error: set_add()\n");

   data = 23987LL;
   if (set_add(id, &data) != 0)
     printf("error: set_add()\n");

   set_dump(id);

   if (set_locate(id, 98LL, &iterator) != 0)
     printf("error: set_locate()\n");

   if (set_object(id, iterator, (void**)&o) != 0)
     printf("error: set_object()\n");

   printf("locate() -> object(): 0x%x %qu\n", iterator, *o);

   if (set_get(id, 320498523LL, (void**)&o) == 0)
     printf("error: set_get(), found a unexisting object\n");

   if (set_get(id, 843536LL, (void**)&o) != 0)
     printf("error: set_get()\n");

   printf("get(): %qu\n", *o);

   if (set_remove(id, 23987LL) != 0)
     printf("error: set_remove()\n");

   data = 6354LL;
   if (set_add(id, &data) != 0)
     printf("error: set_add()\n");

   data = 45897634543LL;
   if (set_add(id, &data) != 0)
     printf("error: set_add()\n");

   data = 0LL;
   if (set_add(id, &data) != 0)
     printf("error: set_add()\n");

   if (set_remove(id, 0LL) != 0)
     printf("error: set_remove()\n");

   data = 234LL;
   if (set_add(id, &data) != 0)
     printf("error: set_add()\n");

   if (set_remove(id, 45897634543LL) != 0)
     printf("error: set_remove()\n");

   if (set_remove(id, 234LL) != 0)
     printf("error: set_remove()\n");

   data = 78LL;
   if (set_insert_head(id, &data) != 0)
     printf("error: set_insert_head()\n");

   data = 1LL;
   if (set_insert_tail(id, &data) != 0)
     printf("error: set_insert_tail()\n");

   if (set_locate(id, 1LL, &iterator) != 0)
     printf("error: set_locate()\n");

   data = 789456LL;
   if (set_insert_before(id, iterator, &data) != 0)
     printf("error: set_insert_before()\n");

   if (set_locate(id, 78LL, &iterator) != 0)
     printf("error: set_locate()\n");

   data = 42LL;
   if (set_insert_before(id, iterator, &data) != 0)
     printf("error: set_insert_before()\n");

   set_dump(id);

   if (set_locate(id, 42LL, &iterator) != 0)
     printf("error: set_locate()\n");

   data = 123456LL;
   if (set_insert_after(id, iterator, &data) != 0)
     printf("error: set_insert_after()\n");

   if (set_locate(id, 1LL, &iterator) != 0)
     printf("error: set_locate()\n");

   data = 21LL;
   if (set_insert_after(id, iterator, &data) != 0)
     printf("error: set_insert_after()\n");

   set_dump(id);

   alloc_dump();

   while (1);
 }
#endif

  return (0);
}
