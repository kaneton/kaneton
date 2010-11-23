/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...NEW/kaneton/core/capability/capability.c
 *
 * created       julien quintard   [sun jun  3 19:48:52 2007]
 * updated       julien quintard   [mon nov 22 12:14:50 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the capability manager provides an interface to manipulate the
 * capabilities.
 *
 * although the capabilities over the distributed system will have different
 * formats, the kaneton core uses a unique capability format for every
 * kaneton object.
 *
 * note that every capability format over the distributed system must
 * begins with a 128-bit field describing the node identifier.
 *
 * the kaneton core capability format is composed of a node identifier on
 * 64 bits specifying the task of the distributed system which had generated
 * this capability, an object identifier on 64 bits, an operation field on
 * 32 bits describing the allowed operations and finally a check field on
 * 64 bits to achieve security of the capability.
 *
 * the node identifier is used to locate a service independently of the
 * machine on which it resides. indeed, if a task is migrated on another
 * machine, the node identifier of this task will remain the same. then
 * the user programs of the service will continue to work properly.
 *
 * to verify the correctness of the capabilities used by the user programs,
 * the core needs to store the check fields of each capability previously
 * generated. these check fields will be stored in set objects although
 * a check field is not a kaneton object. indeed, an identifier is
 * included in the capability descriptor structure and will take the value
 * of the check field since it is a 64-bit number too. in addition, other
 * informations are stored with each check field, including the capability
 * descriptor's identifier of the capability from which this capability was
 * built and a set of capabilities generated from this capability.
 *
 * recall that the capabilities permit a program to restrict the permissions
 * of a capability and then to distribute this new generated capability to
 * other tasks. but the kaneton capability system also provides other
 * interesting features including the possibility to restrict a previously
 * restricted capability and so on but also to invalidate a restricted
 * capability. this last feature is very interesting because it permit
 * a program to distribute privileges and then to simply forbid the use
 * of capabilities it previously forged.
 *
 * finally, to avoid check fields collisions, a salt is used.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the capability manager variable.
 */

m_capability*		_capability = NULL;

/*
 * the kernel manager variable.
 */

extern m_kernel*	_kernel;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * Make a checksum on a capability, this one is simple but can be easily change
 */
// XXX
static t_error		simple_checksum(char*			data,
					t_uint32		size,
					t_uint64*		res)
{
  CAPABILITY_ENTER(_capability);

  *res = (t_uint64)sum2((char*)&data, size);

  CAPABILITY_LEAVE(_capability, ERROR_OK);
}

/*
 * Display a capability, used for debugging purpose
 */

t_error			capability_show(t_id			id)
{
  t_capability_descriptor*	descriptor;

  CAPABILITY_ENTER(_capability);

  if (set_get(_capability->descriptors, id, (void**)&descriptor) != ERROR_OK)
    CAPABILITY_LEAVE(_capability, ERROR_KO);

  module_call(console, console_message,
	      '#', "  [%8qd] node: %qd object: %qd operations: %024b\n",
	      descriptor->id,
	      descriptor->capability.node,
	      descriptor->capability.object,
	      descriptor->capability.operations);

  module_call(console, console_message,
	      '#', "             check: %qd parent: %qd children:\n",
	      descriptor->check,
	      descriptor->parent);

  if (descriptor->children != ID_UNUSED)
    {
      t_state			state;
      t_id*			data;
      t_iterator		i;

      set_foreach(SET_OPTION_FORWARD, descriptor->children, &i, state)
	{
	  if (set_object(descriptor->children, i, (void**)&data) != ERROR_OK)
	    {
	      module_call(console, console_message,
			  '!', "capability: cannot find the object "
			  "corresponding to its identifier\n");

	      CAPABILITY_LEAVE(_capability, ERROR_KO);
	    }

	  module_call(console, console_message,
		      '#', "             [child] %qd\n", *data);
	}
    }

  CAPABILITY_LEAVE(_capability, ERROR_OK);
}

/*
 * Dumps the whole capabilities in the capability set
 *
 * 1) Gets the size of the capability set
 * 2) Displays the numbers of capabilities
 * 3) Displays all the capabilities
displays
 */

t_error			capability_dump(void)
{
  t_state			state;
  t_capability_descriptor*	data;
  t_setsz			size;
  t_iterator			i;

  CAPABILITY_ENTER(_capability);

  /*
   * 1)
   */

  if (set_size(_capability->descriptors, &size) != ERROR_OK)
    CAPABILITY_LEAVE(_capability, ERROR_KO);

  /*
   * 2)
   */

  module_call(console, console_message,
	      '#', "dumping %qu capability descriptor(s) from the "
	      "capability set:\n", size);

  /*
   * 3)
   */

  set_foreach(SET_OPTION_FORWARD, _capability->descriptors, &i, state)
    {
      if (set_object(_capability->descriptors, i, (void**)&data) != ERROR_OK)
	{
	  module_call(console, console_message,
		      '!', "capability: cannot find the capability descriptor "
		      "corresponding to its identifier\n");

	  CAPABILITY_LEAVE(_capability, ERROR_KO);
	}

      if (capability_show(data->id) != ERROR_OK)
	CAPABILITY_LEAVE(_capability, ERROR_KO);
    }

  CAPABILITY_LEAVE(_capability, ERROR_OK);
}


/*
 * Reserves a capability
 *
 * 1) Setting the values of the capability
 * 2) Getting an id for the capability descriptor
 * 3) Setting the values of the capability descriptor and calculating the checksum
 * 4) Adding the capability to the set
 */

  /*
   * XXX the children set object is not reserved yet since it is likely that no
   * capabilities will be restricted. so, to avoid wasting space, no set
   * is reserved.
   */

t_error			capability_reserve(t_id			object,
					   t_operations		operations,
					   t_capability*	new)
{
  t_capability_descriptor	descriptor;

  CAPABILITY_ENTER(_capability);

  /*
   * 1)
   */

  new->node.machine = _kernel->machine;
  new->node.task = _kernel->task;
  new->object = object;
  new->operations = operations;

  /*
   * 2)
   */
  // XXX should be a 64-bit random number generation
  new->descriptor = (t_uint64)sum2((char*)&new, sizeof(t_capability));

  /*
   * 3)
   */

  descriptor.id = new->descriptor;
  if (_capability->f_checksum((char *)new, sizeof (t_capability), &descriptor.check) !=
      ERROR_OK)
    CAPABILITY_LEAVE(_capability, ERROR_KO);
  descriptor.capability = *new;
  descriptor.parent = ID_UNUSED;
  descriptor.children = ID_UNUSED;

  /*
   * 4)
   */
  if (set_add(_capability->descriptors, &descriptor) != ERROR_OK)
    CAPABILITY_LEAVE(_capability, ERROR_KO);

  CAPABILITY_LEAVE(_capability, ERROR_OK);
}

/*
 * Releases a capability descriptor, if the capability has children it won't be deleted
 *
 * 1) Gets ths descriptor and then check if it has a parent
 * 2) Invalidates all the children of the released capability
 * 3) Removes the capability from the capability descriptor set
 */

t_error			capability_release(t_id			id)
{
  t_capability_descriptor*	descriptor;
  t_state			state;
  t_id*				data;
  t_iterator			i;

  CAPABILITY_ENTER(_capability);

  /*
   * 1)
   */

  if (capability_get(id, &descriptor) != ERROR_OK)
    CAPABILITY_LEAVE(_capability, ERROR_KO);

  if (descriptor->parent != ID_UNUSED)
    CAPABILITY_LEAVE(_capability, ERROR_KO);

  /*
   * 2)
   */
  if (descriptor->children != ID_UNUSED)
    {
      set_foreach(SET_OPTION_FORWARD, descriptor->children, &i, state)
	{
	  if (set_object(descriptor->children, i, (void**)&data) != ERROR_OK)
	    {
	      module_call(console, console_message,
			  '!', "capability: cannot find the object "
			  "corresponding to its identifier\n");

	      CAPABILITY_LEAVE(_capability, ERROR_KO);
	    }

	  if (capability_invalidate(descriptor->id, *data) != ERROR_OK)
	    CAPABILITY_LEAVE(_capability, ERROR_KO);
	}

      if (set_release(descriptor->children) != ERROR_OK)
	CAPABILITY_LEAVE(_capability, ERROR_KO);
    }

  /*
   * 3)
   */
  if (set_remove(_capability->descriptors, descriptor->id) != ERROR_OK)
    CAPABILITY_LEAVE(_capability, ERROR_KO);

  CAPABILITY_LEAVE(_capability, ERROR_OK);
}

/*
 * Creates a new restricted capability from another capability
 *
 * 1) Gets the capability
 * 2) Checking that operations are really restriction of the parent capability
 * 3) Setting the values of the new capability and its associated descriptor
 * 4) Reserve a set on the parent capability if it doesn't have any child and
 *    add the new capability to the set of children of the parent
 */

t_error			capability_restrict(t_id		id,
					    t_operations	operations,
					    t_capability*	new)
{
  t_capability_descriptor	restricted;
  t_capability_descriptor*	parent;

  CAPABILITY_ENTER(_capability);

  /*
   * 1)
   */
  if (capability_get(id, &parent) != ERROR_OK)
    CAPABILITY_LEAVE(_capability, ERROR_KO);

  /*
   * 2)
   */
  if ((parent->capability.operations | operations) !=
      parent->capability.operations)
    CAPABILITY_LEAVE(_capability, ERROR_KO);

  /*
   * 3)
   */
  new->node = parent->capability.node;
  new->object = parent->capability.object;
  new->operations = operations;

  // XXX should be a 64-bit random number generation
  new->descriptor = (t_uint64)sum2((char*)&new, sizeof(t_capability));

  restricted.id = new->descriptor;
  if (_capability->f_checksum((char *)new, sizeof (t_capability), &restricted.check) !=
      ERROR_OK)
    CAPABILITY_LEAVE(_capability, ERROR_KO);
  restricted.capability = *new;
  restricted.parent = parent->id;
  restricted.children = ID_UNUSED;

  if (set_add(_capability->descriptors, &restricted) != ERROR_OK)
    CAPABILITY_LEAVE(_capability, ERROR_KO);


  /*
   * 4)
   */
  if (parent->children == ID_UNUSED)
      if (set_reserve(array, SET_OPTION_ALLOC, CAPABILITY_CHILDREN_INITSZ,
		      sizeof(t_id), &parent->children) != ERROR_OK)
	  CAPABILITY_LEAVE(_capability, ERROR_KO);

  if (set_add(parent->children, &restricted.id) != ERROR_OK)
    {
      CAPABILITY_LEAVE(_capability, ERROR_KO);
    }

  CAPABILITY_LEAVE(_capability, ERROR_OK);
}

/*
 * Invalidates a previously restricted capability
 */

t_error			capability_invalidate(t_id		p,
					      t_id		c)
{
  t_capability_descriptor*	restricted;
  t_capability_descriptor*	parent;
  t_state			state;
  t_id*				data;
  t_iterator			i;

  CAPABILITY_ENTER(_capability);

  if (capability_get(p, &parent) != ERROR_OK)
    CAPABILITY_LEAVE(_capability, ERROR_KO);

  if (capability_get(c, &restricted) != ERROR_OK)
    CAPABILITY_LEAVE(_capability, ERROR_KO);

  if (restricted->parent != parent->id)
    CAPABILITY_LEAVE(_capability, ERROR_KO);

  if (set_remove(parent->children, restricted->id) != ERROR_OK)
    CAPABILITY_LEAVE(_capability, ERROR_KO);

  if (restricted->children != ID_UNUSED)
    {
      set_foreach(SET_OPTION_FORWARD, restricted->children, &i, state)
	{
	  if (set_object(restricted->children, i, (void**)&data) != ERROR_OK)
	    {
	      module_call(console, console_message,
			  '!', "capability: cannot find the object "
			  "corresponding to its identifier\n");

	      CAPABILITY_LEAVE(_capability, ERROR_KO);
	    }

	  if (capability_invalidate(c, *data) != ERROR_OK)
	    CAPABILITY_LEAVE(_capability, ERROR_KO);
	}

      if (set_release(restricted->children) != ERROR_OK)
	CAPABILITY_LEAVE(_capability, ERROR_KO);
    }

  if (set_remove(_capability->descriptors, restricted->id) != ERROR_OK)
    CAPABILITY_LEAVE(_capability, ERROR_KO);

  CAPABILITY_LEAVE(_capability, ERROR_OK);
}

/*
 * Gets a capability descriptor corresponding to an id
 */

t_error			capability_get(t_id			id,
				       t_capability_descriptor** descriptor)
{
  CAPABILITY_ENTER(_capability);

  if (set_get(_capability->descriptors, id, (void**)descriptor) != ERROR_OK)
    CAPABILITY_LEAVE(_capability, ERROR_KO);

  CAPABILITY_LEAVE(_capability, ERROR_OK);
}


/*
 * Gives a capability to another node
 * 1) Gets the descriptor
 * 2) Sets the new node in the capability
 * 3) Updates the checksum
 */

t_error			capability_give(t_id			id,
					i_node			node)
{
  t_capability_descriptor* descriptor;

  CAPABILITY_ENTER(_capability);

  /*
   * 1)
   */
  if (capability_get(id, &descriptor) != ERROR_OK)
    CAPABILITY_LEAVE(_capability, ERROR_KO);

  /*
   * 2)
   */
  descriptor->capability.node = node;

  /*
   * 3)
   */
  if (_capability->f_checksum((char *)&descriptor->capability,
			     sizeof (t_capability),
			     &descriptor->check) != ERROR_OK)
    CAPABILITY_LEAVE(_capability, ERROR_KO);

  CAPABILITY_LEAVE(_capability, ERROR_OK);
}


/*
 * Vefifies if a capability is correct by checking the value of the checksum
 * 1) Gets the descriptor
 * 2) Generates a correct checksum for the provided capability
 * 3) Compares the generated checksum with the one saved in the descriptor
 */

t_error			capability_verify(t_capability*		provided)
{
  t_capability_descriptor*	current;
  t_uint64			check;

  CAPABILITY_ENTER(_capability);

  /*
   * 1)
   */
  if (capability_get((t_id)provided->descriptor, &current) != ERROR_OK)
    CAPABILITY_LEAVE(_capability, ERROR_KO);

  /*
   * 2)
   */
  if (_capability->f_checksum((char *)provided, sizeof (t_capability), &check) !=
      ERROR_OK)
    CAPABILITY_LEAVE(_capability, ERROR_KO);

  /*
   * 3)
   */
  if (check != current->check)
    CAPABILITY_LEAVE(_capability, ERROR_KO);

  CAPABILITY_LEAVE(_capability, ERROR_OK);
}


/*
 * Init
 */

t_error			capability_initialize(void)
{
  /*
   * 1)
   */

  if ((_capability = malloc(sizeof(m_capability))) == NULL)
    {
      module_call(console, console_message,
		  '!', "capability: cannot allocate memory for the capability "
		  "manager structure\n");

      return (ERROR_KO);
    }

  memset(_capability, 0x0, sizeof(m_capability));

  /*
   * 2)
   */

  if (set_reserve(ll, SET_OPTION_ALLOC | SET_OPTION_SORT,
		  sizeof(t_capability_descriptor),
		  &_capability->descriptors) != ERROR_OK)
    {
      module_call(console, console_message,
		  '!', "capability: unable to reserve the capability "
		  "descriptors set\n");

      return (ERROR_KO);
    }

  /*
   * 3)
   */
  _capability->f_checksum = simple_checksum;

#if (DEBUG & DEBUG_CAPABILITY)
  capability_dump();
#endif

#if 0 // XXX
  t_capability c;
  t_capability r1;
  t_capability r2;
  t_capability f;

  capability_reserve(42, 0x17, &c);

  capability_restrict((t_id)c.descriptor, 0x11, &r1);
  capability_restrict((t_id)c.descriptor, 0x7, &r2);

  capability_restrict((t_id)r1.descriptor, 0x1, &f);

  capability_dump();

  capability_verify(&c);
  capability_verify(&f);
  capability_verify(&r1);
  capability_verify(&r2);
#endif

  return (ERROR_OK);
}

/*
 * Cleaning
 */

t_error			capability_clean(void)
{
  t_capability_descriptor*	data;
  t_iterator			i;

  /*
   * 1)
   */

  while (set_head(_capability->descriptors, &i) == ERROR_OK)
    {
      if (set_object(_capability->descriptors, i, (void**)&data) != ERROR_OK)
	{
	  module_call(console, console_message,
		      '!', "capability: cannot find the capability descriptor "
		      "corresponding to its identifier\n");

	  return (ERROR_KO);
	}

      if (capability_release(data->id) != ERROR_OK)
	return (ERROR_KO);
    }

  if (set_release(_capability->descriptors) != ERROR_OK)
    {
      module_call(console, console_message,
		  '!', "task: unable to release the capability "
		  "descriptor set\n");

      return (ERROR_KO);
    }

  /*
   * 2)
   */

  free(_capability);

  return (ERROR_OK);
}

// XXX si erreur, alors liberer ce qui vient d etre construit

// XXX changer car actuellement on a pas de fonctions de hash correct

// XXX fonction random pour les id ou pas? cela est-il vraiment necessaire?
