/*
 * licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/env/users/matthieu.bucchianeri/conf.h
 *
 * created       matthieu bucchianeri   [fri dec  2 18:50:46 2005]
 * updated       matthieu bucchianeri   [fri aug 18 17:46:11 2006]
 */

#ifndef KANETON_CONF_H
#define KANETON_CONF_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/debug.h>

#define CONF_MODE_DEVELOPMENT	(1 << 0)
#define CONF_MODE_RELEASE	(1 << 1)

#define CONF_MODE		MODE_DEVELOPMENT

/*
 * which algorithm to use in segment
 */

#define SEGMENT_LOOKUP_ALGORITHM	FIT_FIRST

/*
 * which algorithm to use in region
 */

#define REGION_LOOKUP_ALGORITHM		FIT_FIRST

/*
 * which fit method to use in alloc
 */

#define ALLOC_FIT	FIT_FIRST

/*
 * this define activates the statistics
 */

#undef CONF_STATS

/*
 * this define starts the tests by hand.
 */

#undef CONF_ENABLE_CHECK

/*
 * the configuration debug
 */

#define CONF_DEBUG							\
  (0)

/*
 * the machine-dependent configuration debug
 */

#define CONF_MACHDEP_DEBUG						\
  (0)

/*
 * serial port debug
 */

#define SERIAL

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/conf/conf.c
 */

/*
 * ../../core/conf/conf.c
 */


/*
 * eop
 */

#endif
