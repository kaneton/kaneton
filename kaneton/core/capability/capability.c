/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/capability/capability.c
 *
 * created       julien quintard   [sat dec 10 13:56:00 2005]
 * updated       julien quintard   [thu jul  6 17:27:46 2006]
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
 * begins with a 64-bit field describing the node identifier.
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

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the capability manager variable.
 */

m_capability*		capability = NULL;

/*
 * the kernel manager variable.
 */

extern m_kernel*	kernel;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * XXX
 */

t_error			capability_show(t_id			id)
{
  t_capability_descriptor*	descriptor;

  CAPABILITY_ENTER(capability);

  if (set_get(capability->descriptors, id, (void**)&descriptor) != ERROR_NONE)
    CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);

  cons_msg('#', "  [%8qd] node: %qd object: %qd operations: %024b\n",
	   descriptor->id,
	   descriptor->capability.node,
	   descriptor->capability.object,
	   descriptor->capability.operations);

  cons_msg('#', "             check: %qd parent: %qd children:\n",
	   descriptor->capability.check,
	   descriptor->parent);

  if (descriptor->children != ID_UNUSED)
    {
      t_state			state;
      t_id*			data;
      t_iterator		i;

      set_foreach(SET_OPT_FORWARD, descriptor->children, &i, state)
	{
	  if (set_object(descriptor->children, i, (void**)&data) != ERROR_NONE)
	    {
	      cons_msg('!', "capability: cannot find the object "
		       "corresponding to its identifier\n");

	      CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);
	    }

	  cons_msg('#', "             [child] %qd\n", *data);
	}
    }

  CAPABILITY_LEAVE(capability, ERROR_NONE);
}

/*
 * XXX
 */

t_error			capability_dump(void)
{
  t_state			state;
  t_capability_descriptor*	data;
  t_setsz			size;
  t_iterator			i;

  CAPABILITY_ENTER(capability);

  /*
   * 1)
   */

  if (set_size(capability->descriptors, &size) != ERROR_NONE)
    CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);

  /*
   * 2)
   */

  cons_msg('#', "dumping %qu capability descriptor(s) from the "
	   "capability set:\n", size);

  /*
   * 3)
   */

  set_foreach(SET_OPT_FORWARD, capability->descriptors, &i, state)
    {
      if (set_object(capability->descriptors, i, (void**)&data) != ERROR_NONE)
	{
	  cons_msg('!', "capability: cannot find the capability descriptor "
		   "corresponding to its identifier\n");

	  CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);
	}

      if (capability_show(data->id) != ERROR_NONE)
	CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);
    }

  CAPABILITY_LEAVE(capability, ERROR_NONE);
}

/*
 * XXX
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

  CAPABILITY_ENTER(capability);

  /*
   * 1)
   */

  //new->node = kernel->node;
  get_kernel_node(&new->node);
  new->object = object;
  new->operations = operations;
  new->check = 0;

  // XXX should be a 64-bit random number generation
  new->check = (t_uint64)sum2((char*)&new, sizeof(t_capability));

  /*
   * 2)
   */

  descriptor.id = new->check;
  descriptor.capability = *new;
  descriptor.parent = ID_UNUSED;
  descriptor.children = ID_UNUSED;

  if (set_add(capability->descriptors, &descriptor) != ERROR_NONE)
    CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);

  CAPABILITY_LEAVE(capability, ERROR_NONE);
}

/*
 * XXX
 */

t_error			capability_release(t_id			id)
{
  t_capability_descriptor*	descriptor;
  t_state			state;
  t_id*				data;
  t_iterator			i;

  CAPABILITY_ENTER(capability);

  if (capability_get(id, &descriptor) != ERROR_NONE)
    CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);

  if (descriptor->parent != ID_UNUSED)
    CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);

  if (descriptor->children != ID_UNUSED)
    {
      set_foreach(SET_OPT_FORWARD, descriptor->children, &i, state)
	{
	  if (set_object(descriptor->children, i, (void**)&data) != ERROR_NONE)
	    {
	      cons_msg('!', "capability: cannot find the object "
		       "corresponding to its identifier\n");

	      CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);
	    }

	  if (capability_invalidate(descriptor->id, *data) != ERROR_NONE)
	    CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);
	}

      if (set_release(descriptor->children) != ERROR_NONE)
	CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);
    }

  if (set_remove(capability->descriptors, descriptor->id) != ERROR_NONE)
    CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);

  CAPABILITY_LEAVE(capability, ERROR_NONE);
}

