/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.NEW/kaneton/core/include/error.h
 *
 * created       julien quintard   [wed jun  6 13:02:28 2007]
 * updated       julien quintard   [tue nov 23 14:45:51 2010]
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

#define ERROR_OK		(1 << 0)
#define ERROR_KO		(1 << 1)

/*
 * ---------- macro functions -------------------------------------------------
 */

#define assert(_test_)							\
  if (!(_test_))							\
    {									\
      module_call(report, report_dump);					\
									\
      printf("[!] assertion failed in '%s' (%s:%u)\n",			\
             #_test_, __FUNCTION__, __LINE__);				\
									\
      while (1)								\
	;								\
    }

/*
 * the other values are reserved for the specific managers use.
 */

#endif
