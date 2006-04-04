/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/core.h
 *
 * created       julien quintard   [fri feb 11 02:19:44 2005]
 * updated       julien quintard   [sat apr  1 23:31:22 2006]
 */

#ifndef CORE_CORE_H
#define CORE_CORE_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>

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

#define WORDSZ_32		(1 << 0)
#define WORDSZ_64		(1 << 1)
#define WORDSZ_128		(1 << 2)

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

#define kaneton_error(_fmt_...)						\
  {									\
    printf("%#[%#!%#]%# %s:%u: ",					\
           CONS_FRONT(CONS_BLUE) | CONS_BACK(CONS_BLACK) | CONS_INT,	\
           CONS_FRONT(CONS_RED) | CONS_BACK(CONS_BLACK) | CONS_INT,	\
           CONS_FRONT(CONS_BLUE) | CONS_BACK(CONS_BLACK) | CONS_INT,	\
           CONS_FRONT(CONS_WHITE) | CONS_BACK(CONS_BLACK) | CONS_INT,	\
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

#include <core/conf.h>

#include <core/as.h>

/*                                                                [cut] k1   */
#include <core/serial.h>
/*                                                               [cut] /k1   */
#include <core/debug.h>

#include <core/error.h>
#include <core/types.h>

#include <core/id.h>
#include <core/init.h>
#include <core/region.h>
#include <core/segment.h>
#include <core/map.h>
#include <core/set.h>
#include <core/stats.h>
#include <core/event.h>
#include <core/timer.h>
#include <core/thread.h>
#include <core/task.h>

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


/*
 * eop
 */

#endif
