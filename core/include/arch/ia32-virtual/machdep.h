/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/include/arch/machdep/machdep.h
 *
 * created       julien quintard   [fri feb 11 02:23:53 2005]
 * updated       matthieu bucchianeri   [tue dec 20 14:26:39 2005]
 */

#ifndef IA32_MACHDEP_H
#define IA32_MACHDEP_H		1

/*
 * ---------- includes --------------------------------------------------------
 */

#include <arch/libia32/libia32.h>
#include <arch/machdep/kaneton/kaneton.h>
#include <arch/machdep/services/services.h>
#include <arch/machdep/sys/sys.h>

/*
 * ---------- macros ----------------------------------------------------------
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
