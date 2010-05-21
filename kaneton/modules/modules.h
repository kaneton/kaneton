/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/modules/modules.h
 *
 * created       julien quintard   [fri may  1 12:58:24 2009]
 * updated       julien quintard   [mon may 17 12:36:55 2010]
 */

#ifndef MODULES_MODULES_H
#define MODULES_MODULES_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- macro-functions -------------------------------------------------
 */

#define module_call(_module_, _function_, _arguments_...)		\
  module_call_ ## _module_ (_function_, ##_arguments_)

#define module_dispatch(_function_, _arguments_...)			\
  _function_(_arguments_)

/*
 * ---------- modules ---------------------------------------------------------
 */

/*
 * console
 */
#ifdef MODULE_console
# include <modules/console/include/console.h>
# define module_call_console(_function_, _arguments_...)		\
  module_dispatch(_function_, ##_arguments_)
#else
# define module_call_console(_function_, _arguments_...)
#endif

/*
 * forward
 */
#ifdef MODULE_forward
# include <modules/forward/include/forward.h>
# define module_call_forward(_function_, _arguments_...)		\
  module_dispatch(_function_, ##_arguments_)
#else
# define module_call_forward(_function_, _arguments_...)
#endif

/*
 * test
 */
#ifdef MODULE_test
# include <modules/test/include/test.h>
# define module_call_test(_function_, _arguments_...)			\
  module_dispatch(_function_, ##_arguments_)
#else
# define module_call_test(_function_, _arguments_...)
#endif

#endif
