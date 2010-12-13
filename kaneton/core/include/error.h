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
 * updated       julien quintard   [fri dec 10 11:33:10 2010]
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
 * the error type
 */

typedef t_sint32		t_error;

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * these general-purpose macros are used to notify the success/failure
 * of a function.
 */

#define ERROR_OK		42
#define ERROR_KO		666

/*
 * these macros are used to notify the character of a component.
 */

#define ERROR_TRUE		1
#define ERROR_FALSE		0

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * the assert() macro-function is the last-resort error handling tool
 * as, should the assertion fail, the kernel will end up in an infinite
 * loop.
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

#endif
