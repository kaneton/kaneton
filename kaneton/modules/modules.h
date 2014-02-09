/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          kaneton/kaneton/modules/modules.h
 *
 * created       julien quintard   [fri may  1 12:58:24 2009]
 * updated       julio guerra   [sun sep  8 13:59:39 2013]
 */

#ifndef MODULES_MODULES_H
#define MODULES_MODULES_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- includes --------------------------------------------------------
 */

#include <modules/console/include/console.h>
#include <modules/report/include/report.h>
#include <modules/forward/include/forward.h>
#include <modules/test/include/test.h>

/*
 * ---------- macro-functions -------------------------------------------------
 */

#define module_load(_module_)						\
  module_call_ ## _module_ (load)

#define module_unload(_module_)						\
  module_call_ ## _module_ (unload)

#define module_call(_module_, _function_, _arguments_...)		\
  module_call_ ## _module_ (_function_, ##_arguments_)

/*
 * ---------- modules ---------------------------------------------------------
 */

/*
 * console
 */
#ifdef MODULE_console
# define module_call_console(_function_, _arguments_...)		\
  module_console_ ## _function_ (_arguments_)
#else
# define module_call_console(_function_, _arguments_...)
#endif

/*
 * report
 */
#ifdef MODULE_report
# define module_call_report(_function_, _arguments_...)			\
  module_report_ ## _function_ (_arguments_)
#else
# define module_call_report(_function_, _arguments_...)
#endif

/*
 * forward
 */
#ifdef MODULE_forward
# define module_call_forward(_function_, _arguments_...)		\
  module_forward_ ## _function_ (_arguments_)
#else
# define module_call_forward(_function_, _arguments_...)
#endif

/*
 * test
 */
#ifdef MODULE_test
# define module_call_test(_function_, _arguments_...)			\
  module_test_ ## _function_ (_arguments_)
#else
# define module_call_test(_function_, _arguments_...)
#endif

/*
 * debug
 */
#ifdef MODULE_debug
# define module_call_debug(_function_, _arguments_...)			\
  module_debug_ ## _function_ (_arguments_)
#else
# define module_call_debug(_function_, _arguments_...)
#endif

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * escape
 */

#define MODULE_ESCAPE(_format_, _arguments_...)				\
  {									\
    module_call(report, record,						\
		_format_ " (%s:%u)",					\
		##_arguments_, __FUNCTION__, __LINE__);			\
									\
    return (STATUS_ERROR);							\
  }

/*
 * leave
 */

#define MODULE_LEAVE()							\
  {									\
    return (STATUS_OK);							\
  }

#endif
