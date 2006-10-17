/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/sgi-octane.mips64/core/stats.h
 *
 * created       Enguerrand RAYMOND   [tue oct 17 13:32:31 2006]
 * updated       Enguerrand RAYMOND   [tue oct 17 13:32:32 2006]
 */

#ifndef OCTANE_CORE_STATS_H
#define OCTANE_CORE_STATS_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/stats.h>

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machdep_include_stats()

#define		machdep_call_stats(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      _r_;								\
    }									\
  )

#define		machdep_data_stats()

#endif
