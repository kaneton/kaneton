/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/export_kaneton/kaneton/core/include/arch/ia32-segment/kaneton/debug.h
 *
 * created       julien quintard   [fri feb 11 02:23:53 2005]
 * updated       matthieu bucchianeri   [mon jan 30 20:34:50 2006]
 */

#ifndef IA32_KANETON_DEBUG_H
#define IA32_KANETON_DEBUG_H	1

/*
 * ---------- defines ---------------------------------------------------------
 */

#define IA32_DEBUG_INIT		(1 << 0)
#define IA32_DEBUG_PMODE	(1 << 1)

#define IA32_DEBUG							\
  (CONF_MACHDEP_DEBUG)

#endif
