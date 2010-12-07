/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...STABLE/kaneton/machine/include/machine.h
 *
 * created       julien quintard   [thu jun  7 12:46:25 2007]
 * updated       julien quintard   [sat dec  4 23:06:46 2010]
 */

#ifndef MACHINE_H
#define MACHINE_H			1

/*
 * ---------- includes --------------------------------------------------------
 */

#include <glue/glue.h>
#include <architecture/architecture.h>
#include <platform/platform.h>

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * escape
 */

#define MACHINE_ESCAPE(_format_, _arguments_...)			\
  {									\
    module_call(report, record,						\
		_format_ " (%s:%u)",					\
		##_arguments_, __FUNCTION__, __LINE__);			\
									\
    return (ERROR_KO);							\
  }

/*
 * leave
 */

#define MACHINE_LEAVE()							\
  {									\
    return (ERROR_OK);							\
  }

#endif
