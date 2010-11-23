/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.NEW/kaneton/core/include/kernel.h
 *
 * created       julien quintard   [wed jun  6 13:27:34 2007]
 * updated       julien quintard   [tue nov 23 14:45:44 2010]
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
  /* XXX: node cf: kernel.c */

  i_task			task;
  i_as				as;
  i_thread			thread;

  /* XXX machine_data(m_kernel); */
}				m_kernel;

/*
 * the kernel architecture dependent interface
 */

typedef struct
{
  /* XXX */
}				d_kernel;

/*
 * ---------- macro functions -------------------------------------------------
 */

#define kernel_error(_fmt_, _arguments_...)				\
  {									\
    module_call(report, report_dump);					\
									\
    printf("[!] " _fmt_ " (%s:%u)\n",					\
           ##_arguments_, __FUNCTION__, __LINE__);			\
									\
    while (1)								\
      ;									\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/kernel/kernel.c
 */

/*
 * ../../core/kernel/kernel.c
 */

void			kernel_initialize(void);

void			kernel_clean(void);


/*
 * eop
 */

#endif
