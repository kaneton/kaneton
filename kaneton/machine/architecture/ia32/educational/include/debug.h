/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...tecture/ia32/educational/include/debug.h
 *
 * created       julien quintard   [wed jun  6 16:15:05 2007]
 * updated       matthieu bucchianeri   [sun jun 17 17:03:16 2007]
 */

#ifndef ARCHITECTURE_DEBUG_H
#define ARCHITECTURE_DEBUG_H		1

/*
 * ---------- macros ----------------------------------------------------------
 */

#define IA32_DEBUG_INIT		(1 << 0)
#define IA32_DEBUG_PMODE	(1 << 1)
#define IA32_DEBUG_PAGING	(1 << 2)
#define IA32_DEBUG_VIRTUAL	(IA32_DEBUG_PMODE | IA32_DEBUG_PAGING)

#define IA32_DEBUG							\
  (0)

#endif
