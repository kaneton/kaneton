/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/machdep/machdep.h
 *
 * created       Enguerrand RAYMOND   [sun oct 15 22:04:48 2006]
 * updated       Enguerrand RAYMOND   [mon oct 16 00:02:09 2006]
 */

#ifndef OCTANE_MACHDEP_H
#define OCTANE_MACHDEP_H	1

/*
 * ---------- includes --------------------------------------------------------
 */

#include <core/types.h>
#include <liboctane.h>

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
