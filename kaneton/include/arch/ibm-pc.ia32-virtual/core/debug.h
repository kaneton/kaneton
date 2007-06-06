/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...aneton/include/arch/machdep/core/debug.h
 *
 * created       julien quintard   [wed jun  6 16:15:05 2007]
 * updated       julien quintard   [wed jun  6 16:15:19 2007]
 */

#ifndef GUARD_IA32_CORE_DEBUG
#define GUARD_IA32_CORE_DEBUG		1

/*
 * ---------- macros ----------------------------------------------------------
 */

#define IA32_DEBUG_INIT		(1 << 0)
#define IA32_DEBUG_PMODE	(1 << 1)
#define IA32_DEBUG_PAGING	(1 << 2)

#define IA32_DEBUG							\
  (CONF_MACHDEP_DEBUG)

#endif