/*
 * XXX
 */

t_error			capability_restrict(t_id		id,
					    t_operations	operations,
					    t_capability*	new)
{
  t_capability_descriptor	restricted;
  t_capability_descriptor*	parent;
  t_uint64			xor;

  CAPABILITY_ENTER(capability);

  if (capability_get(id, &parent) != ERROR_NONE)
    CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);

  if ((parent->capability.operations | operations) !=
      parent->capability.operations)
    CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);

  new->node = parent->capability.node;
  new->object = parent->capability.object;
  new->operations = operations;
  new->check = 0;

  // XXX xor between {node,object,operations} and parent->check
  // XXX so between *new and parent->check
  // XXX for the moment only a xor between operations and parent->check
  xor = ((t_uint64)new->operations) ^ parent->capability.check;

  // XXX then the hash function
  new->check = (t_uint64)sum2((char*)&xor, sizeof(t_uint64));

  /*
   * 2)
   */

  restricted.id = new->check;
  restricted.capability = *new;
  restricted.parent = parent->id;
  restricted.children = ID_UNUSED;

  if (set_add(capability->descriptors, &restricted) != ERROR_NONE)
    CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);

  if (parent->children == ID_UNUSED)
    {
      if (set_reserve(array, SET_OPT_ALLOC, CAPABILITY_CHILDREN_INITSZ,
		      sizeof(t_id), &parent->children) != ERROR_NONE)
	{
	  CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);
	}
    }

  if (set_add(parent->children, &restricted.id) != ERROR_NONE)
    {
      CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);
    }

  CAPABILITY_LEAVE(capability, ERROR_NONE);
}

/*
 * XXX
 */

t_error			capability_invalidate(t_id		p,
					      t_id		c)
{
  t_capability_descriptor*	restricted;
  t_capability_descriptor*	parent;
  t_state			state;
  t_id*				data;
  t_iterator			i;

  CAPABILITY_ENTER(capability);

  if (capability_get(p, &parent) != ERROR_NONE)
    CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);

  if (capability_get(c, &restricted) != ERROR_NONE)
    CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);

  if (restricted->parent != parent->id)
    CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);

  if (set_remove(parent->children, restricted->id) != ERROR_NONE)
    CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);

  if (restricted->children != ID_UNUSED)
    {
      set_foreach(SET_OPT_FORWARD, restricted->children, &i, state)
	{
	  if (set_object(restricted->children, i, (void**)&data) != ERROR_NONE)
	    {
	      cons_msg('!', "capability: cannot find the object "
		       "corresponding to its identifier\n");

	      CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);
	    }

	  if (capability_invalidate(c, *data) != ERROR_NONE)
	    CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);
	}

      if (set_release(restricted->children) != ERROR_NONE)
	CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);
    }

  if (set_remove(capability->descriptors, restricted->id) != ERROR_NONE)
    CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);

  CAPABILITY_LEAVE(capability, ERROR_NONE);
}

/*
 * XXX
 */

t_error			capability_get(t_id			id,
				       t_capability_descriptor** descriptor)
{
  CAPABILITY_ENTER(capability);

  if (set_get(capability->descriptors, id, (void**)descriptor) != ERROR_NONE)
    CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);

  CAPABILITY_LEAVE(capability, ERROR_NONE);
}

/*
 * XXX
 */

t_error			capability_verify(t_capability*		provided)
{
  t_capability_descriptor*	current;

  CAPABILITY_ENTER(capability);

  if (capability_get((t_id)provided->check, &current) != ERROR_NONE)
    CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);

  //if (provided->node != current->capability.node)
  if (!node_cmp(provided->node, current->capability.node))
    CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);

  if (provided->object != current->capability.object)
    CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);

  if (current->parent == ID_UNUSED)
    {
      if (provided->operations != current->capability.operations)
	CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);

      if (provided->check != current->capability.check)
	CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);
    }
  else
    {
      // XXX t_capability		verification;
      t_capability_descriptor*	parent;
      t_uint64			xor;

      t_uint64			verification; // XXX

      if (capability_get((t_id)current->parent, &parent) != ERROR_NONE)
	CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);

      // XXX O' ^ C
      xor =
	((t_uint64)current->capability.operations) ^ parent->capability.check;

      // XXX hash(xor)
      verification = sum2((char*)&xor, sizeof(t_uint64));

      if (verification != current->capability.check)
	CAPABILITY_LEAVE(capability, ERROR_UNKNOWN);
    }

  CAPABILITY_LEAVE(capability, ERROR_NONE);
}

