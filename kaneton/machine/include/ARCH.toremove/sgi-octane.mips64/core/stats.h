/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/sgi-octane.mips64/core/stats.h
 *
 * created       enguerrand raymond   [wed oct 18 14:18:51 2006]
 * updated       enguerrand raymond   [wed oct 18 14:19:00 2006]
 */

#ifndef MIPS64_CORE_STATS_H
#define MIPS64_CORE_STATS_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/stats.h>

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_stats()

#define		machine_call_stats(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      _r_;								\
    }									\
  )

#define		machine_data_stats()

#endif
