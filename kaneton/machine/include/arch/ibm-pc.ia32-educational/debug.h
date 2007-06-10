/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...include/arch/ibm-pc.ia32-virtual/debug.h
 *
 * created       julien quintard   [wed jun  6 16:15:05 2007]
 * updated       julien quintard   [thu jun  7 14:41:29 2007]
 */

#ifndef ARCHITECTURE_DEBUG_H
#define ARCHITECTURE_DEBUG_H		1

/*
 * ---------- macros ----------------------------------------------------------
 */

#define IA32_DEBUG_INIT		(1 << 0)
#define IA32_DEBUG_PMODE	(1 << 1)
#define IA32_DEBUG_PAGING	(1 << 2)

#define IA32_DEBUG							\
  (CONF_MACHDEP_DEBUG)

#endif
