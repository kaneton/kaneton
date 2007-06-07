/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../arch/ibm-pc.ia32-virtual/architecture.h
 *
 * created       julien quintard   [thu jun  7 12:02:10 2007]
 * updated       julien quintard   [thu jun  7 14:12:55 2007]
 */

#ifndef ARCHITECTURE_ARCHITECTURE_H
#define ARCHITECTURE_ARCHITECTURE_H	1

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

/*
 * ---------- includes --------------------------------------------------------
 */

#include <arch/machdep/debug.h>

#include <arch/machdep/cons.h>

#include <arch/machdep/elf.h>

#endif
