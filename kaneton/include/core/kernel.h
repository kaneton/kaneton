/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/kernel.h
 *
 * created       julien quintard   [sat jun 24 13:57:54 2006]
 * updated       julien quintard   [sat jun 24 14:10:18 2006]
 */

#ifndef CORE_KERNEL_H
#define CORE_KERNEL_H		1

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

typedef t_id i_machine;

typedef struct
{
  t_id				machine;

  // machdep_data(m_kernel);
}				m_kernel;

typedef struct
{
  i_machine			machine;
  i_task			task;
}				i_node;

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
