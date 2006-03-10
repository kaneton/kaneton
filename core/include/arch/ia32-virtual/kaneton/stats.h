/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/core/include/arch/ia32-virtual/kaneton/stats.h
 *
 * created       julien quintard   [fri feb 11 02:23:41 2005]
 * updated       julien quintard   [fri mar 10 04:02:45 2006]
 */

#ifndef IA32_KANETON_STATS_H
#define IA32_KANETON_STATS_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <kaneton/stats.h>

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
