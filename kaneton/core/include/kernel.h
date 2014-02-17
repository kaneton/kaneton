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
 * updated       julien quintard   [fri dec 10 21:18:18 2010]
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
 * the kernel manager's structure.
 *
 * this structure contains the kernel's machine, task, address space and
 * thread identifiers. finally the node identifier is also included though
 * based on the previous identifiers.
 *
 * indeed, the machine identifier is a random number generated at the boot
 * time. since this is a 64-bit number, the chances of machine identifiers
 * collisions inside a small distributed system (less than a thousand
 * machines) are relatively low.
 *
 * the task, address space and thread identifiers are used to retrieve the
 * respective kernel objects.
 *
 * finally, the node identifier is used to identify a task within the
 * distributed system. this identifier is for instance used to send messages
 * to task of other machines of the network. in other words, every task
 * can be identified either by its task identifier on the machine or by
 * its node identifier in the network. since the kernel is a task very
 * much like the others, it is also reachable through its node identifier
 * which is included in the kernel manager's structure below.
 */

typedef struct
{
  i_cell			cell;
  i_task			task;
  i_as				as;
  i_thread			thread;

  i_node			node;

  machine_data(m_kernel);
}				m_kernel;

/*
 * the kernel dispatcher.
 */

typedef struct
{
  t_status		(*kernel_dump)(void);
  t_status		(*kernel_initialize)(void);
  t_status		(*kernel_clean)(void);
}				d_kernel;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/kernel/kernel.c
 */

/*
 * ../../core/kernel/kernel.c
 */

t_status		kernel_dump(void);

t_status		kernel_initialize(void);

t_status		kernel_clean(void);


/*
 * eop
 */

#endif
