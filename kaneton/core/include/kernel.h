/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/kernel.h
 *
 * created       julien quintard   [wed jun  6 13:27:34 2007]
 * updated       julien quintard   [thu jun  7 16:20:10 2007]
 */

#ifndef CORE_KERNEL_H
#define CORE_KERNEL_H			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/error.h>
#include <core/id.h>

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

  /* XXX machdep_data(m_kernel); */
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
