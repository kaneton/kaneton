/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/core/include/arch/ia32-segment/kaneton/debug.h
 *
 * created       julien quintard   [fri feb 11 02:23:53 2005]
 * updated       julien quintard   [fri mar 10 03:57:24 2006]
 */

#ifndef IA32_KANETON_DEBUG_H
#define IA32_KANETON_DEBUG_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

#define IA32_DEBUG_INIT		(1 << 0)
#define IA32_DEBUG_PMODE	(1 << 1)

#define IA32_DEBUG							\
  (CONF_MACHDEP_DEBUG)

#endif
