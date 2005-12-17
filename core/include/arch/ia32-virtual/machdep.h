/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * machdep.h
 * 
 * path          /home/mycure/kaneton/core
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:23:53 2005   mycure
 * last update   Thu Nov 10 23:03:46 2005   mycure
 */

#ifndef IA32_MACHDEP_H
#define IA32_MACHDEP_H		1

/*
 * ---------- includes --------------------------------------------------------
 */

#include <arch/machdep/ia32/ia32.h>
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
