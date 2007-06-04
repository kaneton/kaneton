/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/arch/ia32-virtual/machdep.h
 *
 * created       julien quintard   [fri feb 11 02:23:53 2005]
 * updated       julien quintard   [sat apr  1 23:00:17 2006]
 */

#ifndef IA32_MACHDEP_H
#define IA32_MACHDEP_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * machdep transparent traps
 */

#define		machdep_include(_mng_)					\
  machdep_include_##_mng_()

#define		machdep_call(_mng_, _function_, _args_...)		\
  machdep_call_##_mng_(_function_, ##_args_)

#define		machdep_data(_object_)					\
  machdep_data_##_object_()

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libia32.h>

#include <arch/machdep/core/core.h>
#include <arch/machdep/services/services.h>
#include <arch/machdep/sys/sys.h>

#endif
