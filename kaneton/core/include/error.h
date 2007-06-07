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
 * updated       julien quintard   [thu jun  7 12:08:49 2007]
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
 * the other values are reserved for the specific managers use.
 */

#endif
