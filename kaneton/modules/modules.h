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
 * updated       julien quintard   [fri may  1 19:26:17 2009]
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
