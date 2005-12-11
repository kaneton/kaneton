/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/kaneton/set/set.c
 *
 * created       julien quintard   [fri dec  2 19:55:19 2005]
 * updated       matthieu bucchianeri   [mon dec 12 00:27:05 2005]
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
 * alloc and allocsz of the init structure: this area is called the survey
 * memory area.
 *
 * using this initial malloc() function, the set manager is able to build
 * sets for every kernel managers: segment manager, address space manager,
 * region manager etc..
 *
 * note that the set manager uses itself to store the set descriptors.
 *
 * moreover, the set manager will provide a very very simple way to compare
 * different algorithms in different cases just by modifying the
 * call to set_reserve() in the desired manager.
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
 *    sometimes, the node is also the object as in the array data structure.
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
 * indeed, in the container, each descriptor are in fact set objects (o_set)
 * so kaneton objects.
 *
 * moreover, the objects the user provides to the set manager to be stored
 * also are kaneton objects.
 *
 * remember that, in the kaneton terms, an object structure always begins
 * with an identifier and is manipulated by a capability on the distributed
 * operating system.
 *
 * the implementation uses macros to provide different data structures
 * manipulation with different number of arguments and different
 * type of arguments.
 *
 * note the specific use of the ## in the set header located
 * in core/include/kaneton/.
 *
 * using the ## with variadic macros allows the use of a variable arguments
 * parameter to be empty, deleting the previous comma if needed.
 *
 * note that this advanced feature is only implemented by the gnu
 * c-preprocessor.
 *
 * each data structure allows some options to parameterise the data structure
 * wanted. the common ones are: ALLOCATE, SORT etc..
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
 * set_flush(), set_release() etc..
 *
 * moreover the FREE option is used in the set_clone() function to
 * duplicate every element.
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
 * entering and leaving each set manager functions excluding the set_init()
 * and set_clean() functions which install the manager.
 *
 * if a developer wants to add something at the start of each set manager's
 * function, he simply has to add his lines in the SET_ENTER macro located
 * in the core/include/kaneton/set.h header file.
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * the student should understand the whole set manager, first to be able
 * to develop into the kaneton project and to add set data structures, then
 * because it uses very special c preprocessor interesting features.
 *
 * take a look to the subparts of the set manager. indeed the real assignments
 * are in relation with these subparts.
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

  if (set_descriptor(set->container, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 2)
   */

  cons_msg('#', "dumping the set container %qd with %qd node(s)\n",
	   set->container,
	   o->size);

  set_foreach(SET_OPT_FORWARD, set->container, &i, state)
    {
      if (set_object(set->container, i, (void**)&data) != ERROR_NONE)
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

t_error			set_size(t_setid			setid,
				 t_setsz*			size)
{
  o_set*		o;

  SET_ENTER(set);

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

  /*
   * 1)
   */

  if (o->setid == set->container)
    {
      if ((set->co = malloc(sizeof(o_set))) == NULL)
	SET_LEAVE(set, ERROR_UNKNOWN);

      memcpy(set->co, o, sizeof(o_set));

      SET_LEAVE(set, ERROR_NONE);
    }

  /*
   * 2)
   */

  if (set_add(set->container, o) != ERROR_NONE)
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
 * 1) removing the container object is not allowed.
 * 2) removes the set descriptor from the set container.
 */

t_error			set_delete(t_setid			setid)
{
  SET_ENTER(set);

  /*
   * 1)
   */

  if (setid == set->container)
    {
      cons_msg('!', "set: cannot remove the set container\n");

      SET_LEAVE(set, ERROR_UNKNOWN);
    }

  /*
   * 2)
   */

  if (set_remove(set->container, setid) != ERROR_NONE)
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

t_error			set_descriptor(t_setid			setid,
				       o_set**			o)
{
  SET_ENTER(set);

  /*
   * 1)
   */

  if (setid == set->container)
    {
      *o = set->co;

      SET_LEAVE(set, ERROR_NONE);
    }

  /*
   * 2)
   */

  if (set_get(set->container, setid, (void**)o) != ERROR_NONE)
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

t_error			set_get(t_setid				setid,
				t_id				id,
				void**				o)
{
  t_iterator		iterator;

  SET_ENTER(set);

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
 * 4) if needed, reserves a statistics object.
 * 5) reserves the set container which will contain the set descriptors
 *    reserved later. obviously, it will be better to take a powerful
 *    data structure because it will contains millions of objects.
 * 6) if necessary, dumps the set container.
 */

t_error			set_init(void)
{
  t_setid		needless;

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
      cons_msg('!', "set: unable to initialise the identifier object\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  if (id_reserve(&set->id, &set->container) != ERROR_NONE)
    {
      cons_msg('!', "set: unable to reserve an identifier\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  STATS_RESERVE("set", &set->stats);

  /*
   * 5)
   */

  if (set_reserve(bpt, SET_OPT_CONTAINER | SET_OPT_ALLOC | SET_OPT_SORT,
		  sizeof(o_set), PAGESZ, &needless) != ERROR_NONE)
    {
      cons_msg('!', "set: unable to reserve the set container\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 6)
   */

#if (DEBUG & DEBUG_SET)
  set_dump();

/*  set_test(SET_TYPE_LL);*/
  // XXX-OK set_test(SET_TYPE_BPT);

  set_test(SET_TYPE_ARRAY);
  set_dump();
#endif

  return (ERROR_NONE);
}

/*
 * this function just reinitialises the set manager.
 *
 * steps:
 *
 * 1) releases each set object the set container still contains.
 * 2) releases the set container.
 * 3) if needed, releases the stats object.
 * 4) frees the set container object.
 * 5) destroys the id object.
 * 6) frees the set manager structure's memory.
 */

t_error			set_clean(void)
{
  t_iterator		iterator;

  /*
   * 1)
   */

  while (set_head(set->container, &iterator) == ERROR_NONE)
    {
      o_set*		o;

      if (set_object(set->container, iterator, (void**)&o) != ERROR_NONE)
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

  if (set_release(set->container) != ERROR_NONE)
    {
      cons_msg('!', "set: unable to release the set container\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  STATS_RELEASE(set->stats);

  /*
   * 4)
   */

  free(set->co);

  /*
   * 5)
   */

  if (id_destroy(&set->id) != ERROR_NONE)
    {
      cons_msg('!', "set: unable to destroy the identifier object\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 6)
   */

  free(set);

  return (ERROR_NONE);
}

/*
 * this function tests the set manager
 */

t_error			set_test(t_type				type)
{
  switch (type)
    {
    case SET_TYPE_ARRAY:
      {
	set_test_array(-1);

	break;
      }
    case SET_TYPE_BPT:
      {
	t_uint32	nalloc = alloc_nalloc();
	t_uint32	nfree = alloc_nfree();
	t_iterator	iterator;
	t_id		data;
	t_setid		id;
	t_id		*o;

	cons_msg('#', "testing SET_TYPE_BPT\n");

	if (set_reserve(bpt, SET_OPT_ALLOC | SET_OPT_SORT,
			sizeof(t_uint64), 64, &id) != ERROR_NONE)
	  printf("error: set_reserve()\n");

	data = 98LL;
	if (set_add(id, &data) != ERROR_NONE)
	  printf("error: set_add()\n");

	data = 843536LL;
	if (set_add(id, &data) != ERROR_NONE)
	  printf("error: set_add()\n");

	data = 23987LL;
	if (set_add(id, &data) != ERROR_NONE)
	  printf("error: set_add()\n");

	set_show(id);

	if (set_locate(id, 98LL, &iterator) != ERROR_NONE)
	  printf("error: set_locate()\n");

	if (set_object(id, iterator, (void**)&o) != ERROR_NONE)
	  printf("error: set_object()\n");

	printf("locate(): 0x%x:%u object(): %qu\n",
	       iterator.u.bpt.entry.node, iterator.u.bpt.entry.ndi, *o);

	if (set_get(id, 320498523LL, (void**)&o) == ERROR_NONE)
	  printf("error: set_get(), found a unexisting object\n");

	if (set_get(id, 843536LL, (void**)&o) != ERROR_NONE)
	  printf("error: set_get()\n");

	printf("get(): %qu\n", *o);

	if (set_remove(id, 23987LL) != ERROR_NONE)
	  printf("error: set_remove()\n");

	data = 6354LL;
	if (set_add(id, &data) != ERROR_NONE)
	  printf("error: set_add()\n");

	data = 45897634543LL;
	if (set_add(id, &data) != ERROR_NONE)
	  printf("error: set_add()\n");

	data = 0LL;
	if (set_add(id, &data) != ERROR_NONE)
	  printf("error: set_add()\n");

	if (set_remove(id, 0LL) != ERROR_NONE)
	  printf("error: set_remove()\n");

	set_show(id);

	data = 234LL;
	if (set_add(id, &data) != ERROR_NONE)
	  printf("error: set_add()\n");

	if (set_remove(id, 45897634543LL) != ERROR_NONE)
	  printf("error: set_remove()\n");

	if (set_remove(id, 234LL) != ERROR_NONE)
	  printf("error: set_remove()\n");

	{
	  t_setid new;

	  set_clone(id, &new);

	  set_release(new);
	}

	set_show(id);

	set_flush(id);

	set_show(id);

	printf("id: %qu\n", id);

	set_dump();

	if (set_release(id) != ERROR_NONE)
	  printf("error: set_release()\n");

	set_dump();

	alloc_dump();

	if ((nalloc - nfree) != (alloc_nalloc() - alloc_nfree()))
	  printf("error: memory leaks detected: %u/%u -> %u/%u\n",
		 nalloc, nfree, alloc_nalloc(), alloc_nfree());
	else
	  printf("no memory leak detected\n");

	while (1);

	break;

	/*
	 * XXX
	 *
	 * chaque operation fut validee, et aucune fuite memoire dans set_bpt
	 */
      }
    case SET_TYPE_LL:
      {
	t_uint32	nalloc = alloc_nalloc();
	t_uint32	nfree = alloc_nfree();
	t_iterator	iterator;
	t_id		data;
	t_id		*o;
	t_setid		id;

	/*cons_msg('#', "testing SET_TYPE_LL\n");
*/
	  if (set_reserve(ll, SET_OPT_ALLOC,
			sizeof(t_uint64), &id) != ERROR_NONE)
	  printf("error: set_reserve()\n");

	data = 98LL;
	if (set_add(id, &data) != ERROR_NONE)
	  printf("error: set_add()\n");

	data = 843536LL;
	if (set_add(id, &data) != ERROR_NONE)
	  printf("error: set_add()\n");

	data = 23987LL;
	if (set_add(id, &data) != ERROR_NONE)
	  printf("error: set_add()\n");

	set_show(id);

	if (set_locate(id, 98LL, &iterator) != ERROR_NONE)
	  printf("error: set_locate()\n");

	if (set_object(id, iterator, (void**)&o) != ERROR_NONE)
	  printf("error: set_object()\n");

	printf("locate(): 0x%x object(): %qu\n", iterator.u.ll.node, *o);

	if (set_get(id, 320498523LL, (void**)&o) == ERROR_NONE)
	  printf("error: set_get(), found a unexisting object\n");

	if (set_get(id, 843536LL, (void**)&o) != ERROR_NONE)
	  printf("error: set_get()\n");

	printf("get(): %qu\n", *o);

	if (set_remove(id, 23987LL) != ERROR_NONE)
	  printf("error: set_remove()\n");

	data = 6354LL;
	if (set_add(id, &data) != ERROR_NONE)
	  printf("error: set_add()\n");

	data = 45897634543LL;
	if (set_add(id, &data) != ERROR_NONE)
	  printf("error: set_add()\n");

	data = 0LL;
	if (set_add(id, &data) != ERROR_NONE)
	  printf("error: set_add()\n");

	if (set_remove(id, 0LL) != ERROR_NONE)
	  printf("error: set_remove()\n");

	set_show(id);

	data = 234LL;
	if (set_add(id, &data) != ERROR_NONE)
	  printf("error: set_add()\n");

	if (set_remove(id, 45897634543LL) != ERROR_NONE)
	  printf("error: set_remove()\n");

	if (set_remove(id, 234LL) != ERROR_NONE)
	  printf("error: set_remove()\n");

	data = 78LL;
	if (set_insert_head(id, &data) != ERROR_NONE)
	  printf("error: set_insert_head()\n");

	data = 1LL;
	if (set_insert_tail(id, &data) != ERROR_NONE)
	  printf("error: set_insert_tail()\n");

	if (set_locate(id, 1LL, &iterator) != ERROR_NONE)
	  printf("error: set_locate()\n");

	data = 789456LL;
	if (set_insert_before(id, iterator, &data) != ERROR_NONE)
	  printf("error: set_insert_before()\n");

	if (set_locate(id, 78LL, &iterator) != ERROR_NONE)
	  printf("error: set_locate()\n");

	data = 42LL;
	if (set_insert_before(id, iterator, &data) != ERROR_NONE)
	  printf("error: set_insert_before()\n");

	set_show(id);

	if (set_locate(id, 42LL, &iterator) != ERROR_NONE)
	  printf("error: set_locate()\n");

	data = 123456LL;
	if (set_insert_after(id, iterator, &data) != ERROR_NONE)
	  printf("error: set_insert_after()\n");

	if (set_locate(id, 1LL, &iterator) != ERROR_NONE)
	  printf("error: set_locate()\n");

	data = 21LL;
	if (set_insert_after(id, iterator, &data) != ERROR_NONE)
	  printf("error: set_insert_after()\n");

	set_show(id);

	{
	  t_setid new;

	  set_clone(id, &new);

	  set_show(new);

	  set_release(new);
	}

	set_flush(id);

	set_show(id);

	printf("id: %qu\n", id);

	set_dump();

	if (set_release(id) != ERROR_NONE)
	  printf("error: set_release()\n");

	set_dump();

	alloc_dump();

	if ((nalloc - nfree) != (alloc_nalloc() - alloc_nfree()))
	  printf("error: memory leaks detected: %u/%u -> %u/%u\n",
		 nalloc, nfree, alloc_nalloc(), alloc_nfree());
	else
	  printf("no memory leak detected\n");

/*	while (1);
*/
	break;

	/*
	 * XXX
	 *
	 * chaque operation fut validee, pas de fuites de memoire
	 */
      }
    }

  return (ERROR_NONE);
}
