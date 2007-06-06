/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/error.h
 *
 * created       julien quintard   [wed jun  6 13:02:28 2007]
 * updated       julien quintard   [wed jun  6 15:55:51 2007]
 */

#ifndef GUARD_CORE_ERROR
#define GUARD_CORE_ERROR		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/core.h>

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
 * the other values are reserved for the specific managers use.
 */

#endif
