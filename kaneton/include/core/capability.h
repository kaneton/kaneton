/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/capability.h
 *
 * created       julien quintard   [wed jun  6 12:44:04 2007]
 * updated       julien quintard   [wed jun  6 15:55:35 2007]
 */

#ifndef GUARD_CORE_CAPABILITY
#define GUARD_CORE_CAPABILITY		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/core.h>
#include <core/id.h>
#include <core/kernel.h>
#include <core/error.h>

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

struct				s_capability
{
  i_node			node;
  t_id				object;
  t_operations			operations;
  t_id				descriptor;

  // machdep_data(t_capability);
};

/*
 * a capability descriptor contains the check field of the generated
 * capability, the capability descriptors' identifier of the parent
 * capability used to build this capability and finally a set of children
 * capabilities generated from this capbility.
 */

struct				s_capability_descriptor
{
  t_id				id;

  t_capability			capability;
  t_uint64			check;

  t_id				parent;
  i_set				children;
};

/*
 * the capability manager structure
 */

struct				sm_capability
{
  i_stats			stats;

  i_set				descriptors;

  t_error			(*f_checksum)(char* data,
					      t_uint32 size,
					      t_uint64* res);

  // machdep_data(m_capability);
};

/*
 * the kernel architecture dependent interface
 */

struct				sd_capability
{
  /* XXX */
};

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define CAPABILITY_CHECK(_capability_)					\
  {									\
    if ((_capability_) == NULL)						\
      return (ERROR_UNKNOWN);						\
  }

/*
 * enter
 */

#define CAPABILITY_ENTER(_capability_)					\
  {									\
    CAPABILITY_CHECK((_capability_));					\
									\
    STATS_BEGIN((_capability_)->stats);					\
  }

/*
 * leave
 */

#define CAPABILITY_LEAVE(_capability_, _error_)				\
  {									\
    STATS_END((_capability_)->stats, (_error_));			\
									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/capability/capability.c
 */

/*
 * ../../core/capability/capability.c
 */

t_error			capability_show(t_id			id);

t_error			capability_dump(void);

t_error			capability_reserve(t_id			object,
					   t_operations		operations,
					   t_capability*	new);

t_error			capability_release(t_id			id);

t_error			capability_restrict(t_id		id,
					    t_operations	operations,
					    t_capability*	new);

t_error			capability_invalidate(t_id		p,
					      t_id		c);

t_error			capability_get(t_id			id,
				       t_capability_descriptor** descriptor);

t_error			capability_give(t_id			id,
					i_node			node);

t_error			capability_verify(t_capability*		provided);

t_error			capability_init(void);

t_error			capability_clean(void);


/*
 * eop
 */

#endif
