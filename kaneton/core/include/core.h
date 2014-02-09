/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/core.h
 *
 * created       julien quintard   [wed jun  6 12:22:26 2007]
 * updated       julien quintard   [tue feb  8 22:19:49 2011]
 */

#ifndef CORE_CORE_H
#define CORE_CORE_H			1

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * endianness
 */

#define ENDIAN_LITTLE		1
#define ENDIAN_BIG		2

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
 * ---------- macro functions -------------------------------------------------
 */

/*
 * true
 */

#define CORE_TRUE()							\
  return (TRUE)

/*
 * false
 */

#define CORE_FALSE()							\
  return (FALSE)

/*
 * leave
 */

#define CORE_LEAVE()							\
  {									\
    return (STATUS_OK);							\
  }

/*
 * escape
 */

#define CORE_ESCAPE(_format_, _arguments_...)				\
  {									\
    module_call(report, record,						\
		_format_ " (%s:%u)",					\
		##_arguments_, __FUNCTION__, __LINE__);			\
									\
    return (STATUS_ERROR);						\
  }

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
#include <core/clock.h>
#include <core/io.h>
#include <core/wait.h>
#include <core/thread.h>
#include <core/task.h>
#include <core/cpu.h>
#include <core/scheduler.h>

// [XXX:improvements] re-develop everything below
#include <core/message.h>
#include <core/capability.h>
#include <core/interface.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/core.c
 */

/*
 * ../../core/core.c
 */

void			kaneton(s_init*				init);

t_status		kaneton_spawn(void);


/*
 * eop
 */

#endif
