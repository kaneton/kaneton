/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/capability.h
 *
 * created       julien quintard   [sat jun 24 14:10:24 2006]
 * updated       julien quintard   [sat jun 24 14:28:50 2006]
 */

#ifndef CORE_CAPABILITY_H
#define CORE_CAPABILITY_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <core/id.h>
#include <core/types.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/* XXX */

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * a capability
 */

typedef struct
{
  t_uint64			check;
  t_id				node;
  t_id				object;
  t_uint16			operations;

  // machdep_data(t_capability);
}				t_capability;

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

t_error			capability_reserve(void);

t_error			capability_release(void);

t_error			capability_clone(void);

t_error			capability_init(void);

t_error			capability_clean(void);


/*
 * eop
 */

#endif
