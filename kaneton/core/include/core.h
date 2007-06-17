/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/core.h
 *
 * created       julien quintard   [wed jun  6 12:22:26 2007]
 * updated       matthieu bucchianeri   [sun jun 17 20:37:52 2007]
 */

#ifndef CORE_CORE_H
#define CORE_CORE_H			1

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * endian
 */

#define ENDIAN_LITTLE		(1 << 0)
#define ENDIAN_BIG		(1 << 1)

/*
 * word size
 */

#define WORDSZ_8		(1 << 0)
#define WORDSZ_16		(1 << 1)
#define WORDSZ_32		(1 << 2)
#define WORDSZ_64		(1 << 3)
#define WORDSZ_128		(1 << 4)

/*
 * perms
 */

#define PERM_READ		(1 << 0)
#define PERM_WRITE		(1 << 1)
#define PERM_EXEC		(1 << 2)

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

#define core_error(_fmt_...)						\
  {									\
    printf("[!] %s:%u: ",						\
           __FILE__,							\
           __LINE__);							\
    printf(_fmt_);							\
									\
    while (1)								\
      ;									\
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

#include <core/debug.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/core.c
 */

/*
 * ../../core/core.c
 */

void			kaneton(t_init*				bootloader);

void			kaneton_dump(void);

t_error			kaneton_launch(void);


/*
 * eop
 */

#endif
