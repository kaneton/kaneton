/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/set/set.c
 *
 * created       julien quintard   [fri dec  2 19:55:19 2005]
 * updated       julien quintard   [sun jan 30 20:34:47 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the set manager is used to manage the various kernel's data structures.
 * indeed, every kernel manager including the task manager, the thread manager,
 * the segment manager etc. uses the set manager to store its objects rather
 * than creating and managing data structures on their own.
 *
 * in order to add a set implementation, referred to as XYZ, to the set
 * manager, one must complete the following steps:
 *
 *  1) create the kaneton/core/set/set_XYZ.c file and modify the Makefile
 *     to take this new file in account;
 *  2) create the kaneton/include/core/set_XYZ.h file and modify the
 *     set.h to include this new file;
 *  3) add the new set implementation to the union contained in the
 *     set object: o_set, in the set.h header file;
 *  4) add the new set implementation type in the switch case contained
 *     in the set_trap() macro, in the set.h header file.
 *
 * the set manager relies on macro-functions to provide the set implementations
 * with a different number of arguments each with its own type.
 *
 * each set implementation provides some options which may vary depending
 * on the nature of the underlying data structure.
 *
 * the most common options are:
 *   o the ALLOCATE option indicates that the objects to be added must be
 *     cloned i.e the set implementation must allocate its own copy of
 *     the object before insertion.
 *   o the SORT option requests the set implementation to sort the objects
 *     to be stored. note that this option may render the implementation
 *     extremely inefficient. besides, note that the identifier collision
 *     detection can only operate with this option.
 *   o beware of inserting pre-allocated objects. indeed, considering the
 *     set_flush() operation, the set manager would clear the data structure
 *     without releasing the object as it does not know they have been
 *     pre-allocated with malloc(). to counter this issue, the FREE option
 *     has been introduced, instructing the set manager to release the
 *     memory of the object when released.
 *   o the option CONTAINER is only used internally to build the very
 *     first set which is the set container i.e the set of sets.
 *   o the ORGANISE option specifies the set implementation to always try
 *     to re-organise the objects. this option is used, for example, by
 *     the 'array' implementation which shifts the objects whenever
 *     an element is removed .
 *
 * note that the set manager relies on the malloc() and free() functions
 * when it comes to memory management, including related to the ALLOCATE
 * and FREE options.
 *
 * every set can store objects which comply to both (i) the definition of
 * an object and (ii) the set implementation's configuration. first, an
 * object is a data element which begins with a kaneton identifier (t_id).
 * this identifier is used to identify the object in the set, hence locate
 * them for deletion, retrieval, modification etc. second, whenever a set
 * is reserved, the size of the objects to be stored is provided so that
 * memory can be optimised.
 *
 * the programmer must take notice of the documentation related to a set
 * implementation before using it. however, most set implementation comply
 * with a generic interface. the following detail some of the most fondamental
 * set operations:
 *
 * the set_reserve(type, options, datasz, &id) function reserves a set
 * of the type---i.e set implementation---'type' with some options. the
 * 'datasz' argument indicates the size of the objects about the be stored
 * while the 'id' argument is the output set identifier. note that extra
 * arguments may be required depending on the set implementation.
 *
 * the set_release(id) function releases the set identified by 'id'.
 *
 * the set_object(id, iterator, &object) function takes a set identifier
 * along with an iterator pointing to an element of the set and returns
 * the address of the iterator's object in 'object'.
 *
 * in addition, objects can be added/removed/pushed/poped/inserted etc.
 * depending on the set implementation. for example, the set_add(id, object)
 * add the 'object' to the set 'id'.
 *
 * note that this manager is considered fondamental as every other manager
 * relies on it. this manager therefore does no relies on the machine,
 * not mentioning that this functionality provided, i.e data structure
 * management, has nothing to do with the underlying machine.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the set manager.
 */

m_set			_set;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function dumps the set manager.
 *
 * steps:
 *
 * 1) retrieve the set container's descriptor.
 * 2) display general information.
 * 3) show the identifier object.
 * 4) go through the sets.
 *   a) retrieve the set descriptor.
 *   b) show the set.
 */

