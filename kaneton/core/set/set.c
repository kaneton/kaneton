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

m_set*			_set = NULL;

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

  /*
   * 1)
   */

  if (set_descriptor(_set->sets, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  module_call(console, message,
	      '#', "dumping the set container %qd with %qd node(s)\n",
	      _set->sets,
	      o->size);

  set_foreach(SET_OPTION_FORWARD, _set->sets, &i, state)
    {
      if (set_object(_set->sets, i, (void**)&data) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the set object corresponding "
		    "to its identifier");

      if (set_show(data->id) != ERROR_OK)
	CORE_ESCAPE("unable to show the set");
    }

  CORE_LEAVE();
}

/*
 * this function returns true if the set is empty.
 */

t_error			set_empty(i_set				setid)
{
  o_set*		o;

  if (set_descriptor(setid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  if (o->size > 0)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function fills the variable size with the set size.
 */

t_error			set_size(i_set				setid,
				 t_setsz*			size)
{
  o_set*		o;

  assert(size != NULL);

  if (set_descriptor(setid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  *size = o->size;

  CORE_LEAVE();
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
  assert(o != NULL);

  /*
   * 1)
   */

  if (o->id == _set->sets)
    {
      if ((_set->container = malloc(sizeof(o_set))) == NULL)
	CORE_ESCAPE("unable to allocate memory for the set container");

      memcpy(_set->container, o, sizeof(o_set));

      CORE_LEAVE();
    }

  /*
   * 2)
   */

  if (set_add(_set->sets, o) != ERROR_OK)
    CORE_ESCAPE("unable to add the set descriptor to the set container");

  CORE_LEAVE();
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
  /*
   * 1)
   */

  if (setid == _set->sets)
    {
      free(_set->container);

      CORE_LEAVE();
    }

  /*
   * 2)
   */

  if (set_remove(_set->sets, setid) != ERROR_OK)
    CORE_ESCAPE("unable to remove the descriptor from the set container");

  CORE_LEAVE();
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
  assert(o != NULL);

  /*
   * 1)
   */

  if (setid == _set->sets)
    {
      *o = _set->container;

      CORE_LEAVE();
    }

  /*
   * 2)
   */

  if (set_get(_set->sets, setid, (void**)o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the descriptor object from "
		"the set container");

  CORE_LEAVE();
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

  assert(o != NULL);

  /*
   * 1)
   */

  if (set_locate(setid, id, &iterator) != ERROR_OK)
    CORE_ESCAPE("unable to locate the object in the set");

  /*
   * 2)
   */

  if (set_object(setid, iterator, o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the object");

  CORE_LEAVE();
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

  if ((_set = malloc(sizeof(m_set))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the set manager's structure");

  memset(_set, 0x0, sizeof(m_set));

  /*
   * 2)
   */

  if (id_build(&_set->id) != ERROR_OK)
    CORE_ESCAPE("unable to build the identifier object");

  /*
   * 3)
   */

  if (id_reserve(&_set->id, &_set->sets) != ERROR_OK)
    CORE_ESCAPE("unable to reserve the identifier for the set container");

  /*
   * 4)
   */

  // XXX ???
  if (set_reserve(bpt,
		  SET_OPTION_CONTAINER | SET_OPTION_ALLOC | SET_OPTION_SORT,
		  sizeof(o_set),
		  PAGESZ,
		  &needless) != ERROR_OK)
    CORE_ESCAPE("unable to reserve the set container");

  CORE_LEAVE();
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

  while (set_head(_set->sets, &iterator) == ERROR_TRUE)
    {
      o_set*		o;

      if (set_object(_set->sets, iterator, (void**)&o) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the object from the set");

      if (set_release(o->id) != ERROR_OK)
	CORE_ESCAPE("unable to releases the set object");
    }

  /*
   * 2)
   */

  if (set_release(_set->sets) != ERROR_OK)
    CORE_ESCAPE("unable to release the set container");

  /*
   * 3)
   */

  if (id_destroy(&_set->id) != ERROR_OK)
    CORE_ESCAPE("unable to destroy the identifier object");

  /*
   * 4)
   */

  free(_set);

  CORE_LEAVE();
}