/*
 * XXX
 */

t_error			capability_init(void)
{
  /*
   * 1)
   */

  if ((capability = malloc(sizeof(m_capability))) == NULL)
    {
      cons_msg('!', "capability: cannot allocate memory for the capability "
	       "manager structure\n");

      return (ERROR_UNKNOWN);
    }

  memset(capability, 0x0, sizeof(m_capability));

  /*
   * 2)
   */

  if (set_reserve(ll, SET_OPT_ALLOC | SET_OPT_SORT,
		  sizeof(t_capability_descriptor),
		  &capability->descriptors) != ERROR_NONE)
    {
      cons_msg('!', "capability: unable to reserve the capability "
	       "descriptors set\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  STATS_RESERVE("capability", &capability->stats);

  /*
   * 4)
   */

#if (DEBUG & DEBUG_CAPABILITY)
  capability_dump();
#endif

#if 0 // XXX
  t_capability c;
  t_capability r1;
  t_capability r2;
  t_capability f;

  capability_reserve(42, 0x17, &c);

  capability_restrict((t_id)c.check, 0x11, &r1);
  capability_restrict((t_id)c.check, 0x7, &r2);

  capability_restrict((t_id)r1.check, 0x1, &f);

  capability_dump();

  capability_verify(&c);
  capability_verify(&f);
  capability_verify(&r1);
  capability_verify(&r2);
#endif

  return (ERROR_NONE);
}

/*
 * XXX
 */

t_error			capability_clean(void)
{
  t_capability_descriptor*	data;
  t_iterator			i;

  /*
   * 1)
   */

  STATS_RELEASE(capability->stats);

  /*
   * 2)
   */

  while (set_head(capability->descriptors, &i) == ERROR_NONE)
    {
      if (set_object(capability->descriptors, i, (void**)&data) != ERROR_NONE)
	{
	  cons_msg('!', "capability: cannot find the capability descriptor "
		   "corresponding to its identifier\n");

	  return (ERROR_UNKNOWN);
	}

      if (capability_release(data->id) != ERROR_NONE)
	return (ERROR_UNKNOWN);
    }

  if (set_release(capability->descriptors) != ERROR_NONE)
    {
      cons_msg('!', "task: unable to release the capability descriptor set\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  free(capability);

  return (ERROR_NONE);
}

// XXX probleme: comment liberer les capabilities
// XXX une tache a une capability sur sa tache, puis elle demande d'obtenir
// XXX une nouvelle cap sur son as. puis cette tache restrict cette nouvelle
// XXX cap et la donne a une autre tache pour que celle ci ne puisse que
// XXX lire son as, par exemple pour debugger mais sans intervenir sur
// XXX l'etat, sans modifier l'as.
// XXX quand la tache meurt, comment savoir quelles sont les capabilities
// XXX qu il faut detruire.
// XXX soit on garde un ensemble de cap generees pour une tache
// XXX soit on organise les cap descriptors par object id et quand une tache
// XXX meurt on parcours le set de cap en fonction des id. le probleme
// XXX de cette derniere solution c'est qu il va falloir le faire pour
// XXX le taskid, l'asid, chaque threadid puis chaque segid. donc c est long!
// XXX -> ajouter un set de capability dans o_task pour pouvoir les liberer

// XXX il faudrait ajouter des operations genre par exemple justement les
// XXX oeprations sur les capabilities. si on forge une nouvelle cap, il
// XXX ne faut pas que si on la distribue a A et a B, a la release car
// XXX B ne pourra plus s'en servir.
// XXX soit on rajoute des op, soit interdit de faire release sur sa cap
// XXX sauf pour la cap racine.
// XXX -> pour l'instant on interdit un release sur une restricted cap

// XXX gerer les collisions avec salt

// XXX si erreur, alors liberer ce qui vient d etre construit

// XXX changer car actuellement on a pas de fonctions de hash ni ce qu'il
// XXX faut pour faire un xor correct sur des donnees.
// XXX actuellement on a le meme systeme que Amoeba et donc failles de secu.

// XXX changer les protos et eviter les arg p, c etc.., il faut toujours
// que les args d'un proto soit bien nomme
