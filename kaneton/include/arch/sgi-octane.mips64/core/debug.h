/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/sgi-octane.mips64/core/debug.h
 *
 * created       Enguerrand RAYMOND   [tue oct 17 13:26:00 2006]
 * updated       Enguerrand RAYMOND   [tue oct 17 13:26:36 2006]
 */

#ifndef OCTANE_CORE_DEBUG_H
#define OCTANE_CORE_DEBUG_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

#define OCTANE_DEBUG_INIT	(1 << 0)
#define OCTANE_DEBUG_PMODE	(1 << 1)
#define OCTANE_DEBUG_PAGING	(1 << 2)

#define OCTANE_DEBUG							\
  (CONF_MACHDEP_DEBUG)

#endif
