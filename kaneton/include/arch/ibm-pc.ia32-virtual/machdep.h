/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/arch/machdep/machdep.h
 *
 * created       julien quintard   [wed jun  6 15:50:24 2007]
 * updated       julien quintard   [wed jun  6 15:50:48 2007]
 */

#ifndef GUARD_IA32_MACHDEP_H
#define GUARD_IA32_MACHDEP_H		1

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

#endif

/*
 * ---------- includes --------------------------------------------------------
 */

#include <arch/machdep/core/core.h>
#include <arch/machdep/services/services.h>
#include <arch/machdep/sys/sys.h>
