/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/set/set.c
 *
 * created       julien quintard   [fri dec  2 19:55:19 2005]
 * updated       julien quintard   [sat jul  8 02:36:34 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the set manager is used to manage the data structures in order to simplify
 * the other kernel managers. indeed, every kernel manager including the
 * task manager, the thread manager, the segment manager etc. uses the
 * set manager to store the data rather than create themselves data structures
 * by their own.
 *
 * to add a data structure to the set manager you have to complete
 * the following steps:
 *
 *  1) create the kaneton/core/set/set_X.c file and modify the makefile
 *     to add this new file.
 *  2) create the kaneton/include/core/set_X.h file and modify the
 *     set.h to include this new file.
 *  3) add the new data structure to the union contained in the
 *     set object: o_set, in the set.h header file.
 *  4) add the new data structure type in the switch case contained
 *     in the set_trap() macro, in the set.h header file.
 *
 * X represents the new data structure short name.
 *
 * the implementation uses macros to provide different data structures
 * manipulation with different number of arguments and different
 * type of arguments.
 *
 * note the specific use of the ## in the set header located
 * in kaneton/include/core/.
 *
 * using the ## with variadic macros allows the use of a variable arguments
 * parameter to be empty, deleting the previous comma if needed.
 *
 * note that this advanced feature is only implemented by the gnu
 * c-preprocessor.
 *
 * each data structure allows some options to parameterise the data structure
 * wanted. the common ones are: ALLOCATE, SORT etc.
 *
 * the ALLOCATE option may be disabled for certain data structures because
 * of its meaningless for this data structure.
 *
 * note that the SORT option may become very inefficient for some data
 * structures. moreover the identifier collision detection may only
 * takes place if the SORT option is enabled.
 *
 * be careful with the ALLOCATE option. this option tells the set manager
 * to always allocate memory and to copy into this memory the contents
 * of the given object. think about that, for example, the set_flush()
 * will clear the data structure contents. if the ALLOCATE option is
 * set, the elements will be freed, but if not, the elements will be lost.
 *
 * for this common problem, a new option was introduced, the FREE option
 * which tells the set manager to call the free() function every time an
 * element is released, for example into the functions set_remove(),
 * set_flush(), set_release() etc.
 *
 * be careful to never set the FREE option with the ALLOCATE option. the
 * FREE option tells the set manager that the user is using preallocated
 * objects which is the contrary to the allocate option.
 *
 * the option CONTAINER is only used by the set manager to build the very
 * first set which is the set container.
 *
 * the ORGANISE option is used for example by the array data structure. it
 * tells the subpart of the set manager to always organise data in the
 * best way. for the array data structure, this option means that the
 * elements will always be left shifted to fulfill the array by the left.
 *
 * the two macros SET_ENTER and SET_LEAVE are used to launch operations
 * entering and leaving each set manager functions excluding the
 * set_initialize() and set_clean() functions which install the manager.
 *
 * if a developer wants to add something at the start of each set manager's
 * function, he simply has to add his lines in the SET_ENTER macro located
 * in the kaneton/include/core/set.h header file.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

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
 * this function dumps the set manager state.
 *
 * the set container will be displayed with each set object contained.
 */

t_error			set_dump(void)
{
  t_state		state;
  o_set*		data;
  o_set*		o;
  t_iterator		i;

  SET_ENTER(set);

  /*
   * 1)
   */

  if (set_descriptor(set->sets, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  cons_msg('#', "dumping the set container %qd with %qd node(s)\n",
	   set->sets,
	   o->size);

  set_foreach(SET_OPT_FORWARD, set->sets, &i, state)
    {
      if (set_object(set->sets, i, (void**)&data) != ERROR_NONE)
	{
	  cons_msg('!', "set: cannot find the set object "
		   "corresponding to its identifier\n");

	  SET_LEAVE(set, ERROR_UNKNOWN);
	}

      if (set_show(data->setid) != ERROR_NONE)
	SET_LEAVE(set, ERROR_UNKNOWN);
    }

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function fills the variable size with the set size.
 */

t_error			set_size(i_set				setid,
				 t_setsz*			size)
{
  o_set*		o;

  SET_ENTER(set);

  assert(size != NULL);

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  *size = o->size;

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function adds a set descriptor to the set container.
 *
 * steps:
 *
 * 1) if the descriptor to add is the set which will contain the set objects,
 *    the container, just put it as the set container.
 * 2) otherwise, add this object in the set container.
 */

t_error			set_new(o_set*				o)
{
  SET_ENTER(set);

  assert(o != NULL);

  /*
   * 1)
   */

  if (o->setid == set->sets)
    {
      if ((set->container = malloc(sizeof(o_set))) == NULL)
	SET_LEAVE(set, ERROR_UNKNOWN);

      memcpy(set->container, o, sizeof(o_set));

      SET_LEAVE(set, ERROR_NONE);
    }

  /*
   * 2)
   */

  if (set_add(set->sets, o) != ERROR_NONE)
    {
      cons_msg('!', "set: unable to add this set descriptor "
	       "to the set container\n");

      SET_LEAVE(set, ERROR_UNKNOWN);
    }

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function removes a set descriptor from the set container.
 *
 * steps:
 *
 * 1) removes the set container object.
 * 2) removes the set descriptor from the set container.
 */

t_error			set_destroy(i_set			setid)
{
  SET_ENTER(set);

  /*
   * 1)
   */

  if (setid == set->sets)
    {
      free(set->container);

      SET_LEAVE(set, ERROR_UNKNOWN);
    }

  /*
   * 2)
   */

  if (set_remove(set->sets, setid) != ERROR_NONE)
    {
      cons_msg('!', "set: unable to remove this descriptor "
	       "from the set container\n");

      SET_LEAVE(set, ERROR_UNKNOWN);
    }

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function returns the set descriptor corresponding to a set identifier.
 *
 * steps:
 *
 * 1) if the looked for descriptor is the set container, just return it.
 * 2) otherwise, tries to get the set descriptor in the set container.
 */

