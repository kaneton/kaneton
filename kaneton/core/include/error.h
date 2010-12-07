/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.STABLE/kaneton/core/include/error.h
 *
 * created       julien quintard   [wed jun  6 13:02:28 2007]
 * updated       julien quintard   [sun dec  5 15:11:53 2010]
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

#define ERROR_OK		42
#define ERROR_KO		666

#define ERROR_TRUE		1
#define ERROR_FALSE		0

/*
 * ---------- macro functions -------------------------------------------------
 */

#define assert(_test_)							\
  if (!(_test_))							\
    {									\
      module_call(report, dump);					\
									\
      module_call(console, message,					\
		  '!', "assertion failed in '%s' (%s:%u)\n",		\
		  #_test_, __FUNCTION__, __LINE__);			\
									\
      while (1)								\
	;								\
    }

/*
 * the other values are reserved for the specific managers use.
 */

#endif