t_status		set_dump(void)
{
  t_state		state;
  o_set*		data;
  o_set*		o;
  s_iterator		i;

  /*
   * 1)
   */

  if (set_descriptor(_set.sets, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  module_call(console, message,
	      '#', "set manager: sets(%qd)\n",
	      _set.sets);

  /*
   * 3)
   */

  if (id_show(&_set.id, MODULE_CONSOLE_MARGIN_SHIFT) != STATUS_OK)
    CORE_ESCAPE("unable to show the identifier object");

  /*
   * 4)
   */

  module_call(console, message,
	      '#', "  sets: id(%qd) size(%qd)\n",
	      o->id,
	      o->size);

  set_foreach(SET_OPTION_FORWARD, o->id, &i, state)
    {
      /*
       * a)
       */

      if (set_object(o->id, i, (void**)&data) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the set object corresponding "
		    "to its identifier");

      /*
       * b)
       */

      if (set_show(data->id,
		   2 * MODULE_CONSOLE_MARGIN_SHIFT) != STATUS_OK)
	CORE_ESCAPE("unable to show the set");
    }

  CORE_LEAVE();
}

/*
 * this function returns true if the set is empty.
 *
 * steps:
 *
 * 1) retrieve the set descriptor.
 * 2) return false if there is at least an object in the set, true otherwise.
 */

t_bool			set_empty(i_set				setid)
{
  o_set*		o;

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  if (o->size > 0)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function returns the size of the set.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the set descriptor.
 * 2) set the size to return.
 */

t_status		set_size(i_set				setid,
				 t_setsz*			size)
{
  o_set*		o;

  /*
   * 0)
   */

  if (size == NULL)
    CORE_ESCAPE("the 'size' argument is null");

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  /*
   * 2)
   */

  *size = o->size;

  CORE_LEAVE();
}

/*
 * this function adds a set descriptor to the set container.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) add the object depending on its identifier.
 *   A) if the object is the very first one, it is considered as being the
 *       container...
 *     a) allocate memory for the set container's object because the received
 *        object has not been dynamically allocated. indeed the set container
 *        is configured to clone its object i.e the set objects. therefore
 *        the reserved object must be cloned as well.
 *     b) copy the object's content.
 *   B) if not...
 *     a) add the object into the set container.
 */

t_status		set_new(o_set*				object)
{
  /*
   * 0)
   */

  if (object == NULL)
    CORE_ESCAPE("the 'object' argument is null");

  /*
   * 1)
   */

  if (object->id == _set.sets)
    {
      /*
       * A)
       */

      /*
       * a)
       */

      if ((_set.container = malloc(sizeof (o_set))) == NULL)
	CORE_ESCAPE("unable to allocate memory for the set container");

      /*
       * b)
       */

      memcpy(_set.container, object, sizeof (o_set));
    }
  else
    {
      /*
       * B)
       */

      /*
       * a)
       */

      if (set_add(_set.sets, object) != STATUS_OK)
	CORE_ESCAPE("unable to add the set descriptor to the set container");
    }

  CORE_LEAVE();
}

/*
 * this function removes a set descriptor from the set container.
 *
 * steps:
 *
 * 1) removes the set depending on its identifier.
 *   A) the set to remove is the set container...
 *     a) release the container's memory.
 *   B) the set is a normal object...
 *     a) remove it from the set container.
 */

t_status		set_destroy(i_set			setid)
{
  /*
   * 1)
   */

  if (setid == _set.sets)
    {
      /*
       * A)
       */

      /*
       * a)
       */

      free(_set.container);
    }
  else
    {
      /*
       * B)
       */

      /*
       * a)
       */

      if (set_remove(_set.sets, setid) != STATUS_OK)
	CORE_ESCAPE("unable to remove the descriptor from the set container");
    }

  CORE_LEAVE();
}

/*
 * this function returns the set descriptor, i.e set object, corresponding
 * to a set identifier.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) locate the descriptor depending on the set identifier.
 *   A) if the set is the container...
 *     a) return the address of the set container which is located in
 *        the manager's structure.
 *   B) otherwise...
 *     a) retrieve the set object from the set container.
 */

t_status		set_descriptor(i_set			setid,
				       o_set**			object)
{
  /*
   * 0)
   */

  if (object == NULL)
    CORE_ESCAPE("the 'object' argument is null");

  if (setid == ID_UNUSED)
    CORE_ESCAPE("invalid set identifier");

  /*
   * 1)
   */

  if (setid == _set.sets)
    {
      /*
       * A)
       */

      /*
       * a)
       */

      *object = _set.container;
    }
  else
    {
      /*
       * B)
       */

      /*
       * a)
       */

      if (set_get(_set.sets, setid, (void**)object) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the descriptor object from "
		    "the set container");
    }

  CORE_LEAVE();
}

/*
 * this function returns an object given its identifier. note that this
 * function is useful in many cases because the programmer does not
 * have to perform a manual walk through the set objects.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) locate the object in the set according to its identifier.
 * 2) retrieve the object from its iterator.
 */

t_status		set_get(i_set				setid,
				t_id				id,
				void**				object)
{
  s_iterator		iterator;

  /*
   * 0)
   */

  if (object == NULL)
    CORE_ESCAPE("the 'object' argument is null");

  /*
   * 1)
   */

  if (set_locate(setid, id, &iterator) != STATUS_OK)
    CORE_ESCAPE("unable to locate the object in the set");

  /*
   * 2)
   */

  if (set_object(setid, iterator, object) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the object");

  CORE_LEAVE();
}

/*
 * this function initializes the set manager.
 *
 * be careful, the container have to be build using the options
 * SET_OPTION_CONTAINER and SET_OPTION_ALLOCATE.
 *
 * steps:
 *
 * 1) display a message.
 * 2) initialize the set manager's structure.
 * 3) build the identifier object used to generate set identifiers.
 * 4) reserve an identifier for the set container.
 * 5) reserve the set container which will hold the set descriptors i.e
 *    set objects.
 */

t_status		set_initialize(void)
{
  i_set			needless;

  /*
   * 1)
   */

  module_call(console, message,
	      '+', "initializing the set manager\n");

  /*
   * 2)
   */

  memset(&_set, 0x0, sizeof (m_set));

  /*
   * 3)
   */

  if (id_build(&_set.id) != STATUS_OK)
    CORE_ESCAPE("unable to build the identifier object");

  /*
   * 4)
   */

  if (id_reserve(&_set.id, &_set.sets) != STATUS_OK)
    CORE_ESCAPE("unable to reserve the identifier for the set container");

  /*
   * 5)
   */

  if (set_reserve(bpt,
		  SET_OPTION_CONTAINER | SET_OPTION_ALLOCATE | SET_OPTION_SORT,
		  sizeof (o_set),
		  ___kaneton$pagesz,
		  &needless) != STATUS_OK)
    CORE_ESCAPE("unable to reserve the set container");

  CORE_LEAVE();
}

/*
 * this function cleans the set manager.
 *
 * steps:
 *
 * 1) display a message.
 * 2) release every set object the set container contains.
 * 3) release the set container.
 * 4) destroys the identifier object.
 */

t_status		set_clean(void)
{
  s_iterator		iterator;

  /*
   * 1)
   */

  module_call(console, message,
	      '+', "cleaning the set manager\n");

  /*
   * 2)
   */

  while (set_head(_set.sets, &iterator) == TRUE)
    {
      o_set*		o;

      if (set_object(_set.sets, iterator, (void**)&o) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the object from the set");

      if (set_release(o->id) != STATUS_OK)
	CORE_ESCAPE("unable to releases the set object");
    }

  /*
   * 3)
   */

  if (set_release(_set.sets) != STATUS_OK)
    CORE_ESCAPE("unable to release the set container");

  /*
   * 4)
   */

  if (id_destroy(&_set.id) != STATUS_OK)
    CORE_ESCAPE("unable to destroy the identifier object");

  CORE_LEAVE();
}
