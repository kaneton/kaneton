/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/arch/ia32-virtual/core/debug.h
 *
 * created       julien quintard   [fri feb 11 02:23:53 2005]
 * updated       julien quintard   [sat apr  1 22:57:01 2006]
 */

#ifndef IA32_CORE_DEBUG_H
#define IA32_CORE_DEBUG_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

#define IA32_DEBUG_INIT		(1 << 0)
#define IA32_DEBUG_PMODE	(1 << 1)
#define IA32_DEBUG_PAGING	(1 << 2)

#define IA32_DEBUG							\
  (CONF_MACHDEP_DEBUG)

#endif
