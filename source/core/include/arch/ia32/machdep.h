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
 * last update   Sat May 28 17:09:07 2005   mycure
 */

#ifndef IA32_MACHDEP_H
#define IA32_MACHDEP_H

/*
 * includes
 */

#include <arch/machdep/ia32/asm.h>
#include <arch/machdep/ia32/pio.h>
#include <arch/machdep/ia32/stdarg.h>
#include <arch/machdep/ia32/types.h>

#include <arch/machdep/sys/multiboot.h>
#include <arch/machdep/sys/elf.h>

#include <arch/machdep/kaneton/as.h>
#include <arch/machdep/kaneton/cpu.h>
#include <arch/machdep/kaneton/map.h>
#include <arch/machdep/kaneton/msg.h>
#include <arch/machdep/kaneton/region.h>
#include <arch/machdep/kaneton/sched.h>
#include <arch/machdep/kaneton/segment.h>
#include <arch/machdep/kaneton/set.h>
#include <arch/machdep/kaneton/task.h>
#include <arch/machdep/kaneton/thread.h>
#include <arch/machdep/kaneton/time.h>
#include <arch/machdep/kaneton/trap.h>

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
