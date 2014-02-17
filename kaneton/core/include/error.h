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
 * the status type
 */

typedef enum
{
    STATUS_OK    = 42,
    STATUS_ERROR = 666
} t_status;

typedef enum
{
    FALSE = 0,
    TRUE  = 1
} t_bool;

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
