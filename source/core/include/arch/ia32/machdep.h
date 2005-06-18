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
 * last update   Sun Jun 19 00:21:42 2005   mycure
 */

#ifndef IA32_MACHDEP_H
#define IA32_MACHDEP_H		1

/* XXX to move in kaneton/debug.h */
/*
 * debug
 */

#define IA32_DEBUG_INIT		0x1
#define IA32_DEBUG_PMODE	0x2
#define IA32_DEBUG_PAGING	0x4

#define IA32_DEBUG							\
  (0)

/*
 * includes
 */

#include <arch/machdep/ia32/ia32.h>
#include <arch/machdep/kaneton/kaneton.h>
#include <arch/machdep/services/services.h>
#include <arch/machdep/sys/sys.h>

/*
 * includes
 */

#include <arch/machdep/asm.h>
#include <arch/machdep/stdarg.h>
#include <arch/machdep/types.h>

#include <arch/machdep/multiboot.h>
#include <arch/machdep/elf.h>

#include <arch/machdep/kaneton.h>
#include <arch/machdep/init.h>
#include <arch/machdep/cons.h>
#include <arch/machdep/pmode.h>
#include <arch/machdep/paging.h>
#include <arch/machdep/bootloader.h>

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
 * bootloader memory description
 */

typedef struct		s_machdep_init
{
  t_gdte*		gdt;
  t_pde*		pd;
  t_cons		cons;
}			t_machdep_init;

#define		machdep_include_init()					\
  t_machdep_init	machdep

#endif
