/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/error.h
 *
 * created       julien quintard   [wed jun  6 13:02:28 2007]
 * updated       julien quintard   [sat may  2 00:16:37 2009]
 */

#ifndef CORE_ERROR_H
#define CORE_ERROR_H			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * error type
 */

typedef t_sint32		t_error;

/*
 * ---------- macros ----------------------------------------------------------
 */

#define ERROR_NONE		(1 << 0)
#define ERROR_UNKNOWN		(1 << 1)

/*
 * ---------- macro functions -------------------------------------------------
 */

#define assert(_test_)							\
  if (!(_test_))							\
    {									\
      printf("[assert] '%s' failed at %s:%d\n",				\
             #_test_, __FILE__, __LINE__);				\
									\
      while (1)								\
	;								\
    }

/*
 * the other values are reserved for the specific managers use.
 */

#endif
