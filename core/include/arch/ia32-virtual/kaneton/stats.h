/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * stats.h
 * 
 * path          /home/mycure/kaneton/core/include/arch/ia32/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:23:41 2005   mycure
 * last update   Sat Sep 10 15:16:31 2005   mycure
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
