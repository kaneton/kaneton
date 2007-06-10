/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/sgi-octane.mips64/machdep.h
 *
 * created       Enguerrand RAYMOND   [tue oct 17 14:03:09 2006]
 * updated       enguerrand raymond   [wed oct 18 14:23:37 2006]
 */

#ifndef OCTANE_MACHDEP_H
#define OCTANE_MACHDEP_H	1

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libmipsr10000.h>
#include <arch/machdep/core/core.h>
#include <arch/machdep/services/services.h>
#include <arch/machdep/sys/sys.h>
#include <libc.h>

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
