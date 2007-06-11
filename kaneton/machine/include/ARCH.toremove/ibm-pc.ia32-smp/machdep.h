/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/arch/ia32-smp/machdep.h
 *
 * created       julien quintard   [fri feb 11 02:23:53 2005]
 * updated       matthieu bucchianeri   [mon aug 21 14:53:01 2006]
 */

#ifndef IA32_MACHDEP_H
#define IA32_MACHDEP_H		1

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libia32.h>
#include <arch/machdep/core/core.h>
#include <arch/machdep/services/services.h>
#include <arch/machdep/sys/sys.h>

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * machdep transparent traps
 */

#define		machine_include(_mng_)					\
  machine_include_##_mng_()

#define		machine_call(_mng_, _function_, _args_...)		\
  machine_call_##_mng_(_function_, ##_args_)

#define		machine_data(_object_)					\
  machine_data_##_object_()

#endif
