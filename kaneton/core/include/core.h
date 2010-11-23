/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.NEW/kaneton/core/include/core.h
 *
 * created       julien quintard   [wed jun  6 12:22:26 2007]
 * updated       julien quintard   [mon nov 22 22:58:19 2010]
 */

#ifndef CORE_CORE_H
#define CORE_CORE_H			1

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * booleans
 */

#define BOOLEAN_TRUE		1
#define BOOLEAN_FALSE		0

/*
 * permissions
 */

#define PERMISSION_READ		(1 << 0)
#define PERMISSION_WRITE	(1 << 1)
#define PERMISSION_EXEC		(1 << 2)

#define PERMISSION_INVALID	(~((PERMISSION_READ) |			\
				   (PERMISSION_WRITE) |			\
				   (PERMISSION_EXEC)))

/*
 * fits
 */

#define FIT_FIRST		(1 << 0)
#define FIT_BEST		(1 << 1)
#define FIT_NEXT		(1 << 2)
#define FIT_WORST		(1 << 3)

/*
 * ---------- includes --------------------------------------------------------
 */

#include <core/types.h>
#include <core/error.h>
#include <core/id.h>

#include <core/init.h>

#include <core/set.h>
#include <core/kernel.h>
#include <core/segment.h>
#include <core/region.h>
#include <core/as.h>
#include <core/map.h>
#include <core/event.h>
#include <core/timer.h>
#include <core/io.h>
#include <core/wait.h>
#include <core/thread.h>
#include <core/task.h>
#include <core/cpu.h>
#include <core/scheduler.h>
#include <core/message.h>
#include <core/capability.h>

/* XXX badly generated: rethink the syntax */
#include <core/interface.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/core.c
 */

/*
 * ../../core/core.c
 */

void			kaneton(t_init*				bootloader);

t_error			kaneton_spawn(void);


/*
 * eop
 */

#endif
