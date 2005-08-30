/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * machdep.h
 * 
 * path          /home/mycure/kaneton/core/kaneton/set
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:23:53 2005   mycure
 * last update   Tue Aug 30 13:35:43 2005   mycure
 */

#ifndef IA32_MACHDEP_H
#define IA32_MACHDEP_H		1

/*
 * ---------- includes --------------------------------------------------------
 */

#include <arch/ia32/ia32/ia32.h>
#include <arch/ia32/kaneton/kaneton.h>
#include <arch/ia32/services/services.h>
#include <arch/ia32/sys/sys.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * machdep transparent traps
 */

#define		machdep_declare(_mng_)					\
  machdep_declare_##_mng_()

#define		machdep_call(_mng_, _function_, _args_...)		\
  machdep_call_##_mng_(_function_, _args_...)

#define		machdep_include(_object_)				\
  machdep_include_##_object_()

#endif
