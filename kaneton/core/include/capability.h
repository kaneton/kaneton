/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/capability.h
 *
 * created       julien quintard   [wed jun  6 12:44:04 2007]
 * updated       julien quintard   [thu dec  9 15:09:33 2010]
 */

#ifndef CORE_CAPABILITY_H
#define CORE_CAPABILITY_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/error.h>
#include <core/id.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * init sizes for the array data structure set
 */

#define CAPABILITY_CHILDREN_INITSZ	0x4

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * a capability
 *
 * this is the structure manipulated by the programs.
 */

typedef struct
{
  i_node			node;
  t_id				object;
  t_operations			operations;
  t_id				descriptor;

  // machine_data(t_capability);
}				t_capability; // XXX s_capability

/*
 * a capability descriptor contains the check field of the generated
 * capability, the capability descriptors' identifier of the parent
 * capability used to build this capability and finally a set of children
 * capabilities generated from this capbility.
 */

typedef struct
{
  t_id				id;

  t_capability			capability;
  t_uint64			check;

  t_id				parent;
  i_set				children;
}				t_capability_descriptor; // XXX s_...

/*
 * the capability manager structure
 */

typedef struct
{
  i_set				descriptors;

  t_status		(*f_checksum)(char* data,
					      t_uint32 size,
					      t_uint64* res);

  // machine_data(m_capability);
}				m_capability;

/*
 * the kernel architecture dependent interface
 */

typedef struct
{
  /* XXX */
}				d_capability;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/capability/capability.c
 */

/*
 * ../../core/capability/capability.c
 */

t_status		capability_show(t_id			id);

t_status		capability_dump(void);

t_status		capability_reserve(t_id			object,
					   t_operations		operations,
					   t_capability*	new);

t_status		capability_release(t_id			id);

t_status		capability_restrict(t_id		id,
					    t_operations	operations,
					    t_capability*	new);

t_status		capability_invalidate(t_id		p,
					      t_id		c);

t_bool			capability_exist(t_id			id);

t_status		capability_get(t_id			id,
				       t_capability_descriptor** descriptor);

t_status		capability_give(t_id			id,
					i_node			node);

t_status		capability_verify(t_capability*		provided);

t_status		capability_initialize(void);

t_status		capability_clean(void);


/*
 * eop
 */

#endif
