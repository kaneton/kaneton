/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/core/include/kaneton/kaneton.h
 *
 * created       julien quintard   [fri feb 11 02:19:44 2005]
 * updated       matthieu bucchianeri   [mon jan 30 22:41:22 2006]
 */

#ifndef KANETON_KANETON_H
#define KANETON_KANETON_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>

/*
 * ---------- defines ---------------------------------------------------------
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
 * ---------- macros ----------------------------------------------------------
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

#include <kaneton/conf.h>

#include <kaneton/as.h>

/*                                                                [cut] k1   */
#include <kaneton/serial.h>
#include <kaneton/debug.h>
/*                                                               [cut] /k1   */

#include <kaneton/error.h>
#include <kaneton/types.h>

#include <kaneton/id.h>
#include <kaneton/init.h>
#include <kaneton/region.h>
#include <kaneton/segment.h>
#include <kaneton/set.h>
#include <kaneton/stats.h>
#include <kaneton/task.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/kaneton.c
 */

/*
 * ../../kaneton/kaneton.c
 */

void			kaneton(t_init*				bootloader);

void			kaneton_dump(void);


/*
 * eop
 */

#endif