t_error			set_descriptor(i_set			setid,
				       o_set**			o)
{
  SET_ENTER(set);

  assert(o != NULL);

  /*
   * 1)
   */

  if (setid == set->sets)
    {
      *o = set->container;

      SET_LEAVE(set, ERROR_NONE);
    }

  /*
   * 2)
   */

  if (set_get(set->sets, setid, (void**)o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function returns an object given its identifier without being
 * force to deal with iterators.
 *
 * steps:
 *
 * 1) finds the object given its identifier and gets an iterator on it.
 * 2) from the iterator, gets the real object.
 */

t_error			set_get(i_set				setid,
				t_id				id,
				void**				o)
{
  t_iterator		iterator;

  SET_ENTER(set);

  assert(o != NULL);

  /*
   * 1)
   */

  if (set_locate(setid, id, &iterator) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (set_object(setid, iterator, o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function initializes the set manager.
 *
 * be careful, the container have to be build using the option SET_OPT_ALLOC.
 * if this option is not set, the entire set manager will not be able
 * to work properly.
 *
 * steps:
 *
 * 1) allocates and initializes the set manager structure.
 * 2) builds the identifier object used to generate set identifiers.
 * 3) reserves an identifier for the container.
 * 4) reserves the set container which will contain the set descriptors
 *    reserved later. obviously, it will be better to take a powerful
 *    data structure because it will contains millions of objects.
 */

t_error			set_initialize(void)
{
  i_set			needless;

  /*
   * 1)
   */

  if ((set = malloc(sizeof(m_set))) == NULL)
    {
      cons_msg('!', "set: cannot allocate memory for the set manager "
	       "structure\n");

      return (ERROR_UNKNOWN);
    }

  memset(set, 0x0, sizeof(m_set));

  /*
   * 2)
   */

  if (id_build(&set->id) != ERROR_NONE)
    {
      cons_msg('!', "set: unable to initialize the identifier object\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  if (id_reserve(&set->id, &set->sets) != ERROR_NONE)
    {
      cons_msg('!', "set: unable to reserve an identifier\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  if (set_reserve(bpt, SET_OPT_CONTAINER | SET_OPT_ALLOC | SET_OPT_SORT,
		  sizeof(o_set), PAGESZ, &needless) != ERROR_NONE)
    {
      cons_msg('!', "set: unable to reserve the set container\n");

      return (ERROR_UNKNOWN);
    }

  return (ERROR_NONE);
}

/*
 * this function just reinitializes the set manager.
 *
 * steps:
 *
 * 1) releases each set object the set container still contains.
 * 2) releases the set container.
 * 3) destroys the id object.
 * 4) frees the set manager structure's memory.
 */

t_error			set_clean(void)
{
  t_iterator		iterator;

  /*
   * 1)
   */

  while (set_head(set->sets, &iterator) == ERROR_NONE)
    {
      o_set*		o;

      if (set_object(set->sets, iterator, (void**)&o) != ERROR_NONE)
	{
	  cons_msg('!', "set: cannot find the set object "
		   "corresponding to its identifier\n");

	  SET_LEAVE(set, ERROR_UNKNOWN);
	}

      if (set_release(o->setid) != ERROR_NONE)
	{
	  cons_msg('!', "set: cannot releases a set object located in the "
		   "set container\n");

	  SET_LEAVE(set, ERROR_UNKNOWN);
	}
    }

  /*
   * 2)
   */

  if (set_release(set->sets) != ERROR_NONE)
    {
      cons_msg('!', "set: unable to release the set container\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  if (id_destroy(&set->id) != ERROR_NONE)
    {
      cons_msg('!', "set: unable to destroy the identifier object\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  free(set);

  return (ERROR_NONE);
}
