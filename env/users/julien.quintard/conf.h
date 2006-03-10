/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/core/include/kaneton/conf.h
 *
 * created       julien quintard   [sat dec 17 17:24:46 2005]
 * updated       julien quintard   [fri mar 10 03:37:23 2006]
 */

#ifndef KANETON_CONF_H
#define KANETON_CONF_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <kaneton/debug.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * the current kernel mode, release, development.
 *
 * the development mode means that the current kernel was not
 * entirely tested while the release mode indicates that the kernel can
 * be used.
 */

#define CONF_MODE_DEVELOPMENT	(1 << 0)
#define CONF_MODE_RELEASE	(1 << 1)

#define CONF_MODE		MODE_DEVELOPMENT

/*
 * which fit method to use in segment
 */

#define SEGMENT_FIT	FIT_FIRST

/*
 * which fit method to use in region
 */

#define REGION_FIT	FIT_FIRST

/*
 * which fit method to use in alloc
 */

#define ALLOC_FIT	FIT_FIRST

/*
 * this define activates the statistics
 */

#undef CONF_STATS

/*
 * the configuration debug
 */

#define CONF_DEBUG							\
  (DEBUG_TASK)

/*
 * the machine-dependent configuration debug
 */

#define CONF_MACHDEP_DEBUG						\
  (0)

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/conf/conf.c
 */

/*
 * ../../kaneton/conf/conf.c
 */


/*
 * eop
 */

#endif
