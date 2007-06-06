/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/sgi-octane.mips64/core/debug.h
 *
 * created       enguerrand raymond   [wed oct 18 14:15:50 2006]
 * updated       enguerrand raymond   [wed oct 18 14:16:19 2006]
 */

#ifndef MIPS64_CORE_DEBUG_H
#define MIPS64_CORE_DEBUG_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

#define MIPS64_DEBUG_INIT	(1 << 0)
#define MIPS64_DEBUG_PMODE	(1 << 1)
#define MIPS64_DEBUG_PAGING	(1 << 2)

#define MIPS64_DEBUG			\
  (CONF_MACHDEP_DEBUG)

#endif
