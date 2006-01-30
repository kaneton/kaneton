/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/core/include/arch/ia32-virtual/kaneton/stats.h
 *
 * created       julien quintard   [fri feb 11 02:23:41 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:25:29 2006]
 */

#ifndef IA32_KANETON_STATS_H
#define IA32_KANETON_STATS_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <kaneton/stats.h>

/*
 * ---------- macros ----------------------------------------------------------
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
