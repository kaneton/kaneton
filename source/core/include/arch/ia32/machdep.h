/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * machdep.h
 * 
 * path          /home/mycure/kaneton/core/include/arch/ia32
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:23:53 2005   mycure
 * last update   Sun Jun 19 20:15:19 2005   mycure
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

/*
 * ---------- prototypes ------------------------------------------------------
 */

/*
 * prototypes:     ../../../kaneton/arch/ia32/as.c
 *                 ../../../kaneton/arch/ia32/segment.c
 */

/*
 * ../../../kaneton/arch/ia32/as.c
 */

/*
 * ../../../kaneton/arch/ia32/segment.c
 */
/*
 * eop
 */

#endif
