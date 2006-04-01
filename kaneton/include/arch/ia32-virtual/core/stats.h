/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/arch/ia32-virtual/core/stats.h
 *
 * created       julien quintard   [fri feb 11 02:23:41 2005]
 * updated       julien quintard   [sat apr  1 22:59:00 2006]
 */

#ifndef IA32_CORE_STATS_H
#define IA32_CORE_STATS_H	1

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
