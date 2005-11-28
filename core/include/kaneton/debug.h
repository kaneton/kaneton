/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/core/include/kaneton/debug.h
 *
 * created       julien quintard   [mon nov 28 19:37:01 2005]
 * updated       julien quintard   [mon nov 28 19:37:14 2005]
 */

#ifndef KANETON_DEBUG_H
#define KANETON_DEBUG_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

#define DEBUG_PARAMS		(1 << 0)
#define DEBUG_AS		(1 << 1)
#define DEBUG_SEGMENT		(1 << 2)
#define DEBUG_SET		(1 << 3)
#define DEBUG_STATS		(1 << 4)

#define DEBUG								\
  (CONF_DEBUG)

/*
 * ---------- types -----------------------------------------------------------
 */

typedef	struct		s_debug_function
{
	char		*f_name;
	int		(*f)(char *str);
}			debug_function_t;



/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/debug/debug.c
 */

/*
 * eop
 */

#endif
