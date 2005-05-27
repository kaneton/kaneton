/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * machdep.h
 * 
 * path          /home/mycure/data/research/projects/kaneton/source/core/include/arch/ia32
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:23:53 2005   mycure
 * last update   Fri May 27 10:37:46 2005   mycure
 */

#ifndef IA32_MACHDEP_H
#define IA32_MACHDEP_H

/*
 * includes
 */

#include <arch/machdep/asm.h>
#include <arch/machdep/limits.h>
#include <arch/machdep/pio.h>
#include <arch/machdep/stdarg.h>
#include <arch/machdep/types.h>

#include <arch/machdep/multiboot.h>

#include <arch/machdep/as.h>
#include <arch/machdep/cpu.h>
#include <arch/machdep/map.h>
#include <arch/machdep/msg.h>
#include <arch/machdep/region.h>
#include <arch/machdep/sched.h>
#include <arch/machdep/segment.h>
#include <arch/machdep/set.h>
#include <arch/machdep/task.h>
#include <arch/machdep/thread.h>
#include <arch/machdep/time.h>
#include <arch/machdep/trap.h>

/*
 * architecture defines
 */

#define ___endian			LITTLE_ENDIAN
#define ___32bits			1

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
