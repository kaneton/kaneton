/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/kernel.h
 *
 * created       julien quintard   [wed jun  6 13:27:34 2007]
 * updated       julien quintard   [wed jun  6 21:24:16 2007]
 */

#ifndef GUARD_CORE_KERNEL
#define GUARD_CORE_KERNEL		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

// XXX #include <core/task.h>


/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * kernel manager
 *
 * this structure contains the machine identifier and the node identifier.
 *
 * the machine identifier is a random number generated at the boot time.
 * since this is a 64-bit number, there is no chance that two machines
 * get the same machine identifier.
 *
 * the node identifier is used to identify a task which provides capabilities.
 * in other words every microkernel servers will need a node id. we use
 * the term node in reference to the distributed system.
 */

/*
 * the kernel manager structure
 */

typedef struct
{
  t_id				machine;

  // XXX machdep_data(m_kernel);
}				m_kernel;

/*
 * the kernel architecture dependent interface
 */

typedef struct
{
  /* XXX */
}				d_kernel;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/kernel/kernel.c
 */

/*
 * ../../core/kernel/kernel.c
 */

void			kernel_init(void);

void			kernel_clean(void);


/*
 * eop
 */

#endif
